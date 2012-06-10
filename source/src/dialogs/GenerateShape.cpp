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

#include <dialogs/GenerateShape.hpp>
using namespace Pantin::dialogs;

#include <modeling/shape/GeneratorParameter.hpp>
#include <modeling/shape/Shape.hpp>
using namespace Nigel::modeling::shape;
using namespace Nigel::geometry;

#include <COLLADA/instantiation/ElementInstance.hpp>
using namespace Nigel::COLLADA::instantiation;

#include <QtGui/QtGui>

GenerateShape::GenerateShape(const Generator* generator, QWidget* parent)
:	QDialog(parent),
 	_generator(generator),
 	_geometry(K_NULL),
 	_geometryInstance(K_NULL)
{
	setWindowTitle(tr("Create Shape"));

	QVBoxLayout* vLayout = new QVBoxLayout;

	_fLayout = new QFormLayout;
	const QList<const GeneratorParameter*> params = generator->parameters();
	for(kint i=0; i<params.size(); i++)
	{
		const GeneratorParameter* param = params.at(i);
		QWidget* widget = K_NULL;
		switch(param->defaultValue().type())
		{
			case QVariant::Int:
				widget = new QSpinBox(this);
				((QSpinBox*)widget)->setValue(param->defaultValue().toInt());
				//((QSpinBox*)widget)->setMaximum(param->maximum().toInt());
				((QSpinBox*)widget)->setMinimum(param->minimum().toInt());
				break;
			case QVariant::Double:
				widget = new QDoubleSpinBox(this);
				((QDoubleSpinBox*)widget)->setValue(param->defaultValue().toDouble());
				//((QDoubleSpinBox*)widget)->setMaximum(param->maximum().toDouble());
				((QDoubleSpinBox*)widget)->setMinimum(param->minimum().toDouble());
				break;
			default:
				break;
		}
		if(widget)
		{
			QString title = QString(tr("%1:")).arg(param->name());
			_fLayout->addRow(title, widget);
		}
	}
	vLayout->addLayout(_fLayout);

	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->addStretch();

	QString label = QString(tr("Create %1")).arg(generator->shape()->blockName());
	hLayout->addWidget(_buttonCreate = new QPushButton(label));
	connect(_buttonCreate, SIGNAL(clicked(bool)), SLOT(create()));

	QPushButton* button = new QPushButton(tr("Cancel"));
	connect(button, SIGNAL(clicked(bool)), SLOT(reject()));
	hLayout->addWidget(button);

	vLayout->addLayout(hLayout);

	setLayout(vLayout);
}

bool GenerateShape::createShape()
{
	return exec() == QDialog::Accepted;
}

Geometry* GenerateShape::geometry()
{
	return _geometry;
}

GeometryInstance* GenerateShape::geometryInstance()
{
	return _geometryInstance;
}

void GenerateShape::create()
{
	if(_geometry == K_NULL)
	{
		_geometry = K_BLOCK_CREATE_INSTANCE(Nigel::geometry::Geometry);
		_geometry->blockName(_generator->shape()->blockName());
		QVariantMap params = Generator::CreateDefaultParametersList(_generator);
		QVariantMap::iterator i = params.begin();
		while(i != params.end())
		{
			for(kint j=0; j<_fLayout->rowCount(); j++)
			{
				QString name = QString(tr("%1:")).arg(i.key());
				if(name.compare(((QLabel*)_fLayout->itemAt(j, QFormLayout::LabelRole)->widget())->text()) == 0)
				{
					switch(i.value().type())
					{
						case QVariant::Int:
							i.value().setValue(((QSpinBox*)_fLayout->itemAt(j, QFormLayout::FieldRole)->widget())->value());
							break;
						case QVariant::Double:
							i.value().setValue(((QDoubleSpinBox*)_fLayout->itemAt(j, QFormLayout::FieldRole)->widget())->value());
							break;
						default:
							break;
					}
				}
			}
			++i;
		}
		_geometry->addBlock(_generator->generatePrimitive(params));
	}

	if(_geometryInstance == K_NULL)
	{
		_geometryInstance = ElementInstance::CreateElementInstance<GeometryInstance>(_geometry);
	}

	accept();
}
