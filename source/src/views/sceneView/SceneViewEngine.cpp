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

#include <views/sceneView/SceneViewEngine.hpp>
using namespace Pantin::views::sceneView;

#include <GooeyEngine.hpp>
using namespace Gooey;

kbool SceneViewEngine::RegisterTool(const MetaBlock* mb)
{
	qDebug("Pantin / 3D Scene View / Registering tool %s", qPrintable(mb->blockClassName()));

	QAction* action = new QAction(
			GooeyEngine::GetIcon(mb->blockIconPath()),
			mb->blockProperty(Block::BlockTypeName).toString(),
			K_NULL
		);
	// Store the tool class name as the object name of the QAction
	action->setObjectName(mb->objectClassName());
	// Store a pointer to the MetaBlock of the tool in the QAction data.
	action->setData(
			QVariant::fromValue<Kore::data::ConstBlockPointer>(Kore::data::ConstBlockPointer(mb))
		);

	ToolsActions()->append(action);
	return true;
}

const QList<QAction*>& SceneViewEngine::RegisteredToolsActions()
{
	return *ToolsActions();
}

QList<QAction*>* SceneViewEngine::ToolsActions()
{
	return _ToolsActions == K_NULL
			? _ToolsActions = new QList<QAction*>
			: _ToolsActions;
}

QList<QAction*>* SceneViewEngine::_ToolsActions = K_NULL;
