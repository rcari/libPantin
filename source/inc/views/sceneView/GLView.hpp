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

#include <rendering/Renderer.hpp>

#include <QtGui/QGraphicsView>
#include <QtCore/QList>

#define GLVIEW_GL_BEGIN				-5.0
#define GLVIEW_GL_VIEWPORTS			-4.0
#define GLVIEW_GL_VIEWPORT_CUBE		-3.0
#define GLVIEW_GL_END				-2.0
#define GLVIEW_OVERLAY				-1.0

namespace Pantin { namespace views { namespace sceneView {

class PantinExport GLView : public QGraphicsView
{
	Q_OBJECT

	friend class GLGraphicsScene;
	friend class GLViewport;

public:
	GLView(GLr::rendering::Renderer* renderer);

	QRect glRect(const QRectF& rect);

	QRectF visibleSceneRect() const;

	virtual int heightForWidth(int w) const { return w; }

	virtual QSize sizeHint() const { return QSize(640,480); }

protected:
	virtual void resizeEvent(QResizeEvent* event);

};

}}}
