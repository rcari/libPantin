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

#pragma once

#include <PantinExport.hpp>

#include <serialization/Serializer.hpp>
#include <project/Project.hpp>

#include <Types.hpp>

#include <QtGui/QWizard>
#include <QtGui/QWizardPage>
#include <QtGui/QLineEdit>

#include <QtCore/QFile>

namespace Pantin { namespace serialization {

class PantinExport FileImporterWizard : public QWizard
{
	Q_OBJECT

public:
	enum
	{
		SelectFilePageId = K_NULL,
		SuccessPageId,
		ErrorPageId,
		FirstSerializerPageId
	};

public:
	FileImporterWizard(QWidget* parent = K_NULL);

	void setFile(QString filename);
	QFile* file();

	void setContext(kvoid* context);
	kvoid* context();

private:
	QFile _file;
	kvoid* _context;
};



class PantinExport SelectFilePage : public QWizardPage
{
    Q_OBJECT

public:
    SelectFilePage();

protected:
    virtual void initializePage();
    virtual bool isComplete() const;
	virtual bool validatePage();
	virtual int nextId() const;

private slots:
	void pickFile();
	void checkFile();

private:
	QLineEdit* _filePath;
	mutable const Pantin::serialization::Serializer* _selectedSerializer;
};



class PantinExport SuccessPage : public QWizardPage
{
    Q_OBJECT

public:
    SuccessPage();

protected:
    virtual void initializePage();
    virtual bool validatePage();
	virtual int nextId() const;
};

class PantinExport ErrorPage : public QWizardPage
{
    Q_OBJECT

public:
    ErrorPage();

protected:
    virtual void initializePage();
    virtual bool validatePage();
	virtual int nextId() const;
};

}}
