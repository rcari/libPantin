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

#include <actions/AddGeometryInstance.hpp>
using namespace Pantin::actions;

using namespace Kore::data;

#include <scene/Node.hpp>
using namespace Nigel::scene;

#include <project/Model.hpp>
using namespace Nigel::project;

#include <geometry/GeometriesLibrary.hpp>
#include <geometry/GeometryInstance.hpp>
using namespace Nigel::geometry;

#include <COLLADA/instantiation/ElementInstance.hpp>
using namespace Nigel::COLLADA::instantiation;

#include <GooeyEngine.hpp>
#include <windows/MainWindow.hpp>
using namespace Gooey;

#include <commands/AddBlock.hpp>
using namespace Gooey::commands;

#include <PantinEngine.hpp>
using namespace Pantin;

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

G_STATIC_BLOCK_ACTION_I( Pantin::actions::AddGeometryInstance )

AddGeometryInstance::AddGeometryInstance(QObject* parent)
:	BlockAction(parent)
{
	setText(tr("Add existing geometry"));
	registerWithMetaBlock(Node::StaticMetaBlock());
}

QStringList AddGeometryInstance::categories() const
{
	return QStringList() << tr("Geometry");
}

kbool AddGeometryInstance::prepareForBlock(Block* block)
{
	// Only enable if the model has some geometries !
	K_ASSERT( block->fastInherits<Node>() ) // It has to be a Node !

	Node* node = static_cast<Node*>(block);

	if(!node->instance())
	{
		return false;
	}

	Model* model = static_cast<Model*>(node->instance());
	GeometriesLibrary* geometriesLib = model->geometries();
	setEnabled( (geometriesLib->geometriesNb() > 0) ? true : false );

	return true;
}

void AddGeometryInstance::performAction(Block* block) const
{
	K_ASSERT( block->fastInherits<Node>() ) // It has to be a Node !

	Node* node = static_cast<Node*>(block);

	if(!node->instance())
	{
		return;
	}

	Model* model = static_cast<Model*>(node->instance());
	GeometriesLibrary* geometriesLib = model->geometries();
	QStringList geometries;
	for(kint i=0; i<geometriesLib->geometriesNb(); i++)
	{
		geometries.append(geometriesLib->geometry(i)->blockName());
	}

	bool ok;
	QString name = QInputDialog::getItem(
			GooeyEngine::MainWindow(),
			tr("Add existing geometry"),
			tr("Please select the existing geometry to add"),
			geometries,
			0,
			false,
			&ok
		);

	if(!ok)
	{
		//user canceled
		return;
	}

	for(kint i=0; i<geometriesLib->geometriesNb(); i++)
	{
		if(geometriesLib->geometry(i)->blockName() == name)
		{
			QUndoStack* undoStack = PantinEngine::InstanceUndoStack(node->instance());

			K_ASSERT( undoStack )

			GeometryInstance* geometryInstance = ElementInstance::CreateElementInstance<GeometryInstance>(geometriesLib->geometry(i));
			AddBlock* add = new AddBlock(node, geometryInstance);
			add->setText(
					tr("Add reference to %1")
					.arg(name)
				);
			undoStack->setActive(true);
			undoStack->push(add);
		}
	}
}
