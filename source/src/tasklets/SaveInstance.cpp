
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

#include <tasklets/SaveInstance.hpp>
using namespace Pantin::tasklets;
using namespace Nigel::COLLADA;

#include <PantinModule.hpp>
#include <parallel/TaskletMacros.hpp>
K_TASKLET_I( Pantin::tasklets::SaveInstance )

SaveInstance::SaveInstance(const DocumentInstance* instance, const QString& filePath, kbool autoDelete)
:	Tasklet(autoDelete),
 	_instance(instance),
 	_filePath(filePath)
{
}

void SaveInstance::run(Tasklet* tasklet) const
{
	SaveInstance* saveTasklet = static_cast<SaveInstance*>(tasklet);

	TaskletRunner::start(tasklet);

	// Simply save the stuff...
	if(DocumentInstance::SaveDocumentInstance(saveTasklet->_instance, saveTasklet->_filePath, saveTasklet))
	{
		TaskletRunner::complete(tasklet);
	}
	else
	{
		TaskletRunner::fail(tasklet);
	}
}

kbool SaveInstance::continueOperation()
{
	return keepRunning();
}

void SaveInstance::progress(kuint64 progress, kuint64 total)
{
	runnerProgress(progress, total);
}
