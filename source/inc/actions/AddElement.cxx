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

#include <COLLADA/Element.hpp>
#include <actions/AddElement.hpp>
#include <commands/AddBlock.hpp>
#include <PantinEngine.hpp>

template<typename T>
Pantin::actions::AddElement<T>::AddElement(QObject* parent, const QString& name, const QStringList& categories)
:	BlockAction(parent), _categories(categories)
{
	setText(name);
}

template<typename T>
QStringList Pantin::actions::AddElement<T>::categories() const
{
	return _categories;
}

template<typename T>
kbool Pantin::actions::AddElement<T>::prepareForBlock(Kore::data::Block* block)
{
	// Only enable if the model has some geometries !
	K_ASSERT( block->fastInherits<Nigel::COLLADA::Element>() ) // It has to be a Node !

	Nigel::COLLADA::Element* element = static_cast<Nigel::COLLADA::Element*>(block);

	if(!element->instance())
	{
		return false;
	}

	return true;
}

template<typename T>
void Pantin::actions::AddElement<T>::performAction(Kore::data::Block* block) const
{
	K_ASSERT( block->fastInherits<Nigel::COLLADA::Element>() ) // It has to be a Node !

	Nigel::COLLADA::Element* element = static_cast<Nigel::COLLADA::Element*>(block);

	if(!element->instance())
	{
		return;
	}

	QUndoStack* undoStack = Pantin::PantinEngine::InstanceUndoStack(element->instance());

	K_ASSERT( undoStack )

	Kore::data::Block* newElement = T::StaticMetaBlock()->createBlock();

	K_ASSERT( newElement )

	Gooey::commands::AddBlock* add = new Gooey::commands::AddBlock(element, newElement);
	add->setText(
			tr("Add %1 to %2")
			.arg(newElement->metaBlock()->blockProperty(Kore::data::Block::BlockTypeName).toString())
			.arg(element->blockName())
		);

	undoStack->setActive(true);
	undoStack->push(add);
}
