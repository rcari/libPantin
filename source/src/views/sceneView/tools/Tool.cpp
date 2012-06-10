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

#include <views/sceneView/tools/Tool.hpp>
using namespace Pantin::views::sceneView;

#define K_BLOCK_TYPE Pantin::views::sceneView::Tool
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON("pantin/images/icons/sceneView-tool.png")
	K_BLOCK_VIRTUAL
	K_BLOCK_PROPERTY_DEFAULT
K_BLOCK_END

#include <GooeyEngine.hpp>
using namespace Gooey;

#include <QtGui/QFocusEvent>
#include <QtGui/QGraphicsSceneContextMenuEvent>
#include <QtGui/QGraphicsSceneDragDropEvent>
#include <QtGui/QGraphicsSceneHoverEvent>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsSceneWheelEvent>
#include <QtGui/QKeyEvent>

Tool::Tool()
{
}

kbool Tool::sceneViewEvent(GLViewport* vp, QEvent* event)
{
	switch(event->type())
	{
	case QEvent::FocusIn:
		return focusInEvent(vp, static_cast<QFocusEvent*>(event));
	case QEvent::FocusOut:
		return focusOutEvent(vp, static_cast<QFocusEvent*>(event));
	case QEvent::GraphicsSceneContextMenu:
		return contextMenuEvent(vp, static_cast<QGraphicsSceneContextMenuEvent*>(event));
	case QEvent::GraphicsSceneDragEnter:
		return dragEnterEvent(vp, static_cast<QGraphicsSceneDragDropEvent*>(event));
	case QEvent::GraphicsSceneDragLeave:
		return dragLeaveEvent(vp, static_cast<QGraphicsSceneDragDropEvent*>(event));
	case QEvent::GraphicsSceneDragMove:
		return dragMoveEvent(vp, static_cast<QGraphicsSceneDragDropEvent*>(event));
	case QEvent::GraphicsSceneDrop:
		return dropEvent(vp, static_cast<QGraphicsSceneDragDropEvent*>(event));
	case QEvent::GraphicsSceneHoverEnter:
		return hoverEnterEvent(vp, static_cast<QGraphicsSceneHoverEvent*>(event));
	case QEvent::GraphicsSceneHoverLeave:
		return hoverLeaveEvent(vp, static_cast<QGraphicsSceneHoverEvent*>(event));
	case QEvent::GraphicsSceneHoverMove:
		return hoverMoveEvent(vp, static_cast<QGraphicsSceneHoverEvent*>(event));
	case QEvent::GraphicsSceneMouseDoubleClick:
		return mouseDoubleClickEvent(vp, static_cast<QGraphicsSceneMouseEvent*>(event));
	case QEvent::GraphicsSceneMouseMove:
		return mouseMoveEvent(vp, static_cast<QGraphicsSceneMouseEvent*>(event));
	case QEvent::GraphicsSceneMousePress:
		return mousePressEvent(vp, static_cast<QGraphicsSceneMouseEvent*>(event));
	case QEvent::GraphicsSceneMouseRelease:
		return mouseReleaseEvent(vp, static_cast<QGraphicsSceneMouseEvent*>(event));
	case QEvent::GraphicsSceneWheel:
		return wheelEvent(vp, static_cast<QGraphicsSceneWheelEvent*>(event));
	case QEvent::KeyPress:
		return keyPressEvent(vp, static_cast<QKeyEvent*>(event));
	case QEvent::KeyRelease:
		return keyReleaseEvent(vp, static_cast<QKeyEvent*>(event));
	default:
		return false;
	}
}

void Tool::installTool(SceneView* view)
{
	view->installEventHandler(this);
}

void Tool::uninstallTool(SceneView* view)
{
}

QWidget* Tool::controlsPanel()
{
	return K_NULL;
}

kbool Tool::contextMenuEvent(GLViewport* vp, QGraphicsSceneContextMenuEvent* event)
{
	return false;
}

kbool Tool::dragEnterEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event)
{
	return false;
}

kbool Tool::dragLeaveEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event)
{
	return false;
}

kbool Tool::dragMoveEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event)
{
	return false;
}

kbool Tool::dropEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event)
{
	return false;
}

kbool Tool::focusInEvent(GLViewport* vp, QFocusEvent* event)
{
	return false;
}

kbool Tool::focusOutEvent(GLViewport* vp, QFocusEvent* event)
{
	return false;
}

kbool Tool::hoverEnterEvent(GLViewport* vp, QGraphicsSceneHoverEvent* event)
{
	return false;
}

kbool Tool::hoverLeaveEvent(GLViewport* vp, QGraphicsSceneHoverEvent* event)
{
	return false;
}

kbool Tool::hoverMoveEvent(GLViewport* vp, QGraphicsSceneHoverEvent* event)
{
	return false;
}

kbool Tool::keyPressEvent(GLViewport* vp, QKeyEvent* event)
{
	return false;
}

kbool Tool::keyReleaseEvent(GLViewport* vp, QKeyEvent* event)
{
	return false;
}

kbool Tool::mouseDoubleClickEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return false;
}

kbool Tool::mouseMoveEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return false;
}

kbool Tool::mousePressEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return false;
}

kbool Tool::mouseReleaseEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return false;
}

kbool Tool::wheelEvent(GLViewport* vp, QGraphicsSceneWheelEvent* event)
{
	return false;
}
