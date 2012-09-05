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

#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>

#include <rendering/ICamera.hpp>

namespace Pantin { namespace views { namespace sceneView {

class GLViewport;

class PantinExport GLViewportCamera : public GLr::rendering::ICamera
{
public:
	GLViewportCamera(GLViewport*);

	// ICamera
	virtual void orientation(Nigel::math::Quaternionf&) const;
	virtual void position(Nigel::math::Vector4f&) const;

	virtual Projection projection() const;

	// Common
	virtual kfloat zNear() const;
	virtual kfloat zFar() const;

	// Perspective
	virtual kfloat yFov() const;
	virtual kfloat ratio() const;

	// Orthogonal
	virtual kfloat left() const;
	virtual kfloat right() const;
	virtual kfloat bottom() const;
	virtual kfloat top() const;

	// API for controlling the camera
	inline Nigel::math::Vector4f& center() { return _center; }
	inline Nigel::math::Quaternionf& orientation() { return _orientation; }
	inline void radius(kfloat r) { _radius = r; }
	inline kfloat radius() const { return _radius; }

private:
	GLViewport*	_viewport;
	Nigel::math::Vector4f _center;
	Nigel::math::Quaternionf _orientation;
	kfloat _radius;
};

}}}
