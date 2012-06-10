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

#include <commands/ProjectAddModel.hpp>
using namespace Pantin::commands;
using namespace Gooey::commands;

#include <project/Model.hpp>
#include <project/Project.hpp>
using namespace Nigel::project;

#include <COLLADA/DocumentInstanceReference.hpp>
using namespace Nigel::COLLADA;

ProjectAddModel::ProjectAddModel(Project* project, const QString& modelName, Command* parent)
:	AddBlock(parent)
{
	// First we create the model !
	Model* model = project->createModel(modelName);

	_library = project;
	_block = DocumentInstanceReference::CreateDocumentInstanceReference( project, model );
	setText(QObject::tr("Adding model %1 to project %2").arg(modelName, project->blockName()));
}

ProjectAddModel::ProjectAddModel(Project* project, Model* model, Command* parent)
:	AddBlock(parent)
{
	_library = project;
	_block = DocumentInstanceReference::CreateDocumentInstanceReference( project, model );
	setText(QObject::tr("Adding model %1 to project %2").arg(model->blockName(), project->blockName()));
}

void ProjectAddModel::redoCommand()
{
	AddBlock::redoCommand();
	// Resolve the reference (load the document if necessary).
	static_cast<DocumentInstanceReference*>(_block)->reference();
}

void ProjectAddModel::undoCommand()
{
	DocumentInstanceReference* ref = static_cast<DocumentInstanceReference*>(_block);
	// Destroy the reference and unload !
	ref->reference()->destroy();
	AddBlock::undoCommand();
}
