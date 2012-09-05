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

#include <serialization/3ds/3dsTasklet.hpp>
#include <serialization/3ds/3dsStructures.hpp>
using namespace Pantin::serialization::ThreeDS;

#include <PantinEngine.hpp>
using namespace Pantin;

ThreeDSTasklet::ThreeDSTasklet()
:	Tasklet(false)
{

}

kbool ThreeDSTasklet::continueOperation()
{
	return keepRunning();
}

void ThreeDSTasklet::progress(kuint64, kuint64)
{

}

void ThreeDSTasklet::readHeader(QDataStream& data, ThreeDSHeader& header, TaskletContext& ctx) const
{
	header.id = readShort(data, ctx);
	header.size = readLong(data, ctx) - 6; //remove 6 bytes for the ThreeDSHeader size
}

void ThreeDSTasklet::displayHeader(const ThreeDSHeader& header) const
{
	qDebug("Pantin / the 3DS file contains a chunk with id: 0x%X and size: %lu", header.id, header.size);
}

void ThreeDSTasklet::enterChunk(QDataStream& data, const ThreeDSHeader& header, TaskletContext& ctx) const
{
	
}

void ThreeDSTasklet::skipChunk(QDataStream& data, const ThreeDSHeader& header, TaskletContext& ctx) const
{
	data.skipRawData(header.size);
	ctx.progressPosition += header.size;
}

bool ThreeDSTasklet::readPercentageChunk(QDataStream& data, kfloat& value, TaskletContext& ctx) const
{
	ThreeDSHeader header;
	readHeader(data, header, ctx);
	if(header.id == INT_PERCENTAGE)
	{
		value = readShort(data, ctx);
	}
	else if(header.id == FLOAT_PERCENTAGE)
	{
		value = readFloat(data, ctx);
	}
	else
	{
		skipChunk(data, header, ctx);
		return false;
	}
	return true;
}

bool ThreeDSTasklet::readColorChunk(QDataStream& data, kfloat color[3], TaskletContext& ctx) const
{
	ThreeDSHeader header;
	readHeader(data, header, ctx);
	if(header.id == COLOR_24)
	{
		color[0] = readChar(data, ctx);
		color[1] = readChar(data, ctx);
		color[2] = readChar(data, ctx);
	}
	else if(header.id == COLOR_F)
	{
		color[0] = readFloat(data, ctx);
		color[1] = readFloat(data, ctx);
		color[2] = readFloat(data, ctx);
	}
	else
	{
		skipChunk(data, header, ctx);
		return false;
	}
	return true;
}

QString ThreeDSTasklet::readText(QDataStream& data, TaskletContext& ctx) const
{
	QString text;
	kchar c = readChar(data, ctx);
	while(c != 0x00)
	{
		text.append(c);
		c = readChar(data, ctx);
	}
	return text;
}

kshort ThreeDSTasklet::readShort(QDataStream& data, TaskletContext& ctx) const
{
	qint16 nb;
	data >> nb;
	ctx.progressPosition += 2;
	return nb;
}

klong ThreeDSTasklet::readLong(QDataStream& data, TaskletContext& ctx) const
{
	qint32 nb;
	data >> nb;
	ctx.progressPosition += 4;
	return nb;
}

kfloat ThreeDSTasklet::readFloat(QDataStream& data, TaskletContext& ctx) const
{
	kfloat nb;
	data >> nb;
	ctx.progressPosition += 4;
	return nb;
}

kchar ThreeDSTasklet::readChar(QDataStream& data, TaskletContext& ctx) const
{
	qint8 c;
	data >> c;
	ctx.progressPosition += 1;
	return c;
}
