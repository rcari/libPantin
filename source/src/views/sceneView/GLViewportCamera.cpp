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

#include <views/sceneView/GLViewportCamera.hpp>
#include <views/sceneView/GLViewport.hpp>
using namespace Pantin::views::sceneView;
using namespace GLr::rendering;
using namespace Nigel::math;

#include <QtCore/QtDebug>
#include <QtCore/QRectF>

#include <math.h>
#include <float.h>

GLViewportCamera::GLViewportCamera(GLViewport* vp)
:	_viewport(vp),
 	_center(0.0f, 0.0f, 0.0f),
 	_radius(5000.0f)
{
	_orientation.identity();
}

// ICamera
void GLViewportCamera::orientation(Quaternionf& quat) const
{
	quat = _orientation;
}

void GLViewportCamera::position(Vector4f& vec) const
{
	vec = _center;

	Vector4f zVec(0.0f, 0.0f, -1.0f, 0.0f);
	_orientation.rotateVector(zVec);

	zVec *= _radius;

	vec += zVec;
}

ICamera::Projection GLViewportCamera::projection() const
{
	return ICamera::Perspective;
}

kfloat GLViewportCamera::zNear() const
{
	return 1.0f;
}

kfloat GLViewportCamera::zFar() const
{
	return FLT_MAX;
}

kfloat GLViewportCamera::yFov() const
{
	return 90.0f / ratio(); // We want 90 deg X fov!
}

kfloat GLViewportCamera::ratio() const
{
	QRectF rect = _viewport->boundingRect();
	return rect.width() / rect.height();
}

kfloat GLViewportCamera::left() const
{
	return 0.0f;
}

kfloat GLViewportCamera::right() const
{
	return 0.0f;
}

kfloat GLViewportCamera::bottom() const
{
	return 0.0f;
}

kfloat GLViewportCamera::top() const
{
	return 0.0f;
}
