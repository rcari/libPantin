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

#include <actions/RemoveElement.hpp>
using namespace Pantin::actions;

using namespace Kore::data;

#include <COLLADA/Element.hpp>
using namespace Nigel::COLLADA;

#include <project/Model.hpp>
#include <project/Project.hpp>
using namespace Nigel::project;

#include <GooeyEngine.hpp>
#include <windows/MainWindow.hpp>
using namespace Gooey;

#include <commands/RemoveBlock.hpp>
using namespace Gooey::commands;

#include <PantinEngine.hpp>
using namespace Pantin;


G_STATIC_BLOCK_ACTION_I( Pantin::actions::RemoveElement )

RemoveElement::RemoveElement(QObject* parent)
:	BlockAction(parent)
{
	setText(tr("Remove"));
	registerWithMetaBlock(Element::StaticMetaBlock());
}

QStringList RemoveElement::categories() const
{
	return QStringList();
}

kbool RemoveElement::prepareForBlock(Block* block)
{
	K_ASSERT( block->fastInherits<Element>() ) // It has to be an Element !

	Element* element = static_cast<Element*>(block);

	return ( element->instance() != K_NULL ) && !element->checkFlag(Block::SystemOwned);
}

void RemoveElement::performAction(Block* block) const
{
	K_ASSERT( block->fastInherits<Element>() ) // It has to be an Element !

	Element* element = static_cast<Element*>(block);

	QUndoStack* undoStack = PantinEngine::InstanceUndoStack(element->instance());

	K_ASSERT( undoStack )

	RemoveBlock* remove = new RemoveBlock(element);
	remove->setText(
			tr("Removed %1 from %2")
			.arg(element->blockName())
			.arg(element->parentElement()->toElement()->blockName())
		);
	undoStack->setActive(true);
	undoStack->push(remove);
}

