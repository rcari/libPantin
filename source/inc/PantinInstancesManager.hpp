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

#include <rendering/Renderer.hpp>


#include <COLLADA/InstancesManager.hpp>

#include <QtGui/QUndoGroup>
#include <QtGui/QUndoStack>

namespace Pantin {

/*!
 * @brief The Pantin instances manager
 *
 * The PantinInstancesManager is an InstancesManager specifically tailored for the needs
 * of Pantin. Its main purpose is mostly to add an undo stack for each DocumentInstance and
 * to allow easy/fast retrieval of that. It also provides improved serialization operations
 * with GUI presentation of what is happening.
 */
class PantinExport PantinInstancesManager : public Nigel::COLLADA::InstancesManager
{
	Q_OBJECT
	//Q_PROPERTY( QString displayName READ displayName DESIGNABLE false USER true )
	K_BLOCK

protected:
	PantinInstancesManager();

	virtual void library(Library* library);

public:
	~PantinInstancesManager();

	GLr::rendering::Renderer* renderer();

	/*!
	 * Editors should cache the return value of this as it requires a hashtable lookup.
	 */
	QUndoStack* instanceUndoStack(const Nigel::COLLADA::DocumentInstance* instance) const;

	QUndoGroup* undoGroup();

	kbool isClean() const;

	void updateInstanceAssetLastModified(Nigel::COLLADA::DocumentInstance*);

protected:
	virtual void registerDocumentInstance(Nigel::COLLADA::DocumentInstance*);
	virtual void unregisterDocumentInstance(Nigel::COLLADA::DocumentInstance*);

	virtual Nigel::COLLADA::DocumentInstance* loadInstance(const QFileInfo& file);
	virtual void saveInstance(const Nigel::COLLADA::DocumentInstance* instance) const;

signals:
	void dirtyInstances(bool);

private slots:
	void currentStackIndexChanged();

private:
	QString displayName() const;

private:
	GLr::rendering::Renderer* _renderer;
	QString _runtimeID;
	QUndoGroup _undoGroup;
};

}
