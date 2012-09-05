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

#include <actions/ProjectAddModel.hpp>
using namespace Pantin::actions;

#include <PantinEngine.hpp>
#include <PantinInstancesManager.hpp>
using namespace Pantin;

#include <commands/ProjectAddModel.hpp>

using namespace Kore::data;

#include <COLLADA/DocumentInstanceReference.hpp>
using namespace Nigel::COLLADA;

#include <project/Model.hpp>
#include <project/Project.hpp>
using namespace Nigel::project;

#include <GooeyEngine.hpp>
#include <windows/MainWindow.hpp>
using namespace Gooey;

#include <commands/AddBlock.hpp>
using namespace Gooey::commands;

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

G_STATIC_BLOCK_ACTION_I( Pantin::actions::ProjectAddModel )

ProjectAddModel::ProjectAddModel(QObject* parent)
:	BlockAction(parent)
{
	setText(tr("Add Model"));
	registerWithMetaBlock(Project::StaticMetaBlock());
}

void ProjectAddModel::performAction(Block* block) const
{
	K_ASSERT( block->fastInherits<Project>() ) // It has to be a Project !

	Project* project = static_cast<Project*>(block);

	bool nameOk;

	QString name = QInputDialog::getText(
			GooeyEngine::MainWindow(),
			tr("Add Model"),
			tr("Please enter a name for the new Model"),
			QLineEdit::Normal,
			tr("New Model"),
			&nameOk
		);

	if(!nameOk)
	{
		// User canceled !
		return;
	}

	if(name.isEmpty())
	{
		QMessageBox::warning(
				GooeyEngine::MainWindow(),
				tr("Add Model"),
				tr("Invalid Model name !")
			);
		return;
	}

	// Create a new model in the project !
	Model* model = project->createModel(name);

	// Retrieve the undo stack for the project.
	QUndoStack* stack = PantinEngine::InstanceUndoStack(project);

	K_ASSERT( stack )

	stack->setActive(true);

	Pantin::commands::ProjectAddModel* add = new Pantin::commands::ProjectAddModel(project, model);

	stack->push(add);
}
