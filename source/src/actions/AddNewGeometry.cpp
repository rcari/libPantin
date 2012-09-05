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

#include <actions/AddNewGeometry.hpp>
using namespace Pantin::actions;

using namespace Kore::data;

using namespace Nigel::scene;

#include <project/Model.hpp>
#include <project/Project.hpp>
using namespace Nigel::project;

#include <geometry/GeometriesLibrary.hpp>
#include <geometry/GeometryInstance.hpp>
using namespace Nigel::geometry;

#include <COLLADA/instantiation/ElementInstance.hpp>
using namespace Nigel::COLLADA::instantiation;

#include <modeling/shape/ShapeManager.hpp>
#include <modeling/shape/Shape.hpp>
#include <modeling/shape/Generator.hpp>
using namespace Nigel::modeling::shape;

#include <GooeyEngine.hpp>
#include <windows/MainWindow.hpp>
using namespace Gooey;

#include <commands/AddBlock.hpp>
using namespace Gooey::commands;

#include <dialogs/GenerateShape.hpp>
using namespace Pantin::dialogs;

#include <PantinEngine.hpp>
using namespace Pantin;

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

G_STATIC_BLOCK_ACTION_I( Pantin::actions::AddNewGeometry )

AddNewGeometry::AddNewGeometry(QObject* parent)
:	BlockAction(parent)
{
	setText(tr("Add new geometry"));
	registerWithMetaBlock(Node::StaticMetaBlock());
}

QStringList AddNewGeometry::categories() const
{
	return QStringList() << tr("Geometry");
}

kbool AddNewGeometry::prepareForBlock(Block* block)
{
	K_ASSERT( block->fastInherits<Node>() ) // It has to be a Node !

	Node* node = static_cast<Node*>(block);

	if(!node->instance())
	{
		return false;
	}

	if(!node->instance()->fastInherits<Model>())
	{
		return false;
	}

	QMenu* menu = new QMenu();
	const ShapeManager* manager = ShapeManager::Instance();
	for(kint i=0; i<manager->size(); i++)
	{
		const Shape* shape = manager->constAt(i);
		for(kint j=0; j<shape->size(); j++)
		{
			const Generator* generator = shape->constAt(j);
			AddShape* action = new AddShape(menu, generator);
			menu->addAction(action);
		}
	}
	setMenu(menu);

	return true;
}

void AddNewGeometry::performAction(Block* block) const
{
	Q_UNUSED(block);
}


AddShape::AddShape(QObject* parent, const Generator* generator)
:	BlockAction(parent),
 	_generator(generator)
{
	setText(
			QString("%1 (%2)")
			.arg(generator->shape()->blockName(), generator->geometryMetaBlock()->blockName())
		);
}

void AddShape::performAction(Block* block) const
{
	Node* node = static_cast<Node*>(block);

	GenerateShape dialog(_generator, PantinEngine::MainWindow());

	if(dialog.createShape())
	{
		K_ASSERT( node->instance()->fastInherits<Model>() ); // The instance HAS to be a model in Nigel. Only models can contain geometry.

		Model* model = static_cast<Model*>(node->instance());

		QUndoStack* undoStack = PantinEngine::InstanceUndoStack( model );
		undoStack->setActive(true);

		undoStack->beginMacro(
				tr("Adding new %1 to node %2")
				.arg(_generator->shape()->blockName(), node->blockName())
			);

		AddBlock* addGeometry = new AddBlock(model->geometries(), dialog.geometry());
		undoStack->push(addGeometry);

		AddBlock* addReference = new AddBlock(node, dialog.geometryInstance());
		undoStack->push(addReference);

		undoStack->endMacro();
	}
}
