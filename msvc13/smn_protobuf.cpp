/**
* vim: set ts=4 :
* =============================================================================
* SourceMod
* Copyright (C) 2013 AlliedModders LLC.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, AlliedModders LLC gives you permission to link the
* code of this program (as well as its derivative works) to "Half-Life 2," the
* "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, AlliedModders LLC grants
* this exception to all derivative works.  AlliedModders LLC defines further
* exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
* or <http://www.sourcemod.net/license.php>.
*
* Version: $Id$
*/
#pragma warning( push )
#pragma warning( disable : 4005)

#define MAX_SPLITSCREEN_PLAYERS 1

#include "UserMessagePBHelpers.h"
#include <IHandleSys.h>
#include "CSmProtobuf.h"
#include "CSMBitBuf.h"

#pragma warning( pop ) 

using namespace SourceModNetMessages;

// Assumes pbuf message handle is param 1, gets message as msg
#define GET_MSG_FROM_HANDLE_OR_ERR()                  \
	Handle_t hndl = static_cast<Handle_t>(params[1]); \
	HandleError herr;                                 \
	HandleSecurity sec;                               \
	CSmProtobuf *pmsg;                                 \
	                                                  \
	sec.pOwner = NULL;                                \
	sec.pIdentity = myself->GetIdentity();            \
	                                                  \
	if ((herr=handlesys->ReadHandle(hndl, CSmProtobuf::HandleType, &sec, (void **)&pmsg)) \
		!= HandleError_None)                          \
	{                                                 \
		return pCtx->ThrowNativeError("Invalid protobuf message handle %x (error %d)", hndl, herr); \
	} \
	SMProtobufMessage* msg = pmsg->Get();

// Assumes message field name is param 2, gets as strField
#define GET_FIELD_NAME_OR_ERR()                                           \
	char *strField;                                                       \
	pCtx->LocalToString(params[2], &strField);


cell_t NM_CreateProtobufMessage(IPluginContext *pContext, const cell_t *params)
{
	using namespace google::protobuf;

	char *messageName;
	pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t **>(&messageName));

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
	auto* newMsg = new CSmProtobuf(newMessage);
	auto outHndl = newMsg->CreateHandle(pContext);

	return outHndl;
}

int ProtobufNetMessageNameToId(std::string& messageName)
{
	using namespace google::protobuf;
	// Net messages have a name like CNETMsg_NOP
	// Convert to the Enum name net_NOP then use protobuf reflection to grab the message id

	std::string message;
	if (messageName.compare(0, 4, "CNET") == 0)
	{
		message = "net";
	}
	else if (messageName.compare(0, 4, "CCLC") == 0)
	{
		message = "clc";
	}
	else if (messageName.compare(0, 4, "CSVC") == 0)
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
	const auto enumName = message + messageName.substr(found);

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

cell_t NM_SendProtobufToPlayer(IPluginContext *pContext, const cell_t *params)
{
	const auto client = params[2];

	auto nmobj = CSmProtobuf::FromHandle(params[1]);
	if(!nmobj)
	{
		return pContext->ThrowNativeError("Invalid Handle");
	}

	auto* pNetMessage = nmobj->Get();

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
		auto tempBuf = CSmBitBuf{ msgSize + 8 };
		auto* bitbuf = tempBuf.Get();

		// Actually works because messages are defined in order in the .proto file
		auto msg_type_name = msg->GetTypeName();
		auto msgId = ProtobufNetMessageNameToId(msg_type_name);

		if(msgId == -1)
		{
			return pContext->ThrowNativeError("Protobuf %s cannot be sent as it is not a NetMessage (not CSVCMsg, CCLCMsg, or CNETMsg)", msg_type_name.c_str());
		}
		// Write the message id number (same in other source games)
		bitbuf->WriteVarInt32(msgId);

		// Size of the message calculated from protobuf
		bitbuf->WriteVarInt32(msgSize);

		// Serialize the message to a temp buffer
		msg->SerializeWithCachedSizesToArray(msg_buf.get());

		// Write the temp buffer back to the bitbuffer
		bitbuf->WriteBytes(msg_buf.get(), msgSize);

		// Send the packet reliably
		pNetChan->SendData(*bitbuf, true);

		return true;
	}
	else
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}
}

