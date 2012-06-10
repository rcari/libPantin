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
#include <views/sceneView/GLView.hpp>
#include <views/sceneView/GLViewport.hpp>
#include <views/sceneView/GLViewportCube.hpp>
using namespace Pantin::views::sceneView;

#include <QtGui/QPainter>

#include <QtCore/QtDebug>

GLViewportCube::GLViewportCube(GLViewport* vp)
:	IRendererObject(vp->renderer()),
	_viewport(vp)
{
	//setZValue(GLVIEW_GL_VIEWPORT_CUBE);
}

QRectF GLViewportCube::boundingRect() const
{
	return QRectF(0.0,0.0,96.0,96.0);
}

void GLViewportCube::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0.0, 0.0, -4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	QRect rect = _viewport->glScene()->view()->glRect(sceneBoundingRect());
	QRect vpRect = _viewport->boundingRect().toRect(); // We are only interested in the width/height !

	// Clear viewport's part of the frame buffer (activating Scissor TEST !!).
	glScissor(
			rect.x(),
			rect.y(),
			K_MIN(rect.width(), vpRect.width()),
			K_MIN(rect.height(), vpRect.height())
		);
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Apply view port to OpenGL.
	glViewport(rect.x(), rect.y(), rect.width(), rect.height());

	// Some OpenGL magic !
	//painter->fillRect(boundingRect(), Qt::red);
	glColor3f(1.0f,0.0f,0.0f);
	GLUquadric* quad = gluNewQuadric();
	gluSphere(quad, 1.0, 32, 32);
	gluDeleteQuadric(quad);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

bool GLViewportCube::sceneEvent(QEvent* event)
{
	return QGraphicsItem::sceneEvent(event);
}
