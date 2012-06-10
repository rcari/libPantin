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

#include <Types.hpp>

#include <views/sceneView/GLView.hpp>
#include <views/sceneView/GLViewport.hpp>
#include <views/sceneView/GLGraphicsScene.hpp>
using namespace Pantin::views::sceneView;

using namespace GLr::rendering;

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneWheelEvent>
#include <QtGui/QPainter>

#include <QtCore/QDebug>

GLViewport::GLViewport(GLGraphicsScene* scene)
:	IRendererObject(scene->renderer()),
 	_painter(this),
 	_overlay(new GLViewportOverlay(this)),
 	_overlayPainter(this),
 	_camera(this)
{
	K_ASSERT( renderer() )
	setFocusPolicy(Qt::ClickFocus);
	// For keyboard events !
	setFlag(ItemIsFocusable, true);
}

GLViewportOverlay* GLViewport::overlay()
{
	return _overlay;
}

GLGraphicsScene* GLViewport::glScene()
{
	return static_cast<GLGraphicsScene*>(scene());
}

GLViewportCamera* GLViewport::camera()
{
	return &_camera;
}

void GLViewport::installViewportPainter(GLViewportPainter* painter)
{
	_painter = painter;
	update();
}

void GLViewport::resetViewportPainter()
{
	installViewportPainter(this);
}

void GLViewport::installViewportOverlayPainter(GLViewportOverlayPainter* painter)
{
	_overlayPainter = painter;
	_overlay->update();
}

void GLViewport::resetViewportOverlayPainter()
{
	installViewportOverlayPainter(this);
}

void GLViewport::setupViewport(const QColor& clearColor)
{
	QRect rect = glScene()->view()->glRect(sceneBoundingRect());

	// Retrieve the background color.
	glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());

	// Clear viewport's part of the frame buffer (activating Scissor TEST !!).
	glScissor(rect.x(), rect.y(), rect.width(), rect.height());
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Apply view port to OpenGL.
	GLint viewport[4];
	rect.getRect(viewport, viewport + 1, viewport + 2, viewport + 3);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void GLViewport::cleanupViewport()
{
	glDisable(GL_SCISSOR_TEST);
}

void GLViewport::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	_painter->paintViewport(this, painter, option, widget);
}

void GLViewport::defaultPaintViewport(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* widget)
{
	// Setting up viewport (OpenGL...)
	setupViewport(widget->palette().color(QPalette::AlternateBase));

	GLr::wrappers::scene::WVisualScene* wrapper = static_cast<GLGraphicsScene*>(scene())->sceneView()->glrScene();

	if( wrapper )
	{
		renderer()->renderScene( wrapper, &_camera );
	}

	// Cleanup (OpenGL...)
	cleanupViewport();
}

void GLViewport::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	QGraphicsWidget::resizeEvent(event);
	_overlay->prepareGeometryChange();
	_overlay->setPos(scenePos());
	_overlay->_boundingRect = boundingRect();
	_overlay->update();
}

void GLViewport::paintViewport(GLViewport*, QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget)
{
	defaultPaintViewport(painter, options, widget);
}

void GLViewport::paintViewportOverlay(GLViewportOverlay* overlay, QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	// Nada !
}

GLViewportOverlay::GLViewportOverlay(GLViewport* viewport)
:	_viewport(viewport)
{
	setZValue(GLVIEW_OVERLAY);
}

QRectF GLViewportOverlay::boundingRect() const
{
	return _boundingRect;
}

void GLViewportOverlay::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	_viewport->_overlayPainter->paintViewportOverlay(this, painter, option, widget);
}
