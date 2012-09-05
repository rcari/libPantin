/*
 * 	Copyright (c) 2010-2011, Romuald CARI
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
 *	DISCLAIMED. IN NO EVENT SHALL Romuald CARI BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <dialogs/NewProject.hpp>
using namespace Pantin::dialogs;
using namespace Nigel::project;

#include <PantinInstancesManager.hpp>
using namespace Pantin;

#include <KoreApplication.hpp>
#include <KoreEngine.hpp>
using namespace Kore;

#include <QtGui/QtGui>

NewProject::NewProject(QWidget* parent)
:	QDialog(parent)
{
	setWindowTitle(tr("New Project"));

	QVBoxLayout* vLayout = new QVBoxLayout;
	QFormLayout* fLayout = new QFormLayout;
	QPushButton* button;

	fLayout->addRow(tr("Name:"), _editName = new QLineEdit);
	_editName->setText(tr("New Project"));

	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->addWidget(_editPath = new QLineEdit);
	_editPath->setText(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
	hLayout->addWidget(button = new QPushButton);
	button->setIcon(QIcon(":/gooey/images/icons/global.folder.open.png"));
	connect(button, SIGNAL(clicked(bool)), SLOT(pickFolder()));

	fLayout->addRow(tr("Location:"), hLayout);

	vLayout->addLayout(fLayout);
	vLayout->addStretch();

	hLayout = new QHBoxLayout;
	hLayout->addStretch();
	hLayout->addWidget(_buttonCreate = new QPushButton(tr("Create project")));
	connect(_buttonCreate, SIGNAL(clicked(bool)), SLOT(create()));

	button = new QPushButton(tr("Cancel"));
	connect(button, SIGNAL(clicked(bool)), SLOT(reject()));
	hLayout->addWidget(button);

	vLayout->addLayout(hLayout);

	setLayout(vLayout);
}

bool NewProject::createNewProject()
{
	return exec() == QDialog::Accepted;
}

void NewProject::pickFolder()
{
	QString path = QFileDialog::getExistingDirectory(
			this,
			tr("Pick the Project location"),
			_editPath->text(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog
		);
	_editPath->setText(path);
}

void NewProject::create()
{
	QDir dir(_editPath->text());
	if(dir.cd(_editName->text()))
	{
		KoreEngine::Error(
				tr("New Project Error !"),
				tr("The project folder:<br/><b>%1</b><br/>already exists !").arg(dir.absolutePath())
			);
		return;
	}

	if(!dir.mkpath(_editName->text()))
	{
		KoreEngine::Error(
				tr("New Project Error !"),
				tr("Could not create project folder:<br/><b>%1</b>").arg(dir.absoluteFilePath(_editName->text()))
			);
		return;
	}

	// Go to the created project directory.
	dir.cd(_editName->text());

	// Create the project.
	Project* project = Project::CreateNewProject(dir.absoluteFilePath(_editName->text()));
	if(!project)
	{
		return;
	}

	// Set the project name
	project->blockName(_editName->text());

	// Create an instances manager
	PantinInstancesManager* manager = K_BLOCK_CREATE_INSTANCE( PantinInstancesManager );

	// Add the project to the manager
	manager->setRootInstance(project, true);

	// Add the manager to the data library of Kore.
	KoreApplication::Instance()->dataLibrary()->addBlock(manager);

	accept();
}