static cell_t smn_PbReadInt(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	int ret;

	int index = params[0] >= 3 ? params[3] : -1;
	if (index < 0)
	{
		if (!msg->GetInt32OrUnsignedOrEnum(strField, &ret))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedInt32OrUnsignedOrEnum(strField, index, &ret))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return ret;
}

static cell_t smn_PbReadFloat(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	float ret;

	int index = params[0] >= 3 ? params[3] : -1;
	if (index < 0)
	{
		if (!msg->GetFloatOrDouble(strField, &ret))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedFloatOrDouble(strField, index, &ret))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return sp_ftoc(ret);
}

static cell_t smn_PbReadBool(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	bool ret;

	int index = params[0] >= 3 ? params[3] : -1;
	if (index < 0)
	{
		if (!msg->GetBool(strField, &ret))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedBool(strField, index, &ret))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return ret ? 1 : 0;
}

static cell_t smn_PbReadString(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	char *buf;
	pCtx->LocalToPhysAddr(params[3], (cell_t **)&buf);

	int index = params[0] >= 5 ? params[5] : -1;

	if (index < 0)
	{
		if (!msg->GetString(strField, buf, params[4]))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedString(strField, index, buf, params[4]))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbReadColor(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *out;
	pCtx->LocalToPhysAddr(params[3], &out);

	Color clr;
	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->GetColor(strField, &clr))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedColor(strField, index, &clr))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	out[0] = clr.r();
	out[1] = clr.g();
	out[2] = clr.b();
	out[3] = clr.a();

	return 1;
}

static cell_t smn_PbReadAngle(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *out;
	pCtx->LocalToPhysAddr(params[3], &out);

	QAngle ang;
	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->GetQAngle(strField, &ang))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedQAngle(strField, index, &ang))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	out[0] = sp_ftoc(ang.x);
	out[1] = sp_ftoc(ang.y);
	out[2] = sp_ftoc(ang.z);

	return 1;
}

