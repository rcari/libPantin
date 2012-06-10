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

#include <serialization/FileImporterWizard.hpp>
using namespace Pantin::serialization;

#include <PantinEngine.hpp>
using namespace Pantin;

#include <data/LibraryT.hpp>
using namespace Kore::data;

#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtGui/QDesktopServices>
#include <QtGui/QFormLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

#include <QtCore/QFileInfo>

FileImporterWizard::FileImporterWizard(QWidget* parent)
:	QWizard(parent),
 	_context(K_NULL)
{
	setPage(FileImporterWizard::SelectFilePageId, new SelectFilePage);
	setPage(FileImporterWizard::SuccessPageId, new SuccessPage);
	setPage(FileImporterWizard::ErrorPageId, new ErrorPage);

	setWindowTitle(tr("File Importer Wizard"));

	QList<QWizard::WizardButton> layout;
	layout << QWizard::Stretch << QWizard::CancelButton << QWizard::NextButton << QWizard::FinishButton;
	setButtonLayout(layout);
}

void FileImporterWizard::setFile(QString filename)
{
	_file.setFileName(filename);
}

QFile* FileImporterWizard::file()
{
	return &_file;
}

void FileImporterWizard::setContext(kvoid* context)
{
	_context = context;
}

kvoid* FileImporterWizard::context()
{
	return _context;
}



SelectFilePage::SelectFilePage()
:	QWizardPage(),
 	_selectedSerializer(K_NULL)
{
	setTitle(tr("File to import:"));

	QVBoxLayout* vLayout = new QVBoxLayout;
	QFormLayout* fileLayout = new QFormLayout;
	QPushButton* button;

	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->addWidget(_filePath = new QLineEdit);
	_filePath->setReadOnly(true);
	hLayout->addWidget(button = new QPushButton);
	button->setIcon(QIcon(":/gooey/images/icons/global.file.open.png"));
	connect(button, SIGNAL(clicked(bool)), SLOT(pickFile()));

	fileLayout->addRow(tr("Location:"), hLayout);
	vLayout->addLayout(fileLayout);
	vLayout->addStretch();

	setLayout(vLayout);
}

void SelectFilePage::initializePage()
{
	_filePath->setText(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
	connect(_filePath, SIGNAL(textChanged(const QString&)), SLOT(checkFile()));
}

bool SelectFilePage::isComplete() const
{
	//find if the path is a file
	QFileInfo info(_filePath->text());
	if(info.exists() && info.isFile())
	{
		if(_selectedSerializer && _selectedSerializer->containsExtension(info.completeSuffix()))
		{
			return true;
		}
	}
	return false;
}

bool SelectFilePage::validatePage()
{
	if(_selectedSerializer)
	{
		_selectedSerializer->addWizardSteps(wizard());
		((FileImporterWizard*)wizard())->setFile(_filePath->text());
		return true;
	}
	return false;
}

int SelectFilePage::nextId() const
{
	if(_selectedSerializer)
	{
		return _selectedSerializer->getFirstWizardStep();
	}
	return K_NULL;
}

void SelectFilePage::pickFile()
{
	_selectedSerializer = K_NULL;

	//set filters
	kbool firstSerializer = true;
	QString filters;
	for(kint i=K_NULL; i<PantinEngine::Serializers().size(); i++)
	{
		const Serializer* serializer = PantinEngine::Serializers().constAt(i);
		if(serializer->canImport())
		{
			if(!firstSerializer)
			{
				//separator between filters
				filters.append(";;");
			}
			filters.append(serializer->blockName());
			filters.append(" ( ");
			QStringList extensions = serializer->getSupportedExtensions();
			for(kint j=K_NULL; j<extensions.size(); j++)
			{
				filters.append("*.");
				filters.append(extensions.at(i));
				filters.append(" ");
			}
			filters.append(")");
			firstSerializer = false;
		}
	}

	//choose the file to import
	QString filter;
	QString path = QFileDialog::getOpenFileName(
			this,
			tr("Pick a File"),
			_filePath->text(),
			filters,
			&filter,
			QFileDialog::DontUseNativeDialog
		);

	//search for selected filter
	for(kint i=K_NULL; i<PantinEngine::Serializers().size(); i++)
	{
		const Serializer* serializer = PantinEngine::Serializers().constAt(i);
		if(serializer->canImport())
		{
			QString name;
			name.append(serializer->blockName());
			name.append(" ( ");
			QStringList extensions = serializer->getSupportedExtensions();
			for(kint j=K_NULL; j<extensions.size(); j++)
			{
				name.append("*.");
				name.append(extensions.at(i));
				name.append(" ");
			}
			name.append(")");
			if(name.compare(filter) == K_NULL)
			{
				_selectedSerializer = serializer;
				break;
			}
		}
	}

	//set file selected
	_filePath->setText(path);
}

void SelectFilePage::checkFile()
{
	//check if the file can be imported
	emit completeChanged();
}



SuccessPage::SuccessPage()
:	QWizardPage()
{
	setTitle(tr("Import done"));

	QVBoxLayout* layout = new QVBoxLayout;
	QLabel* label = new QLabel(tr("Import is successful without any errors."));

	label->setWordWrap(true);
	layout->addWidget(label);

	setLayout(layout);
}

void SuccessPage::initializePage()
{
	// Disable cancel button for this step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(false);
	}
}

bool SuccessPage::validatePage()
{
	// Re-Enable cancel button for the next step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(true);
	}
	return true;
}

int SuccessPage::nextId() const
{
	return -1;
}

ErrorPage::ErrorPage()
:	QWizardPage()
{
	setTitle(tr("Import error"));

	QVBoxLayout* layout = new QVBoxLayout;
	QLabel* label = new QLabel(tr("An error occurs during the process of importing the 3ds file."));

	label->setWordWrap(true);
	layout->addWidget(label);

	setLayout(layout);
}

void ErrorPage::initializePage()
{
	// Disable cancel button for this step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(false);
	}
}

bool ErrorPage::validatePage()
{
	// Re-Enable cancel button for the next step
	QAbstractButton* cancel = wizard()->button(QWizard::CancelButton);
	if(cancel)
	{
		cancel->setEnabled(true);
	}
	return true;
}

int ErrorPage::nextId() const
{
	return -1;
}
