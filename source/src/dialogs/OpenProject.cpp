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

#include <dialogs/OpenProject.hpp>
using namespace Pantin::dialogs;

#include <PantinInstancesManager.hpp>
#include <PantinEngine.hpp>
using namespace Pantin;

#include <tasklets/LoadInstance.hpp>
using namespace Pantin::tasklets;

#include <project/ProjectConstants.hpp>

#include <KoreApplication.hpp>
#include <KoreEngine.hpp>
using namespace Kore;

#include <windows/TaskletProgressDialog.hpp>
using namespace Gooey::windows;

#include <QtGui/QtGui>

OpenProject::OpenProject(QWidget* parent)
:	QDialog(parent)
{
	setWindowTitle(tr("Open Project"));

	QStackedLayout* sLayout = new QStackedLayout;

	QVBoxLayout* vLayout = new QVBoxLayout;

	_fileDialog = new QFileDialog;
	_fileDialog->setOption(QFileDialog::DontUseNativeDialog);
	connect(_fileDialog, SIGNAL(accepted()), SLOT(loadProject()));
	connect(_fileDialog, SIGNAL(rejected()), SLOT(reject()));

	_fileDialog->setFilter(tr("Pantin Project file (*%1)").arg(N_PROJECT_FILE_EXTENSION));

	vLayout->addWidget(_fileDialog);

	// TODO: Make something useful of this option.
	vLayout->addWidget(new QCheckBox(tr("Load everything at once (no lazy loading)")));

	QWidget* firstPanel = new QWidget;
	firstPanel->setLayout(vLayout);
	sLayout->addWidget(firstPanel);

	setLayout(sLayout);
}

bool OpenProject::openProject()
{
	return exec() == QDialog::Accepted;
}

void OpenProject::loadProject()
{
	// Do not hide this !
	_fileDialog->show();

	QFileInfo fileInfo(_fileDialog->selectedFiles().first());

	LoadInstance loadTasklet(fileInfo.absoluteFilePath());

	PantinEngine::LoadProject(fileInfo, this);

	// We are done !
	accept();
}
