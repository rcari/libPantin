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

#include <serialization/3ds/3dsSerializer.hpp>
#include <serialization/3ds/3dsTasklet.hpp>
#include <serialization/3ds/3dsFileImporterWizard.hpp>
using namespace Pantin::serialization::ThreeDS;
using namespace Pantin::serialization;

#include <windows/TaskletProgressDialog.hpp>
using namespace Gooey::windows;

#define K_BLOCK_TYPE Pantin::serialization::ThreeDS::ThreeDSSerializer
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON_DEFAULT
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_METHOD( Pantin::serialization::ThreeDS::ThreeDSSerializer::SerializerProperties )
K_BLOCK_END

#include <data/BlockSettings.hpp>
using namespace Kore::data;
using namespace Kore;

#include <PantinEngine.hpp>
using namespace Pantin;

P_SERIALIZER_I( Pantin::serialization::ThreeDS::ThreeDSSerializer )

ThreeDSSerializer::ThreeDSSerializer()
{
	blockName(tr("3DS Serializer"));
}

QStringList ThreeDSSerializer::getSupportedExtensions() const
{
	return QStringList() << tr("3ds");
}

void ThreeDSSerializer::addWizardSteps(QWizard* wizard) const
{
	wizard->setPage(ThreeDSSerializer::GetObjectsProgressPageId, new GetObjectsProgressPage);
	wizard->setPage(ThreeDSSerializer::ObjectsChoicesPageId, new ObjectsChoicesPage);
	wizard->setPage(ThreeDSSerializer::LoadObjectsProgressPageId, new LoadObjectsProgressPage);
}

kint ThreeDSSerializer::getFirstWizardStep() const
{
	return ThreeDSSerializer::GetObjectsProgressPageId;
}

QVariant ThreeDSSerializer::SerializerProperties(kint property)
{
	switch(property)
	{
	case Block::BlockTypeName:
		return tr("3DS Serializer");
	default:
		return QVariant();
	}
}
