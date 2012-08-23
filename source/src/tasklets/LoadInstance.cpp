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

#include <tasklets/LoadInstance.hpp>
using namespace Pantin::tasklets;
using namespace Nigel::COLLADA;

#include <QtCore/QCoreApplication>

#include <PantinModule.hpp>

#include <parallel/TaskletMacros.hpp>
K_TASKLET_I( Pantin::tasklets::LoadInstance )

LoadInstance::LoadInstance(const QString& filePath, kbool autoDelete)
:	Tasklet(autoDelete),
 	_instanceFilePath(filePath),
 	_loadedInstance(K_NULL)
{
	blockName(tr("Loading instance"));
}

DocumentInstance* LoadInstance::loadedInstance()
{
	return _loadedInstance;
}

void LoadInstance::run(Tasklet* tasklet) const
{
	K_ASSERT( tasklet == this )

	_loadedInstance = K_NULL;

	TaskletRunner::start(tasklet);

	// Simply load the stuff...
	_loadedInstance = DocumentInstance::LoadDocumentInstance(_instanceFilePath, const_cast<LoadInstance*>(this));
	if(_loadedInstance)
	{
		if(_loadedInstance->thread() != qApp->thread())
		{
			// Move the created structure to the default application thread !
			_loadedInstance->moveToThread(qApp->thread());
			// To have the moveToThread actually performed !
			QCoreApplication::processEvents();
		}

		TaskletRunner::complete(tasklet);
	}
	else
	{
		TaskletRunner::fail(tasklet);
	}
}

kbool LoadInstance::continueOperation()
{
	return keepRunning();
}

void LoadInstance::progress(kuint64 progress, kuint64 total)
{
	runnerProgress(progress, total);
}
