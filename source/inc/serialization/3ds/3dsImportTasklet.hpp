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

#include <parallel/Tasklet.hpp>

#include <serialization/3ds/3dsTasklet.hpp>
#include <serialization/3ds/3dsSearchingTasklet.hpp>
#include <serialization/ProgressCallback.hpp>

#include <geometry/mesh/TrianglesPatch.hpp>
#include <geometry/Geometry.hpp>

#include <scene/Node.hpp>

#include <data/Block.hpp>

#include <QtCore/QFile>

namespace Pantin { namespace serialization { namespace ThreeDS {

class PantinExport ThreeDSImportTasklet
:	public ThreeDSTasklet
{
	Q_OBJECT

	K_TASKLET

public:
	ThreeDSImportTasklet(QFile* file, ThreeDSSearchingTasklet* tasklet);
	virtual ~ThreeDSImportTasklet();

	QList<Nigel::geometry::Geometry*>& geometries();
	QList<Nigel::scene::Node*>& sceneNodes();

	virtual void run(Tasklet* tasklet) const;

private:
	kbool isObjectToImport(QString objectName, kushort objectType) const;
	void readObject(QString objectName, QDataStream& data, TaskletContext& ctx) const;
	void readVertices(QDataStream& data, Nigel::geometry::mesh::TrianglesPatch* patch, TaskletContext& ctx) const;
	void readFaces(QDataStream& data, Nigel::geometry::mesh::TrianglesPatch* patch, TaskletContext& ctx) const;

private:
	ThreeDSSearchingTasklet* _tasklet;
	QFile* _file;
	QList<Nigel::geometry::Geometry*> _geometries;
	QList<Nigel::scene::Node*> _sceneNodes;
};

}}}
