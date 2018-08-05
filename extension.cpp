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
#include "clientlistener.h"
#include "msvc13/bitbuf.h"
#include "msvc13/UserMessagePBHelpers.h"
#include "msvc13/protobuf/netmessages.pb.h"
#include <memory>

IForward* g_hReceived;
IForward* g_hRequested;
IForward* g_hDenied;
IForward* g_hSent;

// Natives
sp_nativeinfo_t filenetmessages_natives[] =
{
	{"FNM_SendFile",    FileNetMessages_SendFile},
	{"FNM_RequestFile", FileNetMessages_RequestFile},
	{"NM_GetNetChannelWriteBuf", NetMessages_GetNetChannelWriteBuf},
	{"NM_SendDataToPlayer", NetMessages_SendDataToPlayer},
	{"NM_CreateProtobufMessage", NetMessages_CreateProtobufMessage },
	{"NM_SendProtobufToPlayer", NetMessages_SendProtobufToPlayer},
	{NULL, NULL}
};

#if SOURCE_ENGINE >= SE_EYE
#define NETMSG_BITS 6
#else
#define NETMSG_BITS 5
#endif

#if SOURCE_ENGINE >= SE_LEFT4DEAD
#define NET_SETCONVAR	6
#else
#define NET_SETCONVAR	5
#endif

extern HandleType_t g_WrBitBufType = NO_HANDLE_TYPE;

extern HandleType_t g_RdBitBufType = NO_HANDLE_TYPE;


// Current bitbuf message handle
Handle_t g_CurMsgHandle;

// Data for the current bf_write
char g_CurBitBufBuffer[2048];

// The bitbuf that we'll give for a user on request
bf_write g_CurBitBufBufferObj(g_CurBitBufBuffer, sizeof(g_CurBitBufBuffer));

BitBufferHandleHandler handler;
ProtobufBufferHandleHandler protohandler;

/**
 * This is called after the initial loading sequence has been processed.
 *
 * @param error        Error message buffer.
 * @param maxlength    Size of error message buffer.
 * @param late         Whether or not the module was loaded after map load.
 * @return             True to succeed loading, false to fail.
 */
bool FileNetMessagesExtension::SDK_OnLoad(char *error, size_t err_max, bool late)
{
	/* Add client listener */
	//playerhelpers->AddClientListener(&g_hClientListener);

	/* Add the natives */
	sharesys->AddNatives(myself, filenetmessages_natives);
	sharesys->AddNatives(myself, bitbuf_natives);
	sharesys->AddNatives(myself, protobuf_natives);

	/* Add the forwards */
	g_hReceived = forwards->CreateForward("FNM_OnFileReceived", ET_Ignore, 3, NULL, Param_Cell, Param_String, Param_Cell);
	g_hRequested = forwards->CreateForward("FNM_OnFileRequested", ET_Event, 3, NULL, Param_Cell, Param_String, Param_Cell);
	g_hDenied = forwards->CreateForward("FNM_OnFileDenied", ET_Ignore, 3, NULL, Param_Cell, Param_String, Param_Cell);
	g_hSent = forwards->CreateForward("FNM_OnFileSent", ET_Ignore, 3, NULL, Param_Cell, Param_String, Param_Cell);

	/* Now register the extension */
	sharesys->RegisterLibrary(myself, "filenetmessages");

	HandleAccess sec;

	handlesys->InitAccessDefaults(NULL, &sec);
	sec.access[HandleAccess_Delete] = HANDLE_RESTRICT_IDENTITY;

	HandleError err;
	/* Get BitBuf type */
	g_WrBitBufType = handlesys->CreateType("NM_BfWrite", &handler, 0, NULL, NULL, myself->GetIdentity(), &err);
	g_RdBitBufType = handlesys->CreateType("NM_BfRead", &handler, 0, NULL, &sec, myself->GetIdentity(), &err);
	g_ProtobufType = handlesys->CreateType("NM_Protobuf", &handler, 0, NULL, NULL, myself->GetIdentity(), NULL);

	/* LOADED :) */
	return true;
}


/**
 * This is called right before the extension is unloaded.
 */
void FileNetMessagesExtension::SDK_OnUnload()
{
	playerhelpers->RemoveClientListener(&g_hClientListener);

	forwards->ReleaseForward(g_hReceived);
	forwards->ReleaseForward(g_hRequested);
	forwards->ReleaseForward(g_hDenied);
	forwards->ReleaseForward(g_hSent);

	handlesys->RemoveType(g_WrBitBufType, myself->GetIdentity());
	handlesys->RemoveType(g_RdBitBufType, myself->GetIdentity());
	handlesys->RemoveType(g_ProtobufType, myself->GetIdentity());

	g_hClientListener.Shutdown();
}

