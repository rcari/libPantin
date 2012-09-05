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

#include <GL/glew.h>

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "GLGraphicsScene.hpp"
#include "GLView.hpp"

namespace Pantin { namespace views { namespace sceneView {

class PantinExport GLPainterControlBegin : public QGraphicsItem
{
public:
	GLPainterControlBegin()
	{
		// This will be drawn first
		setZValue(GLVIEW_GL_BEGIN);
	}

	virtual QRectF boundingRect() const
	{
		return QRectF();
	}

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
	{
		painter->beginNativePainting();
	}
};

class PantinExport GLPainterControlEnd : public QGraphicsItem
{
public:
	GLPainterControlEnd()
	{
		// This will be drawn after all the GL viewports (as their parent widget has an index of -2.0).
		setZValue(GLVIEW_GL_END);
	}

	virtual QRectF boundingRect() const
	{
		return QRectF();
	}

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		painter->endNativePainting();
	}
};

}}}
