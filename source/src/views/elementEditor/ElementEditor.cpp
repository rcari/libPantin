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

#include <views/elementEditor/ElementEditor.hpp>
#include <views/elementEditor/ElementModel.hpp>
using namespace Pantin::views::elementEditor;
using namespace Gooey::views;

#include <KoreApplication.hpp>
using namespace Kore::data;

#define K_BLOCK_TYPE Pantin::views::elementEditor::ElementEditor
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON_DEFAULT
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_METHOD( Pantin::views::elementEditor::ElementEditor::ViewProperties )
K_BLOCK_END

#include <GooeyEngine.hpp>
G_VIEW_I( Pantin::views::elementEditor::ElementEditor )

#include <common/LinkBlockToView.hpp>
using namespace Gooey::common;

#include <QtGui/QTreeView>

ElementEditor::ElementEditor()
{
	blockName(tr("Element Editor"));

	// Add a BlockAction on Nigel::COLLADA::Element !
	LinkBlockToView* link = new LinkBlockToView(this);
	link->registerWithMetaBlock(Nigel::COLLADA::Element::StaticMetaBlock());
}

kbool ElementEditor::isViewForBlock(Block* b)
{
	return b->fastInherits<Nigel::COLLADA::Element>();
}

QAbstractItemModel* ElementEditor::createBlockModel(Block* b)
{
	return new ElementModel(static_cast<Nigel::COLLADA::Element*>(b), treeView());
}

QVariant ElementEditor::ViewProperties(kint property)
{
	switch(property)
	{
	case Block::BlockTypeName:
		return tr("Element Editor");
	case View::ViewDescription:
		return tr("Editor for Nigel elements' properties.");
	default:
		return QVariant();
	}
}

bool ElementEditor::dragMoveEvent(QDragMoveEvent* event)
{
	if(event->mimeData()->hasFormat(G_MIME_BLOCK_INTERNAL))
	{
		QByteArray data = event->mimeData()->data(G_MIME_BLOCK_INTERNAL);
		Block* block = K_NULL;
		memcpy(&block, data.data(), sizeof(block));

		event->accept();

		if(!block->fastInherits<Nigel::COLLADA::Element>())
		{
			event->setDropAction(Qt::IgnoreAction);
		}

		return true;
	}
	return View::dragMoveEvent(event);
}

bool ElementEditor::dropEvent(QDropEvent* event)
{
	if(event->mimeData()->hasFormat(G_MIME_BLOCK_INTERNAL))
	{
		event->accept();
		event->acceptProposedAction();

		QByteArray data = event->mimeData()->data(G_MIME_BLOCK_INTERNAL);
		Block* block = K_NULL;
		memcpy(&block, data.data(), sizeof(block));

		if(block->fastInherits<Nigel::COLLADA::Element>())
		{
			activeBlock(block);
		}

		return true;
	}

	return View::dropEvent(event);
}