static cell_t smn_PbReadVector(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *out;
	pCtx->LocalToPhysAddr(params[3], &out);

	Vector vec;
	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->GetVector(strField, &vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedVector(strField, index, &vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	out[0] = sp_ftoc(vec.x);
	out[1] = sp_ftoc(vec.y);
	out[2] = sp_ftoc(vec.z);

	return 1;
}

static cell_t smn_PbReadVector2D(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *out;
	pCtx->LocalToPhysAddr(params[3], &out);

	Vector2D vec;
	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->GetVector2D(strField, &vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->GetRepeatedVector2D(strField, index, &vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	out[0] = sp_ftoc(vec.x);
	out[1] = sp_ftoc(vec.y);

	return 1;
}

static cell_t smn_PbGetRepeatedFieldCount(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	int cnt = msg->GetRepeatedFieldCount(strField);
	if (cnt == -1)
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return cnt;
}

static cell_t smn_PbHasField(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	return msg->HasField(strField) ? 1 : 0;
}

static cell_t smn_PbSetInt(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetInt32OrUnsignedOrEnum(strField, params[3]))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedInt32OrUnsignedOrEnum(strField, index, params[3]))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbSetFloat(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetFloatOrDouble(strField, sp_ctof(params[3])))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedFloatOrDouble(strField, index, sp_ctof(params[3])))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbSetBool(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	bool value = (params[3] == 0 ? false : true);
	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetBool(strField, value))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedBool(strField, index, value))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbSetString(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	char *strValue;
	pCtx->LocalToString(params[3], &strValue);

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetString(strField, strValue))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedString(strField, index, strValue))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbSetColor(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *clrParams;
	pCtx->LocalToPhysAddr(params[3], &clrParams);

	Color clr(
		clrParams[0],
		clrParams[1],
		clrParams[2],
		clrParams[3]);

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetColor(strField, clr))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedColor(strField, index, clr))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbSetAngle(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *angParams;
	pCtx->LocalToPhysAddr(params[3], &angParams);

	QAngle ang(
		sp_ctof(angParams[0]),
		sp_ctof(angParams[1]),
		sp_ctof(angParams[2]));

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetQAngle(strField, ang))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedQAngle(strField, index, ang))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbSetVector(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *vecParams;
	pCtx->LocalToPhysAddr(params[3], &vecParams);

	Vector vec(
		sp_ctof(vecParams[0]),
		sp_ctof(vecParams[1]),
		sp_ctof(vecParams[2]));

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetVector(strField, vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedVector(strField, index, vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}


static cell_t smn_PbSetVector2D(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *vecParams;
	pCtx->LocalToPhysAddr(params[3], &vecParams);

	Vector2D vec(
		sp_ctof(vecParams[0]),
		sp_ctof(vecParams[1]));

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetVector2D(strField, vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedVector2D(strField, index, vec))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbSetMessage(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	char *strValue;
	pCtx->LocalToString(params[3], &strValue);

	int index = params[0] >= 4 ? params[4] : -1;
	if (index < 0)
	{
		if (!msg->SetString(strField, strValue))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}
	else
	{
		if (!msg->SetRepeatedString(strField, index, strValue))
		{
			return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, index, msg->GetProtobufMessage()->GetTypeName().c_str());
		}
	}

	return 1;
}

static cell_t smn_PbAddInt(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	if (!msg->AddInt32OrUnsignedOrEnum(strField, params[3]))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbAddFloat(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	if (!msg->AddFloatOrDouble(strField, sp_ctof(params[3])))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbAddBool(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	bool value = (params[3] == 0 ? false : true);
	if (!msg->AddBool(strField, value))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbAddString(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	char *strValue;
	pCtx->LocalToString(params[3], &strValue);

	if (!msg->AddString(strField, strValue))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbAddColor(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *clrParams;
	pCtx->LocalToPhysAddr(params[3], &clrParams);

	Color clr(
		clrParams[0],
		clrParams[1],
		clrParams[2],
		clrParams[3]);

	if (!msg->AddColor(strField, clr))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbAddAngle(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *angParams;
	pCtx->LocalToPhysAddr(params[3], &angParams);

	QAngle ang(
		sp_ctof(angParams[0]),
		sp_ctof(angParams[1]),
		sp_ctof(angParams[2]));

	if (!msg->AddQAngle(strField, ang))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbAddVector(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *vecParams;
	pCtx->LocalToPhysAddr(params[3], &vecParams);

	Vector vec(
		sp_ctof(vecParams[0]),
		sp_ctof(vecParams[1]),
		sp_ctof(vecParams[2]));

	if (!msg->AddVector(strField, vec))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbAddVector2D(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	cell_t *vecParams;
	pCtx->LocalToPhysAddr(params[3], &vecParams);

	Vector2D vec(
		sp_ctof(vecParams[0]),
		sp_ctof(vecParams[1]));

	if (!msg->AddVector2D(strField, vec))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbRemoveRepeatedFieldValue(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	if (!msg->RemoveRepeatedFieldValue(strField, params[3]))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	return 1;
}

static cell_t smn_PbGetMessage(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	protobuf::Message *innerMsg;
	if (!msg->GetMessage(strField, &innerMsg))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	auto* newMsg = new CSmProtobuf(innerMsg);
	Handle_t outHndl = newMsg->CreateHandle(pCtx);

	msg->AddChildHandle(outHndl);

	return outHndl;
}


static cell_t smn_PbReadRepeatedMessage(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	const protobuf::Message *innerMsg;
	if (!msg->GetRepeatedMessage(strField, params[3], &innerMsg))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\"[%d] for message \"%s\"", strField, params[3], msg->GetProtobufMessage()->GetTypeName().c_str());
	}

	// HACK: Removed const because blah
	auto* newMsg = new CSmProtobuf((protobuf::Message*)innerMsg);
	Handle_t outHndl = newMsg->CreateHandle(pCtx);

	msg->AddChildHandle(outHndl);

	return outHndl;
}

static cell_t smn_PbAddMessage(IPluginContext *pCtx, const cell_t *params)
{
	GET_MSG_FROM_HANDLE_OR_ERR();
	GET_FIELD_NAME_OR_ERR();

	protobuf::Message *innerMsg;
	if (!msg->AddMessage(strField, &innerMsg))
	{
		return pCtx->ThrowNativeError("Invalid field \"%s\" for message \"%s\"", strField, msg->GetProtobufMessage()->GetTypeName().c_str());
	}
	auto* newMsg = new CSmProtobuf(innerMsg);
	Handle_t outHndl = newMsg->CreateHandle(pCtx);

	msg->AddChildHandle(outHndl);

	return outHndl;
}

sp_nativeinfo_t protobuf_natives[] =
{
	// Transitional syntax.
	{ "NM_Protobuf.ReadInt",					smn_PbReadInt },
	{ "NM_Protobuf.ReadFloat",					smn_PbReadFloat },
	{ "NM_Protobuf.ReadBool",					smn_PbReadBool },
	{ "NM_Protobuf.ReadString",					smn_PbReadString },
	{ "NM_Protobuf.ReadColor",					smn_PbReadColor },
	{ "NM_Protobuf.ReadAngle",					smn_PbReadAngle },
	{ "NM_Protobuf.ReadVector",					smn_PbReadVector },
	{ "NM_Protobuf.ReadVector2D",				smn_PbReadVector2D },
	{ "NM_Protobuf.GetRepeatedFieldCount",		smn_PbGetRepeatedFieldCount },
	{ "NM_Protobuf.HasField",					smn_PbHasField },
	{ "NM_Protobuf.SetInt",						smn_PbSetInt },
	{ "NM_Protobuf.SetFloat",					smn_PbSetFloat },
	{ "NM_Protobuf.SetBool",					smn_PbSetBool },
	{ "NM_Protobuf.SetString",					smn_PbSetString },
	{ "NM_Protobuf.SetColor",					smn_PbSetColor },
	{ "NM_Protobuf.SetAngle",					smn_PbSetAngle },
	{ "NM_Protobuf.SetVector",					smn_PbSetVector },
	{ "NM_Protobuf.SetVector2D",				smn_PbSetVector2D },
	{ "NM_Protobuf.AddInt",						smn_PbAddInt },
	{ "NM_Protobuf.AddFloat",					smn_PbAddFloat },
	{ "NM_Protobuf.AddBool",					smn_PbAddBool },
	{ "NM_Protobuf.AddString",					smn_PbAddString },
	{ "NM_Protobuf.AddColor",					smn_PbAddColor },
	{ "NM_Protobuf.AddAngle",					smn_PbAddAngle },
	{ "NM_Protobuf.AddVector",					smn_PbAddVector },
	{ "NM_Protobuf.AddVector2D",				smn_PbAddVector2D },
	{ "NM_Protobuf.RemoveRepeatedFieldValue",	smn_PbRemoveRepeatedFieldValue },
	{ "NM_Protobuf.GetMessage",				    smn_PbGetMessage },
	{ "NM_Protobuf.ReadRepeatedMessage",		smn_PbReadRepeatedMessage },
	{ "NM_Protobuf.AddMessage",					smn_PbAddMessage },
	{ "NM_Protobuf.SendToPlayer",				NM_SendProtobufToPlayer},
	{ "NM_CreateProtobufMessage",				NM_CreateProtobufMessage },
	{ NULL,							NULL }
};
