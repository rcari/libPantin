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
 *	DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <PantinEngine.hpp>
#include <PantinApplication.hpp>
#include <PantinInstancesManager.hpp>
using namespace Pantin;

#include <dialogs/NewProject.hpp>
#include <dialogs/OpenProject.hpp>
using namespace Pantin::dialogs;

#include <tasklets/SaveAllInstances.hpp>
#include <tasklets/LoadInstance.hpp>
using namespace Pantin::tasklets;

#include <GooeyEngine.hpp>
#include <GooeyApplication.hpp>
using namespace Gooey;

#include <windows/MainMenu.hpp>
#include <windows/TaskletProgressDialog.hpp>
using namespace Gooey::windows;

#include <KoreEngine.hpp>
#include <KoreApplication.hpp>
using namespace Kore;

#include <GLrApplication.hpp>
using namespace GLr::rendering;

#include <NigelApplication.hpp>
#include <project/ProjectConstants.hpp>
using namespace Nigel::project;
using namespace Nigel::COLLADA;

#include <serialization/Serializer.hpp>
#include <serialization/FileImporterWizard.hpp>
using namespace Pantin::serialization;

#include <QtCore/QtDebug>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#define LAST_OPENED		"LAST_OPENED"

PantinEngine::PantinEngine()
:	_mainWindow(K_NULL),
	_manager(K_NULL)
{
	_Instance = this;
	_serializers.blockName(tr("Serializers Library"));
	addBlock(&_serializers);

	addFlag(Block::System);

	blockName("Pantin Engine");

	// Quit event from the Gooey engine.
	connect(GooeyEngine::Instance(), SIGNAL(quitRequested()), SLOT(quitRequest()));

	// Kore error events for user to see.
	connect(KoreEngine::Instance(), SIGNAL(error(QString, QString)), SLOT(error(QString, QString)));
}

MainWindow* PantinEngine::MainWindow()
{
	return Instance()->_mainWindow;
}

kbool PantinEngine::HasActiveProject()
{
	return Instance()->_manager != K_NULL;
}

PantinInstancesManager* PantinEngine::ActiveManager()
{
	return Instance()->_manager;
}

Renderer* PantinEngine::InstanceRenderer(DocumentInstance* instance)
{
	K_ASSERT( instance->manager()->fastInherits<PantinInstancesManager>() )
	return static_cast<PantinInstancesManager*>(instance->manager())->renderer();
}

QUndoStack* PantinEngine::InstanceUndoStack(DocumentInstance* instance)
{
	K_ASSERT( instance && instance->manager() )
	K_ASSERT( instance->manager()->fastInherits<PantinInstancesManager>() )
	return static_cast<PantinInstancesManager*>(instance->manager())->instanceUndoStack(instance);
}

kbool PantinEngine::RegisterSerializer(Pantin::serialization::Serializer* serializer)
{
	//qDebug("PantinEngine / Registering serializer %s", qPrintable(serializer->objectClassName()));

	// Change serializer's name to be sure that another serializer has the same
	kint nb = K_NULL;
	QString name = serializer->blockName();
	while(existsSerializerWithName(name))
	{
		nb++;
		name = serializer->blockName();
		name.append(" - ");
		name.append(nb);
	}
	serializer->blockName(name);

	// Register in the global factory store.
	Instance()->_serializers.addBlock(serializer);

	return true;
}

const Kore::data::LibraryT<Pantin::serialization::Serializer>& PantinEngine::Serializers()
{
	return Instance()->_serializers;
}

kbool PantinEngine::existsSerializerWithName(const QString& name)
{
	for(kint i=K_NULL; i<Instance()->_serializers.size(); i++)
	{
		if(name.compare(Instance()->_serializers.at(i)->blockName()) == K_NULL)
		{
			return true;
		}
	}
	return false;
}

void PantinEngine::LoadProject(const QFileInfo& fileInfo, QWidget* parent = K_NULL)
{
	qDebug() << "Pantin / Loading project @" << fileInfo.absoluteFilePath();

	LoadInstance loadTasklet(fileInfo.absoluteFilePath());

	// GUI remains responsive during serialization operation.
	TaskletProgressDialog dialog(&loadTasklet, parent);
	dialog.setLabelText(tr("Loading project %1").arg(fileInfo.fileName()));
	dialog.execTasklet();

	if(!loadTasklet.loadedInstance())
	{
		qDebug() << "Pantin / Failed to load project";
		return;
	}

	qDebug() << "Pantin / Successfully loaded project";

	// Create an instances manager
	PantinInstancesManager* manager = K_BLOCK_CREATE_INSTANCE( PantinInstancesManager );
	// Set the loaded instance as the root instance of the manager.
	manager->setRootInstance(loadTasklet.loadedInstance());
	// Add the manager to the data library of Kore.
	KoreApplication::Instance()->dataLibrary()->addBlock(manager);
}

