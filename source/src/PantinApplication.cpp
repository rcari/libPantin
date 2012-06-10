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

#include <PantinApplication.hpp>
#include <PantinEngine.hpp>
using namespace Pantin;

#include <KoreApplication.hpp>
using namespace Kore;

#include <QtCore/QSettings>
#include <QtGui/QSplashScreen>

#include <windows/MainWindow.hpp>
using namespace Gooey::windows;

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QApplication>

PantinApplication::PantinApplication(QSplashScreen& splash) {

	qDebug("Pantin / Loading PantinApplication");

	// Only one instance is allowed !!!
	K_ASSERT( _Instance == K_NULL )

	_Instance = this;

	// Apply Pantin stylesheet
	{
		QFile file(":/pantin/text/stylesheet.css");
		file.open(QIODevice::ReadOnly);
		QTextStream stream(&file);
		QString css = qApp->styleSheet();
		css.append('\n');
		css.append(stream.readAll());
		qApp->setStyleSheet(css);
		file.close();
	}

	// Create Pantin's Internals
	_pantinLib = new Library(Block::System);
	_pantinLib->blockName("Pantin Internals");

	const_cast<Library*>(kApp->rootLibrary())->insertBlock(_pantinLib, kApp->dataLibrary()->index());

	_pantinLib->addBlock(PantinEngine::Instance());

	// Create the MainWindow
	PantinEngine::Instance()->createMainWindow();

	// And finish the splash with it !
	splash.finish(PantinEngine::MainWindow());

	PantinEngine::MainWindow()->setVisible(true);
	PantinEngine::MainWindow()->raise();

	// Restore the previously opened project if any !
	PantinEngine::Instance()->loadLastOpenedProject();
}

PantinApplication::~PantinApplication()
{
	qDebug("Pantin / Unloading PantinApplication");
	_pantinLib->destroy();
}

Library* PantinApplication::pantinLibrary()
{
	return _pantinLib;
}

PantinApplication* PantinApplication::Instance() {
	return _Instance;
}

QString PantinApplication::Version()
{
	return QString(_PANTIN_VERSION);
}

PantinApplication* PantinApplication::_Instance = K_NULL;
