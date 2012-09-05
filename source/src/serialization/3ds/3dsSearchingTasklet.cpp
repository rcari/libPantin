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

#include <serialization/3ds/3dsSearchingTasklet.hpp>
#include <serialization/3ds/3dsStructures.hpp>
using namespace Pantin::serialization::ThreeDS;

#include <QtCore/QCoreApplication>

#include <PantinModule.hpp>
#include <parallel/TaskletMacros.hpp>
K_TASKLET_I( Pantin::serialization::ThreeDS::ThreeDSSearchingTasklet )

ThreeDSSearchingTasklet::ThreeDSSearchingTasklet()
:	_file(K_NULL)
{
	blockName(tr("Searching for objects in 3ds file"));
	_listObjects.clear();
}

ThreeDSSearchingTasklet::~ThreeDSSearchingTasklet()
{
	for(kint i=_listObjects.size()-1; i>=0; i--)
	{
		ThreeDSContext* context = _listObjects.at(i);
		_listObjects.removeAt(i);
		if(context)
		{
			delete context;
		}
	}
}

void ThreeDSSearchingTasklet::setFile(QIODevice* file)
{
	_file = file;
}

QList<ThreeDSContext*>& ThreeDSSearchingTasklet::objects()
{
	return _listObjects;
}

void ThreeDSSearchingTasklet::run(Tasklet* tasklet) const
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
				static_cast<ThreeDSSearchingTasklet*>(tasklet)->objects().append(readObject(data, header, ctx));
				break;
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

ThreeDSContext* ThreeDSSearchingTasklet::readObject(QDataStream& data, ThreeDSHeader& header, TaskletContext& ctx) const
{
	QString text = readText(data, ctx);
	TaskletRunner::progress(ctx.tasklet, tr("An object named \"%1\" found.").arg(text));
	ThreeDSContext* context = new ThreeDSContext();
	context->name = text;
	context->type = NAMED_OBJECT;
	data.skipRawData(header.size - text.size()-1);
	ctx.progressPosition += (header.size - text.size()-1);
	return context;
}
