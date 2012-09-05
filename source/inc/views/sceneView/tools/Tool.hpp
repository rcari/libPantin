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

#pragma once

#include <PantinExport.hpp>

#include <data/Library.hpp>

#include <QtGui/QGraphicsItem>

#include "../SceneView.hpp"

namespace Pantin { namespace views { namespace sceneView {

class SceneView;

class PantinExport Tool
:	public Kore::data::Library,
 	public SceneViewEventHandler
{
	Q_OBJECT
	K_BLOCK

	friend class SceneView;

public:
	enum {
		ToolCategory = Block::MAX_PROPERTY,
		ToolRanking,
	};

	enum Categories {
		Selection,
		Modeling,
		Rigging,
		Painting,
		Shading,
		// Leaving some space for future categories.
		User = 0x1 << 16,
	};

protected:
	Tool();

public:
	virtual kbool sceneViewEvent(GLViewport* vp, QEvent* event);

	virtual void installTool(SceneView* view);
	virtual void uninstallTool(SceneView* view);

	virtual QWidget* controlsPanel();

protected:
	virtual kbool contextMenuEvent(GLViewport* vp, QGraphicsSceneContextMenuEvent* event);
	virtual kbool dragEnterEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event);
	virtual kbool dragLeaveEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event);
	virtual kbool dragMoveEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event);
	virtual kbool dropEvent(GLViewport* vp, QGraphicsSceneDragDropEvent* event);
	virtual kbool focusInEvent(GLViewport* vp, QFocusEvent* event);
	virtual kbool focusOutEvent(GLViewport* vp, QFocusEvent* event);
	virtual kbool hoverEnterEvent(GLViewport* vp, QGraphicsSceneHoverEvent* event);
	virtual kbool hoverLeaveEvent(GLViewport* vp, QGraphicsSceneHoverEvent* event);
	virtual kbool hoverMoveEvent(GLViewport* vp, QGraphicsSceneHoverEvent* event);
	virtual kbool keyPressEvent(GLViewport* vp, QKeyEvent* event);
	virtual kbool keyReleaseEvent(GLViewport* vp, QKeyEvent* event);
	virtual kbool mouseDoubleClickEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool mouseMoveEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool mousePressEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool mouseReleaseEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool wheelEvent(GLViewport* vp, QGraphicsSceneWheelEvent* event);

};

}}}

#define P_SCENEVIEW_TOOL private: static kbool __P_Registered;
