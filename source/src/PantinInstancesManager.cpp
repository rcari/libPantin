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

#include <PantinInstancesManager.hpp>
#include <PantinEngine.hpp>
using namespace Pantin;

#include <tasklets/LoadInstance.hpp>
#include <tasklets/SaveInstance.hpp>
using namespace Pantin::tasklets;

#include <windows/TaskletProgressDialog.hpp>
using namespace Gooey::windows;

#include <COLLADA/DocumentInstance.hpp>
using namespace Nigel::COLLADA;

#include <GLrEngine.hpp>
using namespace GLr;
using namespace GLr::rendering;

#include <QtCore/QThread>
#include <QtGui/QApplication>

#define K_BLOCK_TYPE Pantin::PantinInstancesManager
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON_DEFAULT
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_DEFAULT
K_BLOCK_END
using namespace Kore;

PantinInstancesManager::PantinInstancesManager()
:	_renderer(GLrEngine::CreateRenderer())
{
	blockName(tr("Pantin Instances Manager"));
	addFlag(Block::SystemOwned);
	// Generate the Runtime ID of this instances manager.
	_runtimeID.sprintf("__pantin_%p", this);

	connect(&_undoGroup, SIGNAL(indexChanged(int)), SLOT(currentStackIndexChanged()));
}

PantinInstancesManager::~PantinInstancesManager()
{
	_renderer->destroy();
}

Renderer* PantinInstancesManager::renderer()
{
	return _renderer;
}

void PantinInstancesManager::library(Library* lib)
{
	if(hasParent())
	{
		PantinEngine::Instance()->unregisterInstancesManager(this);
	}
	InstancesManager::library(lib);
	if(hasParent())
	{
		PantinEngine::Instance()->registerInstancesManager(this);
	}
}

QUndoStack* PantinInstancesManager::instanceUndoStack(const Nigel::COLLADA::DocumentInstance* instance) const
{
	QVariant data = instance->property(qPrintable(_runtimeID));
	return data.isValid()
			?	static_cast<QUndoStack*>(data.value<void*>())
			:	K_NULL;
}

QUndoGroup* PantinInstancesManager::undoGroup()
{
	return &_undoGroup;
}

kbool PantinInstancesManager::isClean() const
{
	QList<QUndoStack*> stacks = _undoGroup.stacks();
	for(kint i = 0; i < stacks.size(); i++)
	{
		if(!stacks.at(i)->isClean())
		{
			// One stack is dirty, the manager is therefore dirty.
			return false;
		}
	}

	return true;
}

void PantinInstancesManager::updateInstanceAssetLastModified(DocumentInstance* instance)
{
	// Update the last modified date.
	instance->asset()->modifiedDate(QDateTime::currentDateTimeUtc());
}

void PantinInstancesManager::registerDocumentInstance(DocumentInstance* instance)
{
	InstancesManager::registerDocumentInstance(instance);

	QUndoStack* undoStack = instanceUndoStack(instance);

	if(!undoStack)
	{
		// No stack was found, create one !
		undoStack = new QUndoStack(instance);
		// Store the undostack !
		instance->setProperty(qPrintable(_runtimeID), qVariantFromValue((void*)undoStack));
	}

	_undoGroup.addStack(undoStack);

	if(!undoStack->isClean())
	{
		emit dirtyInstances(true);
	}
}

void PantinInstancesManager::unregisterDocumentInstance(DocumentInstance* instance)
{
	InstancesManager::unregisterDocumentInstance(instance);
	_undoGroup.removeStack(instanceUndoStack(instance)); // Remove from the undo group.
	// No need to delete the QUndoStack as it is a child of the instance.
	// It will be deleted automatically by Qt if appropriate.
}

DocumentInstance* PantinInstancesManager::loadInstance(const QFileInfo& file)
{
	if(this->thread() != QThread::currentThread())
	{
		return InstancesManager::loadInstance(file);
	}

	LoadInstance load(file.absoluteFilePath());

	TaskletProgressDialog progress(&load, PantinEngine::MainWindow());
	progress.execTasklet();

	if(load.loadedInstance())
	{
		addBlock(load.loadedInstance());
	}

	return load.loadedInstance();
}

void PantinInstancesManager::saveInstance(const DocumentInstance* instance) const
{
	bool result = false;

	if(qApp->thread() == QThread::currentThread()) // We check whether we are currently in the GUI thread, in that case, show the progress dialog.
	{
		SaveInstance save(instance);

		TaskletProgressDialog progress(&save, PantinEngine::MainWindow());

		result = TaskletProgressDialog::Accepted == progress.execTasklet();
	}
	else
	{
		result = DocumentInstance::SaveDocumentInstance(instance);
	}

	if(result)
	{
		// Mark the instance's undo stack to clean !
		QUndoStack* stack = instanceUndoStack(instance);
		stack->setClean();
	}
}

void PantinInstancesManager::currentStackIndexChanged()
{
	if(_undoGroup.activeStack())
	{
		DocumentInstance* instance = static_cast<DocumentInstance*>(_undoGroup.activeStack()->parent());
		// The stack was modified, update the last modified value !
		updateInstanceAssetLastModified(instance);
	}
}

QString PantinInstancesManager::displayName() const
{
	return QString("Project: %1").arg(rootInstance()->blockName());
}
