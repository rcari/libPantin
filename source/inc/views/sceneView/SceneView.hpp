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

#include <layout/View.hpp>

#include <QtCore/QList>

#include <QtGui/QAction>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsItem>
#include <QtGui/QMenu>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>

#include <rendering/Renderer.hpp>

#include <wrappers/scene/WVisualScene.hpp>

namespace Pantin { namespace views { namespace sceneView {

class GLGraphicsScene;
class GLView;
class GLViewport;
class SceneView;

class PantinExport SceneViewEventHandler
{
public:
	virtual ~SceneViewEventHandler() {}
	virtual kbool sceneViewEvent(GLViewport* vp, QEvent* event) = K_NULL;
};

class PantinExport SidePanelsHolder : public QFrame
{
	Q_OBJECT
};

class PantinExport SceneView
:	public Gooey::layout::View,
	private QGraphicsItem // To filter graphics events.
{
	Q_OBJECT

	K_BLOCK

	G_VIEW

	friend class GLGraphicsScene;

private:
	SceneView();

public:
	virtual kbool isViewForBlock(Block* block);
	virtual kbool activeBlock(Block* b);

	inline GLGraphicsScene* scene() { return _scene; }
	inline GLView* view() { return _view; }
	inline GLr::wrappers::scene::WVisualScene* glrScene() { return _glWrapper; }

	void installEventHandler(SceneViewEventHandler* handler);

	GLr::rendering::Renderer* renderer();

private slots:
	void sceneWasRemoved();

private:
	void enableRendering();
	void disableRendering();

	virtual QRectF boundingRect() const { return QRect(); }
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	virtual bool sceneEventFilter(QGraphicsItem* watched, QEvent* event);

public:
	static QVariant ViewProperties(kint property);

private:
	GLGraphicsScene*		_scene;
	GLView*					_view;
	SceneViewEventHandler*	_handler;

	Nigel::scene::VisualScene* _visualScene;
	GLr::wrappers::scene::WVisualScene* _glWrapper;
};

}}}
