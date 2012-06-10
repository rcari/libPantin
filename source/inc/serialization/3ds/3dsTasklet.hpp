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

#pragma once

#include <PantinExport.hpp>

#include <parallel/Tasklet.hpp>

#include <serialization/ProgressCallback.hpp>

namespace Pantin { namespace serialization { namespace ThreeDS {

struct ThreeDSHeader;

struct ThreeDSContext
{
	QString name;
	kushort type;
};

struct TaskletContext
{
	kint64 progressPosition;
	kint64 fileSize;
	Kore::parallel::Tasklet* tasklet;
};

class PantinExport ThreeDSTasklet
:	public Kore::parallel::Tasklet,
 	private Kore::serialization::ProgressCallback
{
	Q_OBJECT

public:
	ThreeDSTasklet();

	virtual void run(Tasklet* tasklet) const = K_NULL;

private:
	virtual kbool continueOperation();
	virtual void progress(kuint64 progress, kuint64 total);

protected:
	//Basic types used inside 3DS Files
	void readHeader(QDataStream& data, ThreeDSHeader& header, TaskletContext& ctx) const;
	void displayHeader(const ThreeDSHeader& header) const;
	void enterChunk(QDataStream& data, const ThreeDSHeader& header, TaskletContext& ctx) const;
	void skipChunk(QDataStream& data, const ThreeDSHeader& header, TaskletContext& ctx) const;
	bool readPercentageChunk(QDataStream& data, kfloat& value, TaskletContext& ctx) const;
	bool readColorChunk(QDataStream& data, kfloat color[3], TaskletContext& ctx) const;
	QString readText(QDataStream& data, TaskletContext& ctx) const;
	kshort readShort(QDataStream& data, TaskletContext& ctx) const;
	klong readLong(QDataStream& data, TaskletContext& ctx) const;
	kfloat readFloat(QDataStream& data, TaskletContext& ctx) const;
	kchar readChar(QDataStream& data, TaskletContext& ctx) const;
};

}}}
