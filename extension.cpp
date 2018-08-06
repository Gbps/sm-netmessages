/**
 * -----------------------------------------------------
 * File        extension.cpp
 * Authors     David Ordnung
 * License     GPLv3
 * Web         http://dordnung.de
 * -----------------------------------------------------
 * 
 * Copyright (C) 2014 David Ordnung
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 */


#include "extension.h"
#include "msvc13/UserMessagePBHelpers.h"
#include "msvc13/protobuf/netmessages.pb.h"
#include <memory>
#include "msvc13/CSmObject.h"

using namespace SourceModNetMessages;

extern sp_nativeinfo_t bitbuf_natives[];

/**
 * This is called after the initial loading sequence has been processed.
 *
 * @param error        Error message buffer.
 * @param maxlength    Size of error message buffer.
 * @param late         Whether or not the module was loaded after map load.
 * @return             True to succeed loading, false to fail.
 */
bool NetMessagesExtension::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	sharesys->AddNatives(myself, bitbuf_natives);
	sharesys->AddNatives(myself, protobuf_natives);

	/* Now register the extension */
	sharesys->RegisterLibrary(myself, "smnetmessage");

	if (!CSmObject<bf_write>::InitHandleType("NM_BfWrite", error, maxlength)) return false;
	if (!CSmObject<SMProtobufMessage>::InitHandleType("NM_Protobuf", error, maxlength)) return false;

	return true;
}

/**
 * This is called right before the extension is unloaded.
 */
void NetMessagesExtension::SDK_OnUnload()
{
	CSmObject<bf_write>::RemoveHandleType();
	CSmObject<SMProtobufMessage>::RemoveHandleType();
}

/* Linking extension */
NetMessagesExtension g_NetMessagesExtension;
SMEXT_LINK(&g_NetMessagesExtension);