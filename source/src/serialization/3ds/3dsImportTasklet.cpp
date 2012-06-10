/*
 * 	Copyright (c) 2010-2011, Christophe EVENO
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

#include <serialization/3ds/3dsImportTasklet.hpp>
#include <serialization/3ds/3dsStructures.hpp>
using namespace Pantin::serialization::ThreeDS;

#include <geometry/Geometry.hpp>
#include <geometry/GeometryInstance.hpp>
#include <geometry/mesh/TrianglesPatch.hpp>
#include <geometry/mesh/Mesh.hpp>
#include <geometry/mesh/Structures.hpp>
#include <geometry/mesh/buffer/VertexBuffer.hpp>
using namespace Nigel::geometry;
using namespace Nigel::geometry::mesh;
using namespace Nigel::geometry::mesh::buffer;

#include <scene/transformation/Matrix.hpp>
#include <scene/Node.hpp>
using namespace Nigel::scene::transformation;
using namespace Nigel::scene;

#include <COLLADA/DocumentInstance.hpp>
#include <COLLADA/instantiation/ElementInstance.hpp>
using namespace Nigel::COLLADA;
using namespace Nigel::COLLADA::instantiation;

#include <QtCore/QCoreApplication>

#include <parallel/TaskletMacros.hpp>
K_TASKLET_I( Pantin::serialization::ThreeDS::ThreeDSImportTasklet )

ThreeDSImportTasklet::ThreeDSImportTasklet(QFile* file, ThreeDSSearchingTasklet* tasklet)
:	_tasklet(tasklet),
 	_file(file)
{
	blockName(tr("Importing for objects from 3ds file"));
	_geometries.clear();
	_sceneNodes.clear();
}

ThreeDSImportTasklet::~ThreeDSImportTasklet()
{
	_geometries.clear();
	_sceneNodes.clear();
}

QList<Geometry*>& ThreeDSImportTasklet::geometries()
{
	return _geometries;
}

QList<Node*>& ThreeDSImportTasklet::sceneNodes()
{
	return _sceneNodes;
}

void ThreeDSImportTasklet::run(Tasklet* tasklet) const
{
	TaskletRunner::start(tasklet);

	if(!_file)
	{
		TaskletRunner::fail(tasklet);
		return;
	}

	TaskletContext ctx;
	ctx.progressPosition = 0;
	ctx.fileSize = _file->size();
	ctx.tasklet = tasklet;

	TaskletRunner::progress(ctx.tasklet, ctx.progressPosition, ctx.fileSize);

	_file->open(QIODevice::ReadOnly);
	QDataStream data(_file);
	data.setByteOrder(QDataStream::LittleEndian);
	data.setFloatingPointPrecision(QDataStream::SinglePrecision);

	ThreeDSHeader header;
	readHeader(data, header, ctx);
	if(header.id == M3DMAGIC)
	{
		while(!data.atEnd())
		{
			switch (header.id)
			{
			//main chunk
			case M3DMAGIC:
				enterChunk(data, header, ctx);
				break;
			//version chunks
			case M3D_VERSION:
			{
				klong nb = readLong(data, ctx);
				if(nb > 3)
				{
					//version not supported
					qDebug("Pantin / The 3DS file may not load correctly because the version is greater than 3.");
					_file->close();
					TaskletRunner::progress(ctx.tasklet, ctx.fileSize, ctx.fileSize);
					TaskletRunner::fail(tasklet);
					return;
				}
				break;
			}
			//editable chunk
			case MDATA:
				enterChunk(data, header, ctx);
				break;
			//object chunks
			case NAMED_OBJECT:
			{
				QString name = readText(data, ctx);
				if(isObjectToImport(name, header.id))
				{
					TaskletRunner::progress(tasklet, tr("Begin import of %1").arg(name));
					char* tab = new char[header.size - name.size() - 1];
					data.readRawData(tab, header.size - name.size() - 1);
					QByteArray tempData(tab, header.size - name.size() - 1);
					QDataStream stream(tempData);
					stream.setByteOrder(QDataStream::LittleEndian);
					stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
					readObject(name, stream, ctx);
					delete[] tab;
					TaskletRunner::progress(tasklet, tr("End import of %1").arg(name));
				}
				else
				{
					data.skipRawData(header.size - name.size() - 1);
					ctx.progressPosition += (header.size - name.size() - 1);
				}
				break;
			}
			//not actually used chunks
			case KFDATA:
			case MAT_ENTRY:
			case MASTER_SCALE:
			case MESH_VERSION:
				skipChunk(data, header, ctx);
				break;
			//unknown chunks
			default:
				skipChunk(data, header, ctx);
				displayHeader(header);
				break;
			}
			TaskletRunner::progress(ctx.tasklet, ctx.progressPosition, ctx.fileSize);
			readHeader(data, header, ctx);
		}
	}
	else
	{
		//the file has not the magic bytes
		_file->close();
		TaskletRunner::progress(ctx.tasklet, ctx.fileSize, ctx.fileSize);
		TaskletRunner::fail(tasklet);
		return;
	}

	_file->close();
	TaskletRunner::progress(ctx.tasklet, ctx.progressPosition, ctx.fileSize);
	TaskletRunner::complete(tasklet);
}

kbool ThreeDSImportTasklet::isObjectToImport(QString objectName, kushort objectType) const
{
	QList<ThreeDSContext*> context = _tasklet->objects();
	for(kint i=0; i<context.size(); i++)
	{
		ThreeDSContext* ctx = context.at(i);
		if((ctx->name.compare(objectName) == 0) && (objectType == ctx->type))
		{
			return true;
		}
	}
	return false;
}

void ThreeDSImportTasklet::readObject(QString objectName, QDataStream& data, TaskletContext& ctx) const
{
	ThreeDSHeader header;
	// Create a geometry
	Nigel::geometry::Geometry* geometry = K_BLOCK_CREATE_INSTANCE(Nigel::geometry::Geometry);
	geometry->blockName(objectName);

	GeometryInstance* geometryInstance = ElementInstance::CreateElementInstance<GeometryInstance>(geometry);

	// Create a Mesh
	Mesh* mesh = Mesh::CreateMesh();
	// Create a triangles patch
	TrianglesPatch* patch = K_BLOCK_CREATE_INSTANCE ( Nigel::geometry::mesh::TrianglesPatch );
	// and add it to the mesh.
	mesh->addBlock(patch);

	patch->blockName(tr("Faces"));
	mesh->verticesBuffer()->blockName(tr("Vertices"));

	while(!data.atEnd())
	{
		readHeader(data, header, ctx);
		switch(header.id)
		{
		//triangles object chunk
		case N_TRI_OBJECT:
			enterChunk(data, header, ctx);
			break;
		//vertices chunk
		case POINT_ARRAY:
			readVertices(data, patch, ctx);
			break;
		//faces chunk
		case FACE_ARRAY:
			readFaces(data, patch, ctx);
			break;
		//matrix chunk
		case MESH_MATRIX:
			{
				Matrix* matrix = K_BLOCK_CREATE_INSTANCE(Nigel::scene::transformation::Matrix);

				kfloat* values = matrix->matrix().values();

				values[ 0] = readFloat(data, ctx);
				values[ 4] = readFloat(data, ctx);
				values[ 8] = readFloat(data, ctx);

				values[ 1] = readFloat(data, ctx);
				values[ 5] = readFloat(data, ctx);
				values[ 9] = readFloat(data, ctx);

				values[ 2] = readFloat(data, ctx);
				values[ 6] = readFloat(data, ctx);
				values[10] = readFloat(data, ctx);

				matrix->matrix().identity();

				values[ 3] = readFloat(data, ctx);
				values[ 7] = readFloat(data, ctx);
				values[11] = readFloat(data, ctx);

				Node* node = K_BLOCK_CREATE_INSTANCE(Nigel::scene::Node);
				node->addBlock(matrix);
				node->addBlock(geometryInstance);
				node->moveToThread(qApp->thread());
				static_cast<ThreeDSImportTasklet*>(ctx.tasklet)->sceneNodes().append(node);
			}
			break;
		//not actually used chunks
		case MSH_MAT_GROUP:
		case SMOOTH_GROUP:
		case TEX_VERTS:
			skipChunk(data, header, ctx);
			break;
		//unknown chunks
		default:
			skipChunk(data, header, ctx);
			displayHeader(header);
			break;
		}
	}

	geometry->addBlock(mesh);
	geometry->moveToThread(qApp->thread());
	static_cast<ThreeDSImportTasklet*>(ctx.tasklet)->geometries().append(geometry);
}

void ThreeDSImportTasklet::readVertices(QDataStream& data, TrianglesPatch* patch, TaskletContext& ctx) const
{
	kushort nb = readShort(data, ctx);
	const kint offset = patch->appendVertices(nb);

	Vertex* vertices = patch->vertices();

	Vertex vertex;
	for(kushort i = 0; i < nb; i++)
	{
		vertex.x = readFloat(data, ctx);
		vertex.y = readFloat(data, ctx);
		vertex.z = readFloat(data, ctx);
		vertices[offset+i] = vertex;
	}
	patch->doneAppendingVertices(nb);
}

void ThreeDSImportTasklet::readFaces(QDataStream& data, TrianglesPatch* patch, TaskletContext& ctx) const
{
	kushort nb = readShort(data, ctx);

	const kint offset = patch->appendTriangles(nb);

	// It is VERY IMPORTANT to store the faces pointer AFTER having allocated / modified the patch !
	// Because the QVector will reallocate the buffer to store the data !!!
	TriangleFace* faces = patch->triangles();

	TriangleFace face;
	for(kushort i = 0; i < nb; i++)
	{
		face.v0 = readShort(data, ctx);
		face.v1 = readShort(data, ctx);
		face.v2 = readShort(data, ctx);
		data.skipRawData(2); // flags !
		ctx.progressPosition += 2;
		faces[offset+i] = face;
	}
	patch->doneAppendingTriangles(nb);
}