void PantinEngine::createMainWindow()
{
	_mainWindow = new Gooey::windows::MainWindow;
	_mainWindow->setWindowIcon(QIcon(":/pantin-studio/images/icons/nigel.png"));

	MainMenu* mainMenu = _mainWindow->mainMenu();

	QAction* prevAction = mainMenu->quitAction();

	QAction* action;

	// The menu is filled in reverse !

	// Separator
	prevAction = mainMenu->fileMenu()->insertSeparator(prevAction);

	// Save
	action = mainMenu->fileMenu()->addAction(tr("Save Project"), this, SLOT(save()), QKeySequence::Save);
	connect(this, SIGNAL(saveEnabled(bool)), action, SLOT(setEnabled(bool)));
	mainMenu->fileMenu()->insertAction(prevAction, action);
	action->setEnabled(false);
	prevAction = action;

	// Open
	action = mainMenu->fileMenu()->addAction(tr("Open Project"), this, SLOT(open()), QKeySequence::Open);
	mainMenu->fileMenu()->insertAction(prevAction, action);
	prevAction = action;

	// New
	action = mainMenu->fileMenu()->addAction(tr("New Project"), this, SLOT(create()), QKeySequence::New);
	mainMenu->fileMenu()->insertAction(prevAction, action);
	prevAction = action;

	prevAction = mainMenu->aboutQtAction();

	// About
	action = mainMenu->aboutMenu()->addAction(tr("About"), this, SLOT(about()));
	mainMenu->aboutMenu()->insertAction(prevAction, action);

	// Project menu
	_projectMenu = new QMenu(tr("Project"), mainMenu);
	mainMenu->insertMenu(mainMenu->windowMenu()->menuAction(), _projectMenu);
	_projectMenu->setEnabled(false);

	//Import menu
	action = _projectMenu->addAction(tr("Import"), this, SLOT(import()));

	//Export menu
	action = _projectMenu->addAction(tr("Export"));
}

void PantinEngine::registerInstancesManager(PantinInstancesManager* manager)
{
	K_ASSERT(_manager == K_NULL)
	_manager = manager;
	// To get the stacks signals.
	connect(_manager->undoGroup(), SIGNAL(cleanChanged(bool)), SLOT(cleanChanged(bool)));

	_projectMenu->setEnabled(true);

	emit projectLoaded();
}

void PantinEngine::unregisterInstancesManager(PantinInstancesManager* manager)
{
	emit projectUnloaded();

	_projectMenu->setEnabled(false);

	K_ASSERT( _manager == manager )
	// Disconnect from the undo group signals.
	_manager->undoGroup()->disconnect(this);
	_manager = K_NULL;
}

void PantinEngine::unloadProject()
{
	K_ASSERT( _manager )
	_manager->destroy();
}

kbool PantinEngine::close()
{
	if(!_manager)
	{
		QMessageBox box(_mainWindow);
		box.setWindowTitle(tr("Closing"));
		box.setIcon(QMessageBox::Question);
		box.setText(tr("Are you sure you want to quit Pantin-Studio ?"));

		box.addButton(QMessageBox::Yes);
		box.button(QMessageBox::Yes)->setText(tr("Yes"));
		box.addButton(QMessageBox::No);
		box.button(QMessageBox::No)->setText(tr("No"));

		box.setDefaultButton(QMessageBox::Yes);
		box.setEscapeButton(QMessageBox::No);

		return box.exec() == QMessageBox::Yes;
	}
	else if(_manager->isClean())
	{
		QMessageBox box(_mainWindow);
		box.setWindowTitle(tr("Closing"));
		box.setIcon(QMessageBox::Question);
		box.setText(tr("Are you sure you want to close the active project ?"));

		box.addButton(QMessageBox::Yes);
		box.button(QMessageBox::Yes)->setText(tr("Yes"));
		box.addButton(QMessageBox::No);
		box.button(QMessageBox::No)->setText(tr("No"));

		box.setDefaultButton(QMessageBox::Yes);
		box.setEscapeButton(QMessageBox::No);

		if(box.exec() == QMessageBox::Yes)
		{
			unloadProject();
			return true;
		}

		return false;
	}
	else
	{
		QMessageBox box(_mainWindow);
		box.setWindowTitle(tr("Closing"));
		box.setIcon(QMessageBox::Question);
		box.setText(tr("The current project has unsaved modifications.<br/>What do you want to do ?"));

		box.addButton(QMessageBox::Save);
		box.button(QMessageBox::Save)->setText(tr("Save"));
		box.addButton(QMessageBox::Discard);
		box.button(QMessageBox::Discard)->setText(tr("Discard"));
		box.addButton(QMessageBox::Cancel);
		box.button(QMessageBox::Cancel)->setText(tr("Cancel"));

		box.setDefaultButton(QMessageBox::Save);
		box.setEscapeButton(QMessageBox::Cancel);

		switch(box.exec())
		{
		case QMessageBox::Save:
			if(save())
			{
				unloadProject();
				return true;
			}
			break;
		case QMessageBox::Discard:
			unloadProject();
			return true;
		case QMessageBox::Cancel:
			return false;
		}
	}

	return false;
}

