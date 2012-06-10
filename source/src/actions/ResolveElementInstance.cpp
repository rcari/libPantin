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

#include <actions/ResolveElementInstance.hpp>
using namespace Pantin::actions;

using namespace Kore::data;

#include <COLLADA/instantiation/ElementInstance.hpp>
using namespace Nigel::COLLADA::instantiation;

#include <GooeyEngine.hpp>
#include <windows/MainWindow.hpp>
using namespace Gooey;

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

G_STATIC_BLOCK_ACTION_I( Pantin::actions::ResolveElementInstance )

ResolveElementInstance::ResolveElementInstance(QObject* parent)
:	BlockAction(parent)
{
	setText(tr("Resolve reference"));
	registerWithMetaBlock(ElementInstance::StaticMetaBlock());
}

kbool ResolveElementInstance::prepareForBlock(Block* block)
{
	// Only enable if the instance is not resolved !
	setEnabled( !static_cast<ElementInstance*>(block)->isInstanceResolved() );
	return true;
}

void ResolveElementInstance::performAction(Block* block) const
{
	Q_UNUSED(block);

	K_ASSERT( block->fastInherits<ElementInstance>() ) // It has to be a Project !

	ElementInstance* instance = static_cast<ElementInstance*>(block);

	// Load and return !
	instance->instantiatedElement();
}
