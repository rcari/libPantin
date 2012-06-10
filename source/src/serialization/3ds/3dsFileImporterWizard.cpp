/*
 * 	Copyright (c) 2010-2011, Christophe EVENO
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *		* Neither the name of the <organization> nor the
 *		  names of its contributors may be used to endorse or promote products
 *		  derived from this software without specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <serialization/3ds/3dsFileImporterWizard.hpp>
#include <serialization/3ds/3dsSearchingTasklet.hpp>
#include <serialization/3ds/3dsSerializer.hpp>
#include <serialization/3ds/3dsStructures.hpp>
#include <serialization/FileImporterWizard.hpp>
using namespace Pantin::serialization::ThreeDS;
using namespace Pantin::serialization;

#include <commands/ProjectAddModel.hpp>
using namespace Pantin::commands;

#include <KoreEngine.hpp>
using namespace Kore;
using namespace Kore::parallel;

#include <PantinEngine.hpp>
using namespace Pantin;

#include <COLLADA/DocumentInstance.hpp>
using namespace Nigel::COLLADA;

#include <project/Project.hpp>
#include <project/Model.hpp>
using namespace Nigel::project;

#include <geometry/GeometriesLibrary.hpp>
#include <geometry/Geometry.hpp>
using namespace Nigel::geometry;

#include <scene/Node.hpp>
#include <scene/VisualScene.hpp>
#include <scene/VisualSceneInstance.hpp>
using namespace Nigel::scene;

#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QHeaderView>
#include <QtGui/QAbstractButton>

#define RANGE_MAX 1000

GetObjectsProgressPage::GetObjectsProgressPage()
:	QWizardPage()
{
	setTitle(tr("Scanning for objects in the file"));

	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(_progress = new QProgressBar);
	vLayout->addSpacing(10);
	vLayout->addWidget(_text = new QTextEdit);
	vLayout->addStretch();

	_text->setReadOnly(true);

	setLayout(vLayout);
}

GetObjectsProgressPage::~GetObjectsProgressPage()
{
	ThreeDSSearchingTasklet* tasklet = (ThreeDSSearchingTasklet*)((FileImporterWizard*)wizard())->context();
	if(tasklet)
	{
		delete tasklet;
		((FileImporterWizard*)wizard())->setContext(K_NULL);
	}
}

void GetObjectsProgressPage::initializePage()
{
	// Disable cancel button for this step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(false);
	}

	// Creating searching tasklet
	ThreeDSSearchingTasklet* tasklet = new ThreeDSSearchingTasklet();
	((FileImporterWizard*)wizard())->setContext(tasklet);
	connect(tasklet, SIGNAL(ended(kint)), SLOT(ended(kint)));
	connect(tasklet, SIGNAL(progress(const QString&)), SLOT(progress(const QString&)));
	connect(tasklet, SIGNAL(progress(kuint64,kuint64)), SLOT(progress(kuint64,kuint64)));

	// Running searching tasklet
	_ended = Tasklet::NotStarted;
	tasklet->setFile(((FileImporterWizard*)wizard())->file());
	KoreEngine::RunTasklet(tasklet, TaskletRunner::Asynchronous);
	emit completeChanged();
}

bool GetObjectsProgressPage::validatePage()
{
	// Re-Enable cancel button for the next step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(true);
	}
	return true;
}

bool GetObjectsProgressPage::isComplete() const
{
	return _ended;
}

int GetObjectsProgressPage::nextId() const
{
	if(_ended == Tasklet::Completed)
	{
		return ThreeDSSerializer::ObjectsChoicesPageId;
	}
	return FileImporterWizard::ErrorPageId;
}

void GetObjectsProgressPage::ended(kint status)
{
	_ended = status;
	emit completeChanged();
}

void GetObjectsProgressPage::progress(const QString& message)
{
	QString text = _text->toPlainText();
	if(text.isEmpty())
	{
		_text->setPlainText(message);
	}
	else
	{
		_text->setPlainText(text + QString("\n") + message);
	}
}

void GetObjectsProgressPage::progress(kuint64 progress, kuint64 total)
{
	if(progress != total)
	{
		_progress->setRange(0, RANGE_MAX);
		double ratio = (kdouble)progress / (kdouble)total;
		_progress->setValue((int)( ratio * RANGE_MAX ));
	}
	else
	{
		_progress->setValue(RANGE_MAX);
	}
}



ObjectsChoicesPage::ObjectsChoicesPage()
:	QWizardPage()
{
	setTitle(tr("Select objects you want to import:"));

	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(_table = new QTableWidget);
	vLayout->addWidget(_checkbox = new QCheckBox);

	_table->setColumnCount(3);
	_table->setHorizontalHeaderLabels(QStringList() << tr("") << tr("Object name") << tr("Object type"));
	_table->verticalHeader()->hide();
	_table->setSelectionMode(QAbstractItemView::NoSelection);
	_checkbox->setTristate(true);
	_checkbox->setText(tr("Select/Deselect all"));
	connect(_checkbox, SIGNAL(stateChanged(int)), SLOT(selectionChanged(int)));

	setLayout(vLayout);
}

void ObjectsChoicesPage::initializePage()
{
	// Fill table with the result of the search
	ThreeDSSearchingTasklet* tasklet = (ThreeDSSearchingTasklet*)((FileImporterWizard*)wizard())->context();
	if(tasklet)
	{
		const QList<ThreeDSContext*> listObjects = tasklet->objects();
		for(kint i=0; i<listObjects.size(); i++)
		{
			ThreeDSContext* context = listObjects.at(i);
			_table->setRowCount(_table->rowCount()+1);
			_table->setCellWidget(i, 0, new QCheckBox);
			_table->setCellWidget(i, 1, new QLabel(context->name));
			QString type;
			switch(context->type)
			{
			case NAMED_OBJECT:
				type = tr("Triangles mesh");
				break;
			default:
				break;
			}
			_table->setCellWidget(i, 2, new QLabel(type));
		}
	}
	_table->resizeColumnsToContents();
	_table->resizeRowsToContents();
	_checkbox->setCheckState(Qt::Checked);
}

bool ObjectsChoicesPage::validatePage()
{
	// Delete all objects that won't be imported
	ThreeDSSearchingTasklet* tasklet = (ThreeDSSearchingTasklet*)((FileImporterWizard*)wizard())->context();
	if(tasklet)
	{
		QList<ThreeDSContext*> listObjects = tasklet->objects();
		for(kint i=_table->rowCount()-1; i>=0; i--)
		{
			QCheckBox* checkbox = (QCheckBox*)_table->cellWidget(i, 0);
			if(checkbox->checkState() == Qt::Unchecked)
			{
				ThreeDSContext* context = listObjects.at(i);
				listObjects.removeAt(i);
				if(context)
				{
					delete context;
				}
			}
		}
	}
	return true;
}

int ObjectsChoicesPage::nextId() const
{
	return ThreeDSSerializer::LoadObjectsProgressPageId;
}

void ObjectsChoicesPage::uncheckAll()
{
	for(kint i=0; i<_table->rowCount(); i++)
	{
		QCheckBox* checkbox = (QCheckBox*)_table->cellWidget(i, 0);
		checkbox->setCheckState(Qt::Unchecked);
	}
}

void ObjectsChoicesPage::checkAll()
{
	for(kint i=0; i<_table->rowCount(); i++)
	{
		QCheckBox* checkbox = (QCheckBox*)_table->cellWidget(i, 0);
		checkbox->setCheckState(Qt::Checked);
	}
}

void ObjectsChoicesPage::selectionChanged(int state)
{
	switch(state)
	{
	case Qt::Checked:
		checkAll();
		break;
	case Qt::Unchecked:
		uncheckAll();
		break;
	default:
		break;
	}
}



LoadObjectsProgressPage::LoadObjectsProgressPage()
:	QWizardPage(),
 	_tasklet(K_NULL)
{
	setTitle(tr("Importing objects"));

	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(_progress = new QProgressBar);
	vLayout->addSpacing(10);
	vLayout->addWidget(_text = new QTextEdit);
	vLayout->addStretch();

	_text->setReadOnly(true);

	setLayout(vLayout);
}

LoadObjectsProgressPage::~LoadObjectsProgressPage()
{
	if(_tasklet)
	{
		delete _tasklet;
	}
}

void LoadObjectsProgressPage::initializePage()
{
	// Disable cancel button for this step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(false);
	}

	// Check if a searching tasklet is still created
	ThreeDSSearchingTasklet* searchingTasklet = (ThreeDSSearchingTasklet*)((FileImporterWizard*)wizard())->context();
	if(searchingTasklet)
	{
		// Check if we have some objects to import
		const QList<ThreeDSContext*> listObjects = searchingTasklet->objects();
		if(listObjects.size() > 0)
		{
			// Run import tasklet
			_ended = Tasklet::NotStarted;
			K_ASSERT( PantinEngine::ActiveManager()->rootInstance()->fastInherits<Project>() ) // It has to be a Project !
			_tasklet = new ThreeDSImportTasklet(((FileImporterWizard*)wizard())->file(), searchingTasklet);
			connect(_tasklet, SIGNAL(ended(kint)), SLOT(ended(kint)));
			connect(_tasklet, SIGNAL(progress(const QString&)), SLOT(progress(const QString&)));
			connect(_tasklet, SIGNAL(progress(kuint64,kuint64)), SLOT(progress(kuint64,kuint64)));
			KoreEngine::RunTasklet(_tasklet, TaskletRunner::Asynchronous);
		}
		else
		{
			_ended = Tasklet::Completed;
		}
	}
	else
	{
		_ended = Tasklet::Failed;
	}
	emit completeChanged();
}

bool LoadObjectsProgressPage::validatePage()
{
	// Re-Enable cancel button for the next step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(true);
	}
	return true;
}

bool LoadObjectsProgressPage::isComplete() const
{
	return _ended;
}

int LoadObjectsProgressPage::nextId() const
{
	if(_ended == Tasklet::Completed)
	{
		return FileImporterWizard::SuccessPageId;
	}
	return FileImporterWizard::ErrorPageId;
}

void LoadObjectsProgressPage::ended(kint status)
{
	_ended = status;

	// Add all data to the project
	Project* project = static_cast<Project*>(PantinEngine::ActiveManager()->rootInstance());
	QFileInfo info(((FileImporterWizard*)wizard())->file()->fileName());

	// Create a model within the project
	Nigel::project::Model* model = project->createModel(info.baseName());
	model->blockName(info.baseName());

	QList<Geometry*> geometries = _tasklet->geometries();
	for(kint i=0; i<geometries.size(); i++)
	{
		model->geometries()->addBlock(geometries.at(i));
	}

	QList<Node*> sceneNodes = _tasklet->sceneNodes();
	Node* rootNode = model->scene()->visualSceneInstance()->instantiatedElement()->node(0);
	for(kint i=0; i<sceneNodes.size(); i++)
	{
		rootNode->addBlock(sceneNodes.at(i));
	}

	// Save the model to disk
	model->manager()->saveInstance(model);

	// Add a reference to the project
	ProjectAddModel* add = new ProjectAddModel(project, model);

	QUndoStack* stack = PantinEngine::InstanceUndoStack(project);
	stack->setActive(true);
	stack->push(add);

	emit completeChanged();
}

void LoadObjectsProgressPage::progress(const QString& message)
{
	QString text = _text->toPlainText();
	if(text.isEmpty())
	{
		_text->setPlainText(message);
	}
	else
	{
		_text->setPlainText(text + QString("\n") + message);
	}
}

void LoadObjectsProgressPage::progress(kuint64 progress, kuint64 total)
{
	if(progress != total)
	{
		_progress->setRange(0, RANGE_MAX);
		double ratio = (kdouble)progress / (kdouble)total;
		_progress->setValue((int)( ratio * RANGE_MAX ));
	}
	else
	{
		_progress->setValue(RANGE_MAX);
	}
}