/**
* @brief Called when destroying a handle.  Must be implemented.
*
* @param type		Handle type.
* @param object	Handle internal object.
*/
void BitBufferHandleHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	Msg("OnHandleDestroy\n");
}

/**
* @brief Called to get the size of a handle's memory usage in bytes.
* Implementation is optional.
*
* @param type		Handle type.
* @param object	Handle internal object.
* @param pSize		Pointer to store the approximate memory usage in bytes.
* @return			True on success, false if not implemented.
*/
bool BitBufferHandleHandler::GetHandleApproxSize(HandleType_t type, void *object, unsigned int *pSize)
{
	return false;
}

/**
* @brief Called when destroying a handle.  Must be implemented.
*
* @param type		Handle type.
* @param object	Handle internal object.
*/
void ProtobufBufferHandleHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	delete static_cast<SMProtobufMessage*>(object);
}

/**
* @brief Called to get the size of a handle's memory usage in bytes.
* Implementation is optional.
*
* @param type		Handle type.
* @param object	Handle internal object.
* @param pSize		Pointer to store the approximate memory usage in bytes.
* @return			True on success, false if not implemented.
*/
bool ProtobufBufferHandleHandler::GetHandleApproxSize(HandleType_t type, void *object, unsigned int *pSize)
{
	return false;
}




// Send a file
cell_t FileNetMessages_SendFile(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	char path[PLATFORM_MAX_PATH + 1];

	smutils->FormatString(path, sizeof(path), pContext, params, 2);
	

	if (client <= 0 || client > playerhelpers->GetMaxClients())
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}

	INetChannel *pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));

	if (!pNetChan)
	{
		return pContext->ThrowNativeError("Client %i has no valid NetChannel!", client);
	}

	static unsigned int transferID = 0;

	#if SOURCE_ENGINE < SE_CSGO
		return pNetChan->SendFile(path, ++transferID) ? transferID : 0;
	#else
		return pNetChan->SendFile(path, ++transferID, false) ? transferID : 0;
	#endif
}



// Request a file
cell_t FileNetMessages_RequestFile(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	char path[PLATFORM_MAX_PATH + 1];

	smutils->FormatString(path, sizeof(path), pContext, params, 2);


	if (client <= 0 || client > playerhelpers->GetMaxClients())
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}

	INetChannel *pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));

	if (!pNetChan)
	{
		return pContext->ThrowNativeError("Client %i has no valid NetChannel!", client);
	}

	#if SOURCE_ENGINE < SE_CSGO
		return pNetChan->RequestFile(path);
	#else
		return pNetChan->RequestFile(path, false);
	#endif
}

// Get player's netchannel
cell_t NetMessages_GetNetChannelWriteBuf(IPluginContext *pContext, const cell_t *params)
{
	auto client = params[1];
	auto netmsgnum = params[2];
	auto pPlayer = playerhelpers->GetGamePlayer(client);
	if (pPlayer &&  pPlayer->IsInGame() && !pPlayer->IsFakeClient())
	{
		auto* pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));
		if (!pNetChan)
		{
			return pContext->ThrowNativeError("Could not get net channel for client.");
		}

		g_CurBitBufBufferObj.Reset();

		g_CurBitBufBufferObj.WriteUBitLong(netmsgnum, NETMSG_BITS);

		HandleError out;
		g_CurMsgHandle = handlesys->CreateHandle(g_WrBitBufType, &g_CurBitBufBufferObj, NULL, myself->GetIdentity(), &out);

		if (g_CurMsgHandle == 0)
		{
			pContext->ThrowNativeError("Could not create handle.");
		}
		return g_CurMsgHandle;
	}
	else
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}
}

// Get player's netchannel
cell_t NetMessages_StartNetChannelProtobuf(IPluginContext *pContext, const cell_t *params)
{
	auto client = params[1];

	// Grab the message name
	char *netMessageName;
	pContext->LocalToString(params[3], &netMessageName);
}

cell_t NetMessages_SendDataToPlayer(IPluginContext *pContext, const cell_t *params)
{
	const auto client = params[1];
	const auto hndl = static_cast<Handle_t>(params[2]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	auto pPlayer = playerhelpers->GetGamePlayer(client);
	if (pPlayer &&  pPlayer->IsInGame() && !pPlayer->IsFakeClient())
	{
		auto* pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));
		if (!pNetChan)
		{
			return pContext->ThrowNativeError("Could not get net channel for client.");
		}

		pNetChan->SendData(*pBitBuf, true);

		return g_CurMsgHandle;
	}
	else
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}
}


