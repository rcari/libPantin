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

#include <opengl/GLWidget.hpp>
using namespace GLr::opengl;

#include <views/sceneView/GLGraphicsScene.hpp>
#include <views/sceneView/GLView.hpp>
using namespace Pantin::views::sceneView;

#include <PantinEngine.hpp>
using namespace Pantin;

GLView::GLView(GLr::rendering::Renderer* renderer)
{
	setViewport( new GLWidget(renderer) );
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
	setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
}

QRect GLView::glRect(const QRectF& rect)
{
	QRect viewRect = QRect(
			mapFromScene(rect.topLeft()),
			mapFromScene(rect.bottomRight())
		);

	return QRect(
			viewRect.x(),
			height() - viewRect.height() - viewRect.y(),
			rect.width(),
			rect.height()
		);
}

QRectF GLView::visibleSceneRect() const
{
	return QRectF(
				mapToScene(0,0),
				mapToScene(this->width(), this->height())
			);
}

void GLView::resizeEvent(QResizeEvent* event)
{
	QGraphicsView::resizeEvent(event);
	QRectF rect = QRectF(
			mapToScene(-1,-1),
			mapToScene(viewport()->width()+2, viewport()->height()+2)
		);

	// Inform the scene that the view geometry has changed !
	static_cast<GLGraphicsScene*>(scene())->viewGeometryChanged(rect);
}
