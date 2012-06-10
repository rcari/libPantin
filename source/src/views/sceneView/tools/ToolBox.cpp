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

#include <views/sceneView/tools/ToolBox.hpp>
#include <views/sceneView/SceneViewEngine.hpp>
using namespace Pantin::views::sceneView;

#include <widgets/GroupForm.hpp>
using namespace Gooey::widgets;

#define K_BLOCK_TYPE Pantin::views::sceneView::ToolBox
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON("pantin/images/icons/sceneView-toolBox.png")
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_METHOD( Pantin::views::sceneView::ToolBox::ViewProperties )
K_BLOCK_END

#include <GooeyEngine.hpp>
using namespace Gooey;
G_VIEW_I(Pantin::views::sceneView::ToolBox)

#include <QtGui/QVBoxLayout>

ToolBox::ToolBox()
:	View(new QWidget)
{
	blockName(tr("3D Sceneview Toolbox"));

	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->setMargin(0);
	vLayout->setSpacing(1);

	_form = new GroupForm;
	vLayout->addWidget(_form);

	_toolPanelArea = new QScrollArea;
	vLayout->addWidget(_toolPanelArea);

	// Set the layout.
	viewWidget<QWidget>()->setLayout(vLayout);
}

void ToolBox::createToolBox()
{

}

QVariant ToolBox::ViewProperties(kint property)
{
	switch(property)
	{
	case Block::BlockTypeName:
		return tr("3D Tool Box");
	case View::ViewDescription:
		return tr("Tool box for 3D scene views.");
	default:
		return QVariant();
	}
}
