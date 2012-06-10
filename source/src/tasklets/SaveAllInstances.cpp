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

#include <tasklets/SaveAllInstances.hpp>
using namespace Pantin::tasklets;
using namespace Pantin;

#include <COLLADA/DocumentInstance.hpp>
using namespace Nigel::COLLADA;

#include <parallel/TaskletMacros.hpp>
K_TASKLET_I( Pantin::tasklets::SaveAllInstances )

SaveAllInstances::SaveAllInstances(const PantinInstancesManager* manager, kbool autoDelete)
:	Tasklet(autoDelete),
 	_manager(manager)
{
	blockName(tr("Saving project"));
}

void SaveAllInstances::run(Tasklet* tasklet) const
{
	TaskletRunner::start(tasklet);
	TaskletRunner::progress(tasklet, 0, _manager->size());

	for(kint i = 0; i < _manager->size(); i++)
	{
		if(!TaskletRunner::keepRunning(tasklet))
		{
			TaskletRunner::cancel(tasklet);
			return;
		}

		const DocumentInstance* instance = _manager->at<DocumentInstance>(i);
		QUndoStack* stack = _manager->instanceUndoStack(instance);

		// Progress...
		TaskletRunner::progress(tasklet, i, _manager->size());
		TaskletRunner::progress(tasklet, tr("Saving %1").arg(instance->blockName()));

		if(stack->isClean())
		{
			continue;
		}

		if(!DocumentInstance::SaveDocumentInstance(instance, QString(), static_cast<SaveAllInstances*>(tasklet)))
		{
			TaskletRunner::fail(tasklet);
			return;
		}

		stack->setClean();
	}

	TaskletRunner::complete(tasklet);
}

kbool SaveAllInstances::continueOperation()
{
	return keepRunning();
}

void SaveAllInstances::progress(kuint64, kuint64)
{
	// We don't detail individual files progress !
}
