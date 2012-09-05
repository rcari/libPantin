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

#include <actions/AddTransformations.hpp>
using namespace Pantin::actions;

using namespace Kore::data;

#include <scene/Node.hpp>
using namespace Nigel::scene;
using namespace Nigel::scene::transformation;

#include <GooeyEngine.hpp>
using namespace Gooey;

#include <commands/AddBlock.hpp>
using namespace Gooey::commands;

#include <PantinEngine.hpp>
using namespace Pantin;

G_STATIC_BLOCK_ACTION_I( Pantin::actions::AddMatrix )

AddMatrix::AddMatrix(QObject* parent)
:	AddElement<Matrix>(parent, tr("Add matrix"), QStringList() << tr("Transformation"))
{
	registerWithMetaBlock(Node::StaticMetaBlock());
}

G_STATIC_BLOCK_ACTION_I( Pantin::actions::AddRotation )

AddRotation::AddRotation(QObject* parent)
:	AddElement<Rotation>(parent, tr("Add rotation"), QStringList() << tr("Transformation"))
{
	registerWithMetaBlock(Node::StaticMetaBlock());
}

G_STATIC_BLOCK_ACTION_I( Pantin::actions::AddScale )

AddScale::AddScale(QObject* parent)
:	AddElement<Scale>(parent, tr("Add scale"), QStringList() << tr("Transformation"))
{
	registerWithMetaBlock(Node::StaticMetaBlock());
}

G_STATIC_BLOCK_ACTION_I( Pantin::actions::AddTranslation )

AddTranslation::AddTranslation(QObject* parent)
:	AddElement<Translation>(parent, tr("Add translation"), QStringList() << tr("Transformation"))
{
	registerWithMetaBlock(Node::StaticMetaBlock());
}
