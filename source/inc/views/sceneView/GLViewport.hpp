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

#pragma once

#include <PantinExport.hpp>

#include <QtGui/QGraphicsWidget>
#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>

#include <Types.hpp>

#include <rendering/IRendererObject.hpp>

#include "GLViewportCamera.hpp"

namespace Pantin { namespace views { namespace sceneView {

class GLGraphicsScene;
class GLViewport;
class GLViewportEventHandler;
class GLViewportOverlay;

class PantinExport GLViewportPainter
{
public:
	virtual ~GLViewportPainter() {}
	virtual void paintViewport(GLViewport*,	QPainter*, const QStyleOptionGraphicsItem*, QWidget*) = K_NULL;
};

class PantinExport GLViewportOverlayPainter
{
public:
	virtual ~GLViewportOverlayPainter() {}
	virtual void paintViewportOverlay(GLViewportOverlay*, QPainter*, const QStyleOptionGraphicsItem* option, QWidget* widget) = K_NULL;
};

class PantinExport GLViewport
:	public QGraphicsWidget,
 	private GLViewportPainter,
 	private GLViewportOverlayPainter,
 	public GLr::rendering::IRendererObject
{
	friend class GLViewportOverlay;

	Q_OBJECT

public:
	GLViewport(GLGraphicsScene* scene);

	GLViewportOverlay* overlay();

	GLGraphicsScene* glScene();

	GLViewportCamera* camera();

	void installViewportPainter(GLViewportPainter* painter);
	void resetViewportPainter();

	void installViewportOverlayPainter(GLViewportOverlayPainter* painter);
	void resetViewportOverlayPainter();

	void defaultPaintViewport(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* widget);
	void setupViewport(const QColor& clearColor);
	void cleanupViewport();

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

protected:
	virtual void resizeEvent(QGraphicsSceneResizeEvent* event);

private:
	virtual void paintViewport(GLViewport*,	QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	virtual void paintViewportOverlay(GLViewportOverlay*, QPainter*, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
	GLViewportPainter*			_painter;

	GLViewportOverlay*			_overlay;
	GLViewportOverlayPainter*	_overlayPainter;

	GLViewportCamera			_camera;
};

class PantinExport GLViewportOverlay : public QGraphicsItem
{
	friend class GLViewport;

private:
	GLViewportOverlay(GLViewport* viewport);

public:
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
	QRectF		_boundingRect;
	GLViewport*	_viewport;
};

}}}