cell_t NetMessages_CreateProtobufMessage(IPluginContext *pContext, const cell_t *params)
{
	using namespace google::protobuf;

	char *messageName;
	pContext->LocalToPhysAddr(params[1], (cell_t **)&messageName);

	// Find the descriptor by name
	auto* msg_desc =
		DescriptorPool::generated_pool()
		->FindMessageTypeByName(messageName);

	if(!msg_desc)
	{
		return pContext->ThrowNativeError("Invalid NetMessage name '%s'!", messageName);
	}

	// Create an instance of the message, unpopulated
	auto* newMessage = 
		MessageFactory::generated_factory()
		->GetPrototype(msg_desc)
		->New();

	if(!newMessage)
	{
		return pContext->ThrowNativeError("Could not create NetMessage '%s'!", messageName);
	}

	// Create a handle for the new message
	auto outHndl = handlesys->CreateHandle(g_ProtobufType, new SMProtobufMessage(newMessage), NULL, myself->GetIdentity(), NULL);

	return outHndl;
}

int ProtobufNetMessageNameToId(std::string& messageName)
{
	using namespace google::protobuf;
	// Net messages have a name like CNETMsg_NOP
	// Convert to the Enum name net_NOP then use protobuf reflection to grab the message id

	std::string message;
	if (messageName.compare(0, 3, "CNET"))
	{
		message = "net";
	}
	else if (messageName.compare(0, 3, "CCLC"))
	{
		message = "clc";
	}
	else if (messageName.compare(0, 3, "CSVC"))
	{
		message = "svc";
	}
	else
	{
		return -1;
	}

	// Grab the position of the first underscore, which is where the prefix must be changed
	const auto found = messageName.find('_');
	if(found == std::string::npos)
	{
		return -1;
	}

	// generate the name like net_NOP
	auto enumName = message + messageName.substr(found);

	// Look for it in the list of enums
	const auto enum_desc =
		DescriptorPool::generated_pool()->FindEnumValueByName(enumName);

	if(!enum_desc)
	{
		return -1;
	}

	// The actual msgId
	const auto msgId = enum_desc->number();

	return msgId;
}

cell_t NetMessages_SendProtobufToPlayer(IPluginContext *pContext, const cell_t *params)
{
	const auto client = params[1];
	const auto hndl = static_cast<Handle_t>(params[2]);
	HandleError herr;
	HandleSecurity sec;
	SMProtobufMessage *pNetMessage;

	sec.pOwner = NULL;                                
	sec.pIdentity = myself->GetIdentity();            
	if ((herr = handlesys->ReadHandle(hndl, g_ProtobufType, &sec, (void **)&pNetMessage)) 
		!= HandleError_None)                          
	{                                                 
		return pContext->ThrowNativeError("Invalid protobuf message handle %x (error %d)", hndl, herr); \
	}

	auto pPlayer = playerhelpers->GetGamePlayer(client);
	if (pPlayer &&  pPlayer->IsInGame() && !pPlayer->IsFakeClient())
	{
		auto* pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));
		if (!pNetChan)
		{
			return pContext->ThrowNativeError("Could not get net channel for client.");
		}

		// Grab the protobuf::Message pointer
		auto* msg = pNetMessage->GetProtobufMessage();

		// Get the total size of the serialized message
		auto msgSize = msg->ByteSize();

		// Temp buffers
		auto msg_buf = std::make_unique<uint8[]>(msgSize);

		// Bitbuffer that will contain everything needed to send the packet
		memset(g_CurBitBufBuffer, 0, sizeof(g_CurBitBufBuffer));
		g_CurBitBufBufferObj.SeekToBit(0);


		// Actually works because messages are defined in order in the .proto file
		auto msg_type_name = msg->GetTypeName();
		auto msgId = ProtobufNetMessageNameToId(msg_type_name);

		if(msgId == -1)
		{
			return pContext->ThrowNativeError("Protobuf %s cannot be sent as it is not a NetMessage (not CSVCMsg, CCLCMsg, or CNETMsg)", msg_type_name.c_str());
		}
		// Write the message id number (same in other source games)
		g_CurBitBufBufferObj.WriteVarInt32(msgId);

		// Size of the message calculated from protobuf
		g_CurBitBufBufferObj.WriteVarInt32(msgSize);

		// Serialize the message to a temp buffer
		msg->SerializeWithCachedSizesToArray(msg_buf.get());

		// Write the temp buffer back to the bitbuffer
		g_CurBitBufBufferObj.WriteBytes(msg_buf.get(), msgSize);

		// Send the packet reliably
		pNetChan->SendData(g_CurBitBufBufferObj, true);

		return true;
	}
	else
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}
}


/* Linking extension */
FileNetMessagesExtension g_FileNetMessagesExtension;
SMEXT_LINK(&g_FileNetMessagesExtension);