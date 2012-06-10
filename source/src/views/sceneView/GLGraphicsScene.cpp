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

#include <GL/glew.h>

#include <views/sceneView/GLGraphicsScene.hpp>
#include <views/sceneView/GLPainterControlItems.hpp>
#include <views/sceneView/GLView.hpp>
#include <views/sceneView/GLViewport.hpp>
#include <views/sceneView/SceneView.hpp>
using namespace Pantin::views::sceneView;

#include <QtGui/QGraphicsView>
#include <QtGui/QPainter>

#include <QtOpenGL/QGLWidget>

GLGraphicsScene::GLGraphicsScene(SceneView* sceneView)
:	IRendererObject(sceneView->renderer()),
 	_sceneView(sceneView),
 	_rootWidget(new QGraphicsWidget),
 	_rootLayout(new QGraphicsGridLayout)
{
	// To filter viewport events !
	addItem(_sceneView);

	// Drawing control !
	_begin = new GLPainterControlBegin;
	addItem(_begin);
	_end = new GLPainterControlEnd;
	addItem(_end);

	// Root widget for viewports' layout !
	_rootWidget->setZValue(GLVIEW_GL_VIEWPORTS); // To be surrounded by the control begin and end !
	addItem(_rootWidget);
	_rootLayout->setContentsMargins(0.0, 0.0, 0.0, 0.0);
	_rootLayout->setSpacing(0.5);
	_rootWidget->setFlag(QGraphicsItem::ItemHasNoContents, true); // No need to draw this widget (purely organizational)
	_rootWidget->setLayout(_rootLayout);
	// Install this scene on the view !
	sceneView->view()->setScene(this);
}

void GLGraphicsScene::addViewport(GLViewport* vp, kint row, kint col, kint rowSpan, kint colSpan)
{
	// Add the viewport to the root layout
	_rootLayout->addItem(vp, row, col, rowSpan, colSpan);
	// Add its overlay to the scene
	addItem(vp->overlay());
	// Install the global event filter on the viewport.
	vp->installSceneEventFilter(_sceneView);
}

void GLGraphicsScene::viewGeometryChanged(const QRectF& rect)
{
	// Update the root widget geometry
	_rootWidget->setGeometry(rect);
	// Ensure that the begin/end GL commands are always visible !
	// Otherwise their draw functions don't get called and cause problems !
	_begin->setPos(rect.center());
	_end->setPos(rect.center());
	// Update the scene rect to avoid scrollbars !
	setSceneRect(rect.adjusted(2.0,2.0,-3.0,-3.0));
}

void GLGraphicsScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	// Retrieve the background color.
	QColor bgColor = painter->background().color();
	glClearColor(bgColor.redF(), bgColor.greenF(), bgColor.blueF(), bgColor.alphaF());

	// Clear the background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
