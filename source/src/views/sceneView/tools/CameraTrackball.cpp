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

#include <views/sceneView/tools/CameraTrackball.hpp>
#include <views/sceneView/GLViewport.hpp>
using namespace Pantin::views::sceneView;

#include <QtCore/QtDebug>
#include <QtGui/QGraphicsSceneWheelEvent>
#include <QtGui/QVector2D>

#include <math/Math.hpp>
using namespace Nigel::math;

#include <PantinModule.hpp>

#define K_BLOCK_TYPE Pantin::views::sceneView::CameraTrackball
#include <data/BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON_DEFAULT
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_DEFAULT
K_BLOCK_END

CameraTrackball::CameraTrackball()
:	_activeVp(K_NULL)
{
	_currentOrientation.identity();
}

kbool CameraTrackball::mouseMoveEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	Vector4f anchor = projectOnSurface(vp, normalizeCoordinates(vp, event->buttonDownPos(Qt::LeftButton)));
	Vector4f current = projectOnSurface(vp, normalizeCoordinates(vp, event->pos()));

	/*Quaternionf transformation = _currentOrientation;
	transformation *= computeIncremental(anchor, current);*/

	// Set the transformation
	vp->camera()->orientation() = computeIncremental(anchor, current);

	vp->update();

	return Tool::mouseMoveEvent(vp, event);
}

kbool CameraTrackball::mousePressEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	if(event->button() == Qt::LeftButton)
	{
		qDebug() << "Mouse down!";
		_currentOrientation = vp->camera()->orientation();
		vp->update();
		return true;
	}
	else
	{
		return Tool::mousePressEvent(vp, event);
	}
}

kbool CameraTrackball::mouseReleaseEvent(GLViewport* vp, QGraphicsSceneMouseEvent* event)
{
	return Tool::mouseReleaseEvent(vp, event);
}

kbool CameraTrackball::wheelEvent(GLViewport* vp, QGraphicsSceneWheelEvent* event)
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

	// Change the radius
	vp->camera()->radius(vp->camera()->radius() - delta);

	vp->update();

	return Tool::wheelEvent(vp, event);
}

void CameraTrackball::paintViewport(GLViewport* vp, QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget)
{
	K_ASSERT( vp == _activeVp )

	vp->defaultPaintViewport(painter, options, widget);

	// Push an update request for continuous painting !
	vp->update();
}

Vector4f CameraTrackball::projectOnSurface(GLViewport* vp, const QPointF& point) const
{
	kfloat x, y, z;
	const kfloat radius = 1.0; //vp->camera()->radius();
	const kfloat radius2 = radius*radius;

	QVector2D vec(point);
	const kfloat length2 = vec.lengthSquared();
	const kfloat lengthInv = 1.0 / vec.length();

	x = point.x();
	y = point.y();

    if(length2 <= radius2)
    {
    	z = Math::Sqrtf(radius2 - length2);
    }
    else
    {
    	x *= radius * lengthInv;
    	y *= radius * lengthInv;
    	z = 0.0f;
    }

    return Vector4f(x, y, z);
}

Quaternionf CameraTrackball::computeIncremental(const Vector4f& anchor, const Vector4f& current) const
{
	Quaternionf q_current(0, current.normalized());
	q_current *= Quaternionf(0, anchor.normalized());
	return Quaternionf(q_current.invert());
}

QPointF CameraTrackball::normalizeCoordinates(GLViewport* vp, const QPointF& point) const
{
	QRectF vpRect = vp->rect();
	//qDebug() << "Before:" << point << vpRect;

	double x = point.x(), y = point.y();
	x = 2.0 * x / vpRect.width() - 1.0;
	/*if(x < -1.0) x = -1.0;
	if(x >  1.0) x =  1.0;*/

	y = 2.0 * y / vpRect.height() - 1.0;
	/*if(y < -1.0) y = -1.0;
	if(y >  1.0) y =  1.0;*/

	/*QPointF pt(x,y);
	qDebug() << "After:" << pt;*/

	return QPointF(x,y);
}