void PantinEngine::loadLastOpenedProject()
{
	QSettings settings;
	settings.beginGroup("pantin");
	settings.beginGroup("project");

	if(settings.contains(LAST_OPENED))
	{
		QFileInfo fileInfo(settings.value(LAST_OPENED).toString());
		if(fileInfo.exists())
		{
			qDebug() << "Pantin / Found last opened project @" << fileInfo.absoluteFilePath();
			PantinEngine::LoadProject(fileInfo, _mainWindow);
		}
	}
}

void PantinEngine::saveLastOpenedProject()
{
	QSettings settings;
	settings.beginGroup("pantin");
	settings.beginGroup("project");
	settings.setValue(LAST_OPENED, _manager->rootInstance()->fileInfo().absoluteFilePath());
}

void PantinEngine::quitRequest()
{
	if(close())
	{
		GooeyEngine::Quit();
	}
}

void PantinEngine::error(QString error, QString details)
{
	QMessageBox::warning(
			_mainWindow,
			tr("An error occured"),
			QString("<b>%1</b><br/><br/>%2").arg(error).arg(details)
		);
}

void PantinEngine::create()
{
	if(_manager && !close())
	{
		return;
	}

	NewProject project(_mainWindow);
	if( project.createNewProject() )
	{
		//save the project as the last opened
		saveLastOpenedProject();
	}
}

void PantinEngine::open()
{
	if(_manager && !close())
	{
		return;
	}

	OpenProject project(_mainWindow);
	if( project.openProject() )
	{
		//save the project as the last opened
		saveLastOpenedProject();
	}
}

kbool PantinEngine::save()
{
	K_ASSERT( _manager )

	// Create a save all tasklet to execute asynchronously
	SaveAllInstances saveAll(_manager);

	// Run the tasklet progress dialog.
	TaskletProgressDialog dialog(&saveAll, _mainWindow);
	return dialog.execTasklet() == QDialog::Accepted;
}

void PantinEngine::about()
{
	QMessageBox::about(
			_mainWindow,
			tr("About Pantin Studio"),
			tr(
					"Pantin will hopefully soon become a powerful 3D animation tool.\n"
					"Nigel is our hero, here at pixelfr0g.\n\n"
					"PantinStudio Version: %1\n"
					"Kore library Version: %2\n"
					"GLr library Version: %3\n"
					"Pantin library Version: %4\n"
					"Gooey library Version: %5\n"
					"Nigel library Version: %6\n\n"
					"Copyright pixelfr0g - 2009/2011"
			)
			.arg(qApp->applicationVersion())
			.arg(Kore::KoreApplication::Version())
			.arg(GLr::GLrApplication::Version())
			.arg(PantinApplication::Version())
			.arg(Gooey::GooeyApplication::Version())
			.arg(Nigel::NigelApplication::Version())
		);

}

void PantinEngine::import()
{
	FileImporterWizard wizard(_mainWindow);
	wizard.exec();
}

void PantinEngine::cleanChanged(bool clean)
{
	K_ASSERT( _manager )
	emit saveEnabled(!clean || !_manager->isClean());
}

PantinEngine* PantinEngine::Instance()
{
	return _Instance ? _Instance : _Instance = new PantinEngine();
}

PantinEngine* PantinEngine::_Instance = K_NULL;
