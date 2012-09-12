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

#include <data/LibraryT.hpp>
#include <windows/MainWindow.hpp>
#include <windows/Perspective.hpp>
#include <PantinInstancesManager.hpp>

#include <rendering/Renderer.hpp>

#include <QtGui/QMenu>
#include <QtGui/QUndoStack>

namespace Pantin {

namespace serialization { class Serializer; }

class PantinApplication;

class PantinExport PantinEngine : public Kore::data::Library
{
	Q_OBJECT

	friend class PantinApplication;
	friend class PantinInstancesManager;

private:
	PantinEngine();

public:
	static Gooey::windows::MainWindow* MainWindow();

	static kbool HasActiveProject();

	static GLr::rendering::Renderer* InstanceRenderer(Nigel::COLLADA::DocumentInstance* instance);

	// XXX: Deprecated !!! To be removed ASAP
	static PantinInstancesManager* ActiveManager();

	static QUndoStack* InstanceUndoStack(Nigel::COLLADA::DocumentInstance* instance);

	QMenu* importMenu();

	static kbool RegisterSerializer(Pantin::serialization::Serializer*);
	static const Kore::data::LibraryT<Pantin::serialization::Serializer>& Serializers();
	static kbool existsSerializerWithName(const QString& name);

	static void LoadProject(const QFileInfo& fileInfo, QWidget* parent);

private:
	void createMainWindow();

	void registerInstancesManager(PantinInstancesManager* manager);
	void unregisterInstancesManager(PantinInstancesManager* manager);

	void unloadProject();
	kbool close();

	void loadLastOpenedProject();
	void saveLastOpenedProject();

private slots:
	void updateStyleSheet();
	void quitRequest();
	void error(QString error, QString details);

	void create();
	void open();
	kbool save();
	void about();
	void import();

	void cleanChanged(bool clean);

signals:
	void projectLoaded();
	void projectUnloaded();
	void saveEnabled(bool);

private:
	Gooey::windows::MainWindow* _mainWindow;
	PantinInstancesManager* _manager;
	QMenu* _projectMenu;
	QMenu* _importMenu;
	QMenu* _exportMenu;

	Kore::data::LibraryT<Pantin::serialization::Serializer> _serializers;
	Kore::data::LibraryT<Gooey::windows::Perspective> _perspectives;

public:
	static PantinEngine* Instance();
private:
	static PantinEngine* _Instance;
};

}

#define P_SERIALIZER_I( serializer ) kbool serializer::__P_Registered = Pantin::PantinEngine::RegisterSerializer( serializer::StaticMetaBlock()->instantiateT<Pantin::serialization::Serializer>() );
