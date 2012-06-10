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

#pragma once

#include <PantinExport.hpp>

#include <Types.hpp>

#include <parallel/Tasklet.hpp>

#include <serialization/3ds/3dsImportTasklet.hpp>

#include <data/Block.hpp>

#include <QtGui/QWizardPage>
#include <QtGui/QProgressBar>
#include <QtGui/QTextEdit>
#include <QtGui/QTableWidget>
#include <QtGui/QCheckBox>

namespace Pantin { namespace serialization { namespace ThreeDS {

class PantinExport GetObjectsProgressPage : public QWizardPage
{
    Q_OBJECT

public:
    GetObjectsProgressPage();
    virtual ~GetObjectsProgressPage();

protected:
    virtual void initializePage();
    virtual bool validatePage();
	virtual bool isComplete() const;
	virtual int nextId() const;

private slots:
	void ended(kint status);
	void progress(const QString& message);
	void progress(kuint64 progress, kuint64 total);

private:
	QProgressBar* _progress;
	QTextEdit* _text;
	kint _ended;
};



class PantinExport ObjectsChoicesPage : public QWizardPage
{
    Q_OBJECT

public:
    ObjectsChoicesPage();

protected:
    virtual void initializePage();
	virtual bool validatePage();
	virtual int nextId() const;

private:
	void uncheckAll();
	void checkAll();

private slots:
	void selectionChanged(int state);

private:
	QTableWidget* _table;
	QCheckBox* _checkbox;
};



class PantinExport LoadObjectsProgressPage : public QWizardPage
{
    Q_OBJECT

public:
    LoadObjectsProgressPage();
    virtual ~LoadObjectsProgressPage();

protected:
    virtual void initializePage();
    virtual bool validatePage();
    virtual bool isComplete() const;
	virtual int nextId() const;

private slots:
	void ended(kint status);
	void progress(const QString& message);
	void progress(kuint64 progress, kuint64 total);

private:
	ThreeDSImportTasklet* _tasklet;
	QProgressBar* _progress;
	QTextEdit* _text;
	kint _ended;
};

}}}
