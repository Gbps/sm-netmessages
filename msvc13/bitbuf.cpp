/**
* vim: set ts=4 :
* =============================================================================
* SourceMod
* Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
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

#include "bitbuf.h"

static cell_t smn_BfWriteUBitLong(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteUBitLong(params[2], params[3]);

	return 1;
}

static cell_t smn_BfWriteUBitVar(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteUBitVar(params[2]);

	return 1;
}

static cell_t smn_BfWriteBool(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteOneBit(params[2]);

	return 1;
}

static cell_t smn_BfWriteByte(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteByte(params[2]);

	return 1;
}

static cell_t smn_BfWriteChar(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteChar(params[2]);

	return 1;
}

static cell_t smn_BfWriteShort(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteShort(params[2]);

	return 1;
}

static cell_t smn_BfWriteWord(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteWord(params[2]);

	return 1;
}

static cell_t smn_BfWriteNum(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteLong(static_cast<long>(params[2]));

	return 1;
}

static cell_t smn_BfWriteFloat(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteFloat(sp_ctof(params[2]));

	return 1;
}

static cell_t smn_BfWriteString(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	char *str;
	pCtx->LocalToString(params[2], &str);

	pBitBuf->WriteString(str);

	return 1;
}

static cell_t smn_BfWriteEntity(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	int index = gamehelpers->ReferenceToIndex(params[2]);

	if (index == -1)
	{
		return 0;
	}

	pBitBuf->WriteShort(index);

	return 1;
}

static cell_t smn_BfWriteAngle(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteBitAngle(sp_ctof(params[2]), params[3]);

	return 1;
}

static cell_t smn_BfWriteCoord(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pBitBuf->WriteBitCoord(sp_ctof(params[2]));

	return 1;
}

static cell_t smn_BfWriteVecCoord(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	cell_t *pVec;
	pCtx->LocalToPhysAddr(params[2], &pVec);
	Vector vec(sp_ctof(pVec[0]), sp_ctof(pVec[1]), sp_ctof(pVec[2]));
	pBitBuf->WriteBitVec3Coord(vec);

	return 1;
}

static cell_t smn_BfWriteVecNormal(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	cell_t *pVec;
	pCtx->LocalToPhysAddr(params[2], &pVec);
	Vector vec(sp_ctof(pVec[0]), sp_ctof(pVec[1]), sp_ctof(pVec[2]));
	pBitBuf->WriteBitVec3Normal(vec);

	return 1;
}

static cell_t smn_BfWriteAngles(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_write *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_WrBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	cell_t *pAng;
	pCtx->LocalToPhysAddr(params[2], &pAng);
	QAngle ang(sp_ctof(pAng[0]), sp_ctof(pAng[1]), sp_ctof(pAng[2]));
	pBitBuf->WriteBitAngles(ang);

	return 1;
}

static cell_t smn_BfReadBool(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return pBitBuf->ReadOneBit() ? 1 : 0;
}

static cell_t smn_BfReadByte(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return pBitBuf->ReadByte();
}

static cell_t smn_BfReadChar(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return pBitBuf->ReadChar();
}

static cell_t smn_BfReadShort(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return pBitBuf->ReadShort();
}

static cell_t smn_BfReadWord(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return pBitBuf->ReadWord();
}

static cell_t smn_BfReadNum(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return static_cast<cell_t>(pBitBuf->ReadLong());
}

static cell_t smn_BfReadFloat(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return sp_ftoc(pBitBuf->ReadFloat());
}

static cell_t smn_BfReadString(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;
	int numChars = 0;
	char *buf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	pCtx->LocalToPhysAddr(params[2], (cell_t **)&buf);
	pBitBuf->ReadString(buf, params[3], params[4] ? true : false, &numChars);

	if (pBitBuf->IsOverflowed())
	{
		return -numChars - 1;
	}

	return numChars;
}

static cell_t smn_BfReadEntity(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	int ref = gamehelpers->IndexToReference(pBitBuf->ReadShort());

	return gamehelpers->ReferenceToBCompatRef(ref);
}

static cell_t smn_BfReadAngle(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return sp_ftoc(pBitBuf->ReadBitAngle(params[2]));
}

static cell_t smn_BfReadCoord(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return sp_ftoc(pBitBuf->ReadBitCoord());
}

static cell_t smn_BfReadVecCoord(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	cell_t *pVec;
	pCtx->LocalToPhysAddr(params[2], &pVec);

	Vector vec;
	pBitBuf->ReadBitVec3Coord(vec);

	pVec[0] = sp_ftoc(vec.x);
	pVec[1] = sp_ftoc(vec.y);
	pVec[2] = sp_ftoc(vec.z);

	return 1;
}

static cell_t smn_BfReadVecNormal(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	cell_t *pVec;
	pCtx->LocalToPhysAddr(params[2], &pVec);

	Vector vec;
	pBitBuf->ReadBitVec3Normal(vec);

	pVec[0] = sp_ftoc(vec.x);
	pVec[1] = sp_ftoc(vec.y);
	pVec[2] = sp_ftoc(vec.z);

	return 1;
}

static cell_t smn_BfReadAngles(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	cell_t *pAng;
	pCtx->LocalToPhysAddr(params[2], &pAng);

	QAngle ang;
	pBitBuf->ReadBitAngles(ang);

	pAng[0] = sp_ftoc(ang.x);
	pAng[1] = sp_ftoc(ang.y);
	pAng[2] = sp_ftoc(ang.z);

	return 1;
}

static cell_t smn_BfGetNumBytesLeft(IPluginContext *pCtx, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError herr;
	HandleSecurity sec;
	bf_read *pBitBuf;

	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	if ((herr = handlesys->ReadHandle(hndl, g_RdBitBufType, &sec, (void **)&pBitBuf))
		!= HandleError_None)
	{
		return pCtx->ThrowNativeError("Invalid bit buffer handle %x (error %d)", hndl, herr);
	}

	return pBitBuf->GetNumBitsLeft() >> 3;
}

sp_nativeinfo_t bitbuf_natives[] =
{
	//{ "NM_BfWriteBool", smn_BfWriteBool},
	//{ "NM_BfWriteByte", smn_BfWriteByte },
	//{ "NM_BfWriteChar", smn_BfWriteChar },
	//{ "NM_BfWriteShort", smn_BfWriteShort },
	//{ "NM_BfWriteWord", smn_BfWriteWord },
	//{ "NM_BfWriteNum", smn_BfWriteNum },
	//{ "NM_BfWriteFloat", smn_BfWriteFloat },
	//{ "NM_BfWriteString", smn_BfWriteString },
	//{ "NM_BfWriteEntity", smn_BfWriteEntity },
	//{ "NM_BfWriteAngle", smn_BfWriteAngle },
	//{ "NM_BfWriteCoord", smn_BfWriteCoord },
	//{ "NM_BfWriteVecCoord", smn_BfWriteVecCoord },
	//{ "NM_BfWriteVecNormal", smn_BfWriteVecNormal },
	//{ "NM_BfWriteAngles", smn_BfWriteAngles },
	//{ "NM_BfReadBool", smn_BfReadBool },
	//{ "NM_BfReadByte", smn_BfReadByte },
	//{ "NM_BfReadChar", smn_BfReadChar },
	//{ "NM_BfReadShort", smn_BfReadShort },
	//{ "NM_BfReadWord", smn_BfReadWord },
	//{ "NM_BfReadNum", smn_BfReadNum },
	//{ "NM_BfReadFloat", smn_BfReadFloat },
	//{ "NM_BfReadString", smn_BfReadString },
	//{ "NM_BfReadEntity", smn_BfReadEntity },
	//{ "NM_BfReadAngle", smn_BfReadAngle },
	//{ "NM_BfReadCoord", smn_BfReadCoord },
	//{ "NM_BfReadVecCoord", smn_BfReadVecCoord },
	//{ "NM_BfReadVecNormal", smn_BfReadVecNormal },
	//{ "NM_BfReadAngles", smn_BfReadAngles },
	//{ "NM_BfGetNumBytesLeft", smn_BfGetNumBytesLeft },

	{ "NM_BfWrite.WriteUBitLong", smn_BfWriteUBitLong },
	{ "NM_BfWrite.WriteUBitVar", smn_BfWriteUBitVar },
	{ "NM_BfWrite.WriteBool", smn_BfWriteBool },
	{ "NM_BfWrite.WriteByte", smn_BfWriteByte },
	{ "NM_BfWrite.WriteChar", smn_BfWriteChar },
	{ "NM_BfWrite.WriteShort", smn_BfWriteShort },
	{ "NM_BfWrite.WriteWord", smn_BfWriteWord },
	{ "NM_BfWrite.WriteNum", smn_BfWriteNum },
	{ "NM_BfWrite.WriteFloat", smn_BfWriteFloat },
	{ "NM_BfWrite.WriteString", smn_BfWriteString },
	{ "NM_BfWrite.WriteEntity", smn_BfWriteEntity },
	{ "NM_BfWrite.WriteAngle", smn_BfWriteAngle },
	{ "NM_BfWrite.WriteCoord", smn_BfWriteCoord },
	{ "NM_BfWrite.WriteVecCoord", smn_BfWriteVecCoord },
	{ "NM_BfWrite.WriteVecNormal", smn_BfWriteVecNormal },
	{ "NM_BfWrite.WriteAngles", smn_BfWriteAngles },

	{ "NM_BfRead.ReadBool", smn_BfReadBool },
	{ "NM_BfRead.ReadByte", smn_BfReadByte },
	{ "NM_BfRead.ReadChar", smn_BfReadChar },
	{ "NM_BfRead.ReadShort", smn_BfReadShort },
	{ "NM_BfRead.ReadWord", smn_BfReadWord },
	{ "NM_BfRead.ReadNum", smn_BfReadNum },
	{ "NM_BfRead.ReadFloat", smn_BfReadFloat },
	{ "NM_BfRead.ReadString", smn_BfReadString },
	{ "NM_BfRead.ReadEntity", smn_BfReadEntity },
	{ "NM_BfRead.ReadAngle", smn_BfReadAngle },
	{ "NM_BfRead.ReadCoord", smn_BfReadCoord },
	{ "NM_BfRead.ReadVecCoord", smn_BfReadVecCoord },
	{ "NM_BfRead.ReadVecNormal", smn_BfReadVecNormal },
	{ "NM_BfRead.ReadAngles", smn_BfReadAngles },
	{ "NM_BfRead.BytesLeft.get", smn_BfGetNumBytesLeft },
	{ NULL, NULL }
};