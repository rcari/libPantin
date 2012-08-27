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

#include <views/sceneView/tools/CameraControl.hpp>
#include <views/sceneView/GLViewport.hpp>
using namespace Pantin::views::sceneView;

#include <QtGui/QGraphicsSceneWheelEvent>

#include <PantinModule.hpp>

#define K_BLOCK_TYPE Pantin::views::sceneView::CameraControl
#include <data/BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON_DEFAULT
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_DEFAULT
K_BLOCK_END

CameraControl::CameraControl()
:	_keyCounter(0),
 	_activeVp(K_NULL)
{
	// Init keys to false !
	memset(_keys, 0x00, sizeof(_keys));
}

kbool CameraControl::keyPressEvent(GLViewport* vp, QKeyEvent* event)
{
	if(event->isAutoRepeat())
	{
		return false;
	}

	switch(event->key())
	{
	case Qt::Key_Up:
		_keys[Up] = true;
		break;
	case Qt::Key_Down:
		_keys[Down] = true;
		break;
	case Qt::Key_Left:
		_keys[Left] = true;
		break;
	case Qt::Key_Right:
		_keys[Right] = true;
		break;
	default:
		goto end;
		break;
	}

	_keyCounter++;

	_activeVp = vp;
	_activeVp->installViewportPainter(this);

	vp->update();

end:
	return true;
}

kbool CameraControl::keyReleaseEvent(GLViewport* vp, QKeyEvent* event)
{
	if(event->isAutoRepeat())
	{
		return false;
	}

	switch(event->key())
	{
	case Qt::Key_Up:
		_keys[Up] = false;
		break;
	case Qt::Key_Down:
		_keys[Down] = false;
		break;
	case Qt::Key_Left:
		_keys[Left] = false;
		break;
	case Qt::Key_Right:
		_keys[Right] = false;
		break;
	default:
		goto end;
		break;
	}

	_keyCounter--;

	if(_keyCounter == 0)
	{
		_activeVp->resetViewportPainter();
		_activeVp = K_NULL;
	}

	vp->update();

end:
	return true;
}


kbool CameraControl::mouseDoubleClickEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return Tool::mouseDoubleClickEvent(vp, event);
}

kbool CameraControl::mouseMoveEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	//QPointF delta = event->pos() - event->lastPos();

	//vp->camera()->move(0.001f * delta.x(), 0.001f * delta.y(), 0.0f);

	vp->update();

	return Tool::mouseMoveEvent(vp, event);
}

kbool CameraControl::mousePressEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return true;
	//return Tool::mousePressEvent(vp, event);
}

kbool CameraControl::mouseReleaseEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return Tool::mouseReleaseEvent(vp, event);
}

kbool CameraControl::wheelEvent(GLViewport* vp, QGraphicsSceneWheelEvent* event)
{
	kfloat delta = (kfloat)event->delta();

	if(event->modifiers() & Qt::ControlModifier)
	{
		delta *= 10.0f; // 10 times faster
	}
	else if(event->modifiers() & Qt::ShiftModifier)
	{
		delta *= 0.1f; // 10 times slower
	}

	//vp->camera()->move(0.0f, 0.0f, -delta);

	return Tool::wheelEvent(vp, event);
}

void CameraControl::paintViewport(GLViewport* vp, QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget)
{
	K_ASSERT( vp == _activeVp )

	/*kfloat dx = 0.0f;
	kfloat dy = 0.0f;

	if(_keys[Up])
	{
		dy += 1000.0f;
	}
	if(_keys[Down])
	{
		dy -= 1000.0f;
	}

	if(_keys[Left])
	{
		dx += 1000.0f;
	}
	if(_keys[Right])
	{
		dx -= 1000.0f;
	}

	vp->camera()->move(dx, dy, 0.0f);*/

	vp->defaultPaintViewport(painter, options, widget);

	// Push an update request for continuous painting !
	vp->update();
}
