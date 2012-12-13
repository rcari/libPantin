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

#include <views/UndoView.hpp>
using namespace Pantin::views;

#include <common/LibraryFilterProxyModel.hpp>
#include <common/LibraryListModel.hpp>
#include <common/PackedVBoxLayout.hpp>
using namespace Gooey::common;
using namespace Gooey::windows;

#include <PantinEngine.hpp>
using namespace Pantin;

#include <KoreApplication.hpp>
using namespace Kore;

#include <QtGui/QUndoView>
#include <QtGui/QVBoxLayout>

UndoView::UndoView()
{
	setWindowTitle(tr("Undo View"));

	PackedVBoxLayout* vLayout = new PackedVBoxLayout;

	_projectCombo = new QComboBox;
	_projectCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	connect(_projectCombo, SIGNAL(currentIndexChanged(int)), SLOT(projectComboIndexChanged(int)));
	vLayout->addWidget(_projectCombo, 0);

	LibraryListModel* model = new LibraryListModel(KoreApplication::Instance()->dataLibrary(), _projectCombo); // For proper deletion by Qt
	LibraryFilterProxyModel* proxy = new LibraryFilterProxyModel(model); // For proper deletion by Qt
	proxy->setSourceModel(model);
	proxy->addClassType<PantinInstancesManager>();
	_projectCombo->setModel(proxy);

	_undoView = new QUndoView;
	vLayout->addWidget(_undoView, 1);

	QWidget* w = new QWidget;
	w->setLayout(vLayout);

	setWidget(w);
}

void UndoView::projectComboIndexChanged(int index)
{
	/*LibraryFilterProxyModel* proxy = static_cast<LibraryFilterProxyModel*>(_projectCombo->model());
	QModelIndex proxyIndex = proxy->index(index, 0);
	QModelIndex originalIndex = proxy->mapToSource(proxyIndex);

	if(!originalIndex.isValid())
	{
		return;
	}

	Block* block = static_cast<LibraryListModel*>(proxy->sourceModel())->block(originalIndex);
	K_ASSERT( block->fastInherits<PantinInstancesManager>() )
	viewWidget<QUndoView>()->setGroup(static_cast<PantinInstancesManager*>(block)->undoGroup());*/
}
