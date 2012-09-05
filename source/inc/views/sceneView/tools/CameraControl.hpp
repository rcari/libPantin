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

#include "Tool.hpp"
#include "../GLViewport.hpp"

namespace Pantin { namespace views { namespace sceneView {

class CameraControl
:	public Tool,
 	public GLViewportPainter
{
	Q_OBJECT
	K_BLOCK

	enum Keys
	{
		Up = 0,
		Down,
		Left,
		Right,
		MAX_KEY,
	};

protected:
	CameraControl();

	virtual kbool keyPressEvent(GLViewport* vp, QKeyEvent* event);
	virtual kbool keyReleaseEvent(GLViewport* vp, QKeyEvent* event);

	virtual kbool mouseDoubleClickEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool mouseMoveEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool mousePressEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool mouseReleaseEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event);
	virtual kbool wheelEvent(GLViewport* vp, QGraphicsSceneWheelEvent* event);

	virtual void paintViewport(GLViewport*,	QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

private:
	kint			_keyCounter;
	GLViewport*		_activeVp;

	bool			_keys[MAX_KEY];
};

}}}
