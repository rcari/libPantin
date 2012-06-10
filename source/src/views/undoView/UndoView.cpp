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

#include <views/undoView/UndoView.hpp>
using namespace Pantin::views::undoView;

#include <common/LibraryFilterProxyModel.hpp>
#include <common/LibraryListModel.hpp>
using namespace Gooey::common;
using namespace Gooey::layout;

#include <PantinEngine.hpp>
using namespace Pantin;

#include <KoreApplication.hpp>
using namespace Kore::data;
using namespace Kore;

#define K_BLOCK_TYPE Pantin::views::undoView::UndoView
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON("pantin/images/icons/undoView.png")
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_METHOD( UndoView::ViewProperties )
K_BLOCK_END

#include <GooeyEngine.hpp>
G_VIEW_I( Pantin::views::undoView::UndoView )

#include <QtGui/QUndoView>

UndoView::UndoView()
:	View(new QUndoView)
{
	blockName(tr("Undo View"));

	viewWidget<QUndoView>()->setEmptyLabel(tr("No changes"));

	_projectCombo = new QComboBox;
	_projectCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	connect(_projectCombo, SIGNAL(currentIndexChanged(int)), SLOT(projectComboIndexChanged(int)));

	LibraryListModel* model = new LibraryListModel(KoreApplication::Instance()->dataLibrary(), _projectCombo); // For proper deletion by Qt
	LibraryFilterProxyModel* proxy = new LibraryFilterProxyModel(model); // For proper deletion by Qt
	proxy->setSourceModel(model);
	proxy->addClassType<PantinInstancesManager>();

	privateBar()->addWidget(_projectCombo);
	privateBar()->polish();

	_projectCombo->setModel(proxy);
}

kbool UndoView::activeBlock(Block*)
{
	// This view does not accept blocks.
	return false;
}

void UndoView::projectComboIndexChanged(int index)
{
	LibraryFilterProxyModel* proxy = static_cast<LibraryFilterProxyModel*>(_projectCombo->model());
	QModelIndex proxyIndex = proxy->index(index, 0);
	QModelIndex originalIndex = proxy->mapToSource(proxyIndex);

	if(!originalIndex.isValid())
	{
		return;
	}

	Block* block = static_cast<LibraryListModel*>(proxy->sourceModel())->block(originalIndex);
	K_ASSERT( block->fastInherits<PantinInstancesManager>() )
	viewWidget<QUndoView>()->setGroup(static_cast<PantinInstancesManager*>(block)->undoGroup());
}

QVariant UndoView::ViewProperties(kint property)
{
	switch(property)
	{
	case Block::BlockTypeName:
		return tr("Undo View");
	case View::ViewDescription:
		return tr("View for the project's undo stacks.");
	default:
		return QVariant();
	}
}

