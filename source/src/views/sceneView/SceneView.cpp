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

#include <views/sceneView/GLGraphicsScene.hpp>
#include <views/sceneView/GLView.hpp>
#include <views/sceneView/GLViewport.hpp>
#include <views/sceneView/GLViewportCamera.hpp>
#include <views/sceneView/GLViewportCube.hpp>
#include <views/sceneView/SceneView.hpp>
#include <views/sceneView/tools/CameraControl.hpp>
#include <views/sceneView/tools/CameraTrackball.hpp>
#include <views/sceneView/tools/Tool.hpp>
using namespace Pantin::views::sceneView;

#include <scene/Scene.hpp>
#include <scene/VisualScene.hpp>
#include <scene/VisualSceneInstance.hpp>
using namespace Nigel::scene;

#include <data/MetaBlock.hpp>
using namespace Kore::data;

#include <PantinModule.hpp>

#define K_BLOCK_TYPE Pantin::views::sceneView::SceneView
#include <data/BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON("pantin/images/icons/sceneView.png")
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_METHOD( Pantin::views::sceneView::SceneView::ViewProperties )
K_BLOCK_END

#include <GooeyEngine.hpp>
using namespace Gooey;
G_VIEW_I(Pantin::views::sceneView::SceneView)

#include <PantinEngine.hpp>
using namespace Pantin;

using namespace GLr::rendering;
using namespace GLr::wrappers::scene;

#include <QtCore/QtDebug>

#include <QtGui/QLabel>
#include <QtGui/QStackedWidget>

SceneView::SceneView()
:	View(new QStackedWidget),
 	_scene(K_NULL),
 	_view(K_NULL),
 	_handler(K_BLOCK_CREATE_INSTANCE(CameraTrackball)),
 	_glWrapper(K_NULL)
{
	blockName(tr("3D Scene View"));

	// No need to paint this dummy widget only here for events purposes !
	QGraphicsItem::setFlag(QGraphicsItem::ItemHasNoContents, true);

	// Default widget when there is no scene.
	QLabel* label = new QLabel(
			QString("%1\n%2").arg(
					tr("No scene to display."),
					tr("Drag & drop a scene on the view.")
				)
		);
	label->setAlignment(Qt::AlignCenter);
	viewWidget<QStackedWidget>()->addWidget(label);

	// To keep up with the project management...
	//connect(PantinEngine::Instance(), SIGNAL(projectLoaded()), SLOT(enableRendering()));
	//connect(PantinEngine::Instance(), SIGNAL(projectUnloaded()), SLOT(disableRendering()));
}

kbool SceneView::isViewForBlock(Block* block)
{
	return ( block->fastInherits<Scene>() || block->fastInherits<VisualScene>() || block->fastInherits<VisualSceneInstance>() );

}

kbool SceneView::activeBlock(Block* b)
{
	_visualScene = K_NULL;
	if(b->fastInherits<VisualScene>())
	{
		_visualScene = static_cast<VisualScene*>(b);
	}
	else if(b->fastInherits<VisualSceneInstance>())
	{
		_visualScene = static_cast<VisualSceneInstance*>(b)->instantiatedElement();
	}
	else if(b->fastInherits<Scene>())
	{
		_visualScene = static_cast<Scene*>(b)->visualSceneInstance()->instantiatedElement();
	}

	K_ASSERT(_visualScene)

	enableRendering(); // At this point, enable rendering !

	return false;
}

void SceneView::installEventHandler(SceneViewEventHandler* handler)
{
	_handler = handler;
}

Renderer* SceneView::renderer()
{
	K_ASSERT( _visualScene )
	return PantinEngine::InstanceRenderer(_visualScene->instance());
}

void SceneView::sceneWasRemoved()
{
	disableRendering();
}

void SceneView::enableRendering()
{
	K_ASSERT( _visualScene );

	if(_view)
	{
		// Cleanup the existing view and everything associated!
		disableRendering();
	}

	K_ASSERT( !_view )
	_view = new GLView( PantinEngine::InstanceRenderer(_visualScene->instance()) );
	_view->viewport()->installEventFilter(this);

	K_ASSERT( !_scene )
	_scene = new GLGraphicsScene(this);

	// TODO: Deal with viewport management
	GLViewport* vp = new GLViewport(_scene);
	_scene->addViewport(vp, 0, 0);

	// Activate the newly added view !
	viewWidget<QStackedWidget>()->addWidget(_view);
	viewWidget<QStackedWidget>()->setCurrentWidget(_view);

	// Get the wrapper for the scene from the renderer.
	_glWrapper = static_cast<WVisualScene*>(
			PantinEngine::InstanceRenderer(_visualScene->instance())->getWrapperForElement(_visualScene)
		);
	K_ASSERT( _glWrapper )

	// To properly update the view whenever something changes.
	connect(_visualScene, SIGNAL(elementChanged()), _scene, SLOT(update()));
	connect(_visualScene, SIGNAL(blockRemoved()), SLOT(sceneWasRemoved()));

	// Update the scene !
	_scene->update();
}

void SceneView::disableRendering()
{
	K_ASSERT( _visualScene )

	// Reset the view
	viewWidget<QStackedWidget>()->setCurrentIndex(0);

	// Disconnect from visual scene signals !
	_visualScene->disconnect(this);

	// Cleanup unused widgets and scene.
	K_ASSERT( _view )
	_view->deleteLater();
	_view = K_NULL;
	K_ASSERT( _scene )
	_scene->removeItem(this); // We remove this from the scene or we would get deleted next !
	_scene->deleteLater();
	_scene = K_NULL;
}

void SceneView::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{
	// Nothing ! -> Never called !
}

bool SceneView::sceneEventFilter(QGraphicsItem* watched, QEvent* event)
{
	return _handler->sceneViewEvent(static_cast<GLViewport*>(watched), event);
}

QVariant SceneView::ViewProperties(kint property)
{
	switch(property)
	{
	case Block::BlockTypeName:
		return tr("3D Scene View");
	case View::ViewDescription:
		return tr("View/Editor for 3D scenes and objects.");
	default:
		return QVariant();
	}
}
