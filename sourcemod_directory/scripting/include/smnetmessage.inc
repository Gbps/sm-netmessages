#if defined _smnetmessage_included
	#endinput
#endif

#define _smnetmessage_included

/**
 * Get a bf_write buffer for a given client.
 *
 * @param client          Client to get bf_write for.
 * @param msgnum          Net message number to send
 *
 * @return                A handle to a bf_write bit packing structure, or INVALID_HANDLE on failure.
 */
native NM_BfWrite NM_GetNetChannelBfWrite(int client, NetMessage msgnum);

/**
 * Get a new Protobuf packet object
 *
 * @param msgType         Name of the message
 *
 * @return                A handle to a NM_Protobuf, or INVALID_HANDLE on failure.
 */
native NM_Protobuf NM_CreateProtobufMessage(char[] msgType);

methodmap NM_BfWrite < Handle
{
    // Writes a signed or unsigned integer with given number of bits
	//
	// @param num       Number to write
    // @param bits      Size of number in bits
	public native void WriteUBitLong(int num, int bits);

    // Writes a signed or unsigned integer with an auto-calculated number of bits
	//
	// @param num       Number to write
	public native void WriteUBitVar(int num);

	// Writes a single bit to a writable bitbuffer (bf_write).
	//
	// @param bit       Bit to write (true for 1, false for 0).
	public native void WriteBool(int bit);
	 
	// Writes a byte to a writable bitbuffer (bf_write).
	//
	// @param byte      Byte to write (value will be written as 8bit).
	public native void WriteByte(int byte);

	// Writes a byte to a writable bitbuffer (bf_write).
	//
	// @param chr       Character to write.
	public native void WriteChar(int chr);

	// Writes a 16bit integer to a writable bitbuffer (bf_write).
	//
	// @param num       Integer to write (value will be written as 16bit).
	public native void WriteShort(int num);

	// Writes a 16bit unsigned integer to a writable bitbuffer (bf_write).
	//
	// @param num       Integer to write (value will be written as 16bit).
	public native void WriteWord(int num);

	// Writes a normal integer to a writable bitbuffer (bf_write).
	//
	// @param num       Integer to write (value will be written as 32bit).
	public native void WriteNum(int num);

	// Writes a floating point number to a writable bitbuffer (bf_write).
	//
	// @param num       Number to write.
	public native void WriteFloat(float num);

	// Writes a string to a writable bitbuffer (bf_write).
	//
	// @param string    Text string to write.
	public native void WriteString(const char[] string);

	// Writes an entity to a writable bitbuffer (bf_write).
	//
	// @param ent       Entity index to write.
	public native void WriteEntity(int ent);

	// Writes a bit angle to a writable bitbuffer (bf_write).
	//
	// @param angle     Angle to write.
	// @param numBits   Optional number of bits to use.
	public native void WriteAngle(float angle, int numBits=8);

	// Writes a coordinate to a writable bitbuffer (bf_write).
	//
	// @param coord     Coordinate to write.
	public native void WriteCoord(float coord);

	// Writes a 3D vector of coordinates to a writable bitbuffer (bf_write).
	//
	// @param coord     Coordinate array to write.
	public native void WriteVecCoord(float coord[3]);

	// Writes a 3D normal vector to a writable bitbuffer (bf_write).
	//
	// @param vec       Vector to write.
	public native void WriteVecNormal(float vec[3]);

	// Writes a 3D angle vector to a writable bitbuffer (bf_write).
	//
	// @param angles    Angle vector to write.
	public native void WriteAngles(float angles[3]);

    /**
    * Send a netmessage packet to a player
    *
    * @param client          Client to send message to.
    *
    * @return                None
    */
    public native void SendToPlayer(int client);
};

// methodmap NM_BfRead < Handle
// {
// 	// Reads a single bit from a readable bitbuffer (bf_read).
// 	//
// 	// @return          Bit value read.
// 	public native bool ReadBool();

// 	// Reads a byte from a readable bitbuffer (bf_read).
// 	//
// 	// @return          Byte value read (read as 8bit).
// 	public native int ReadByte();

// 	// Reads a character from a readable bitbuffer (bf_read).
// 	//
// 	// @return          Character value read.
// 	public native int ReadChar();

// 	// Reads a 16bit integer from a readable bitbuffer (bf_read).
// 	//
// 	// @param bf        bf_read handle to read from.
// 	// @return          Integer value read (read as 16bit).
// 	public native int ReadShort();

// 	// Reads a 16bit unsigned integer from a readable bitbuffer (bf_read).
// 	//
// 	// @param bf        bf_read handle to read from.
// 	// @return          Integer value read (read as 16bit).
// 	public native int ReadWord();

// 	// Reads a normal integer to a readable bitbuffer (bf_read).
// 	//
// 	// @return          Integer value read (read as 32bit).
// 	public native int ReadNum();

// 	// Reads a floating point number from a readable bitbuffer (bf_read).
// 	//
// 	// @return          Floating point value read.
// 	public native float ReadFloat();

// 	// Reads a string from a readable bitbuffer (bf_read).
// 	//
// 	// @param buffer    Destination string buffer.
// 	// @param maxlength Maximum length of output string buffer.
// 	// @param line      If true the buffer will be copied until it reaches a '\n' or a null terminator.
// 	// @return          Number of bytes written to the buffer.  If the bitbuffer stream overflowed, 
// 	//                  that is, had no terminator before the end of the stream, then a negative 
// 	//                  number will be returned equal to the number of characters written to the 
// 	//                  buffer minus 1.  The buffer will be null terminated regardless of the 
// 	//                  return value.
// 	public native int ReadString(char[] buffer, int maxlength, bool line=false);

// 	// Reads an entity from a readable bitbuffer (bf_read).
// 	//
// 	// @return          Entity index read.
// 	public native int ReadEntity();

// 	// Reads a bit angle from a readable bitbuffer (bf_read).
// 	//
// 	// @param numBits   Optional number of bits to use.
// 	// @return          Angle read.
// 	public native float ReadAngle(int numBits=8);

// 	// Reads a coordinate from a readable bitbuffer (bf_read).
// 	//
// 	// @return          Coordinate read.
// 	public native float ReadCoord();

// 	// Reads a 3D vector of coordinates from a readable bitbuffer (bf_read).
// 	//
// 	// @param coord     Destination coordinate array.
// 	public native void ReadVecCoord(float coord[3]);

// 	// Reads a 3D normal vector from a readable bitbuffer (bf_read).
// 	//
// 	// @param vec       Destination vector array.
// 	public native void ReadVecNormal(float vec[3]);

// 	// Reads a 3D angle vector from a readable bitbuffer (bf_read).
// 	//
// 	// @param angles    Destination angle vector.
// 	public native void ReadAngles(float angles[3]);

// 	// Returns the number of bytes left in a readable bitbuffer (bf_read).
// 	property int BytesLeft {
// 		public native get();
// 	}
// };

enum NetMessage
{
	net_NOP = 0, // nop command used for padding
    net_Disconnect = 1, // disconnect, last message in connection
    net_File = 2, // file transmission message request/deny

    net_LastControlMessage = 2, 
    net_Tick = 3, // send last world tick
    net_StringCmd = 4, // a string command
    net_SetConVar = 5, // sends one/multiple convar settings
    net_SignonState = 6, // signals current signon state

    //
    // server to client
    //

    svc_Print = 7, // print text to console

    svc_ServerInfo = 8, // first message from server about game, map etc
    svc_SendTable = 9, // sends a sendtable description for a game class
    svc_ClassInfo = 10, // Info about classes (first byte is a CLASSINFO_ define).							
    svc_SetPause = 11, // tells client if server paused or unpaused

    svc_CreateStringTable = 12, // inits shared string tables
    svc_UpdateStringTable = 13, // updates a string table

    svc_VoiceInit = 14, // inits used voice codecs & quality
    svc_VoiceData = 15, // Voicestream data from the server

    svc_Sounds = 17, // starts playing sound

    svc_SetView = 18, // sets entity as point of view
    svc_FixAngle = 19, // sets/corrects players viewangle
    svc_CrosshairAngle = 20, // adjusts crosshair in auto aim mode to lock on traget

    svc_BSPDecal = 21, // add a static decal to the world BSP

    // Message from server side to client side entity
    svc_UserMessage = 23, // a game specific message 
    svc_EntityMessage = 24, // a message for an entity
    svc_GameEvent = 25, // global game event fired

    svc_PacketEntities = 26, // non-delta compressed entities

    svc_TempEntities = 27, // non-reliable event object

    svc_Prefetch = 28, // only sound indices for now

    svc_Menu = 29, // display a menu from a plugin

    svc_GameEventList = 30, // list of known games events and fields

    svc_GetCvarValue = 31, // Server wants to know the value of a cvar on the client

    svc_CmdKeyValues = 32, // Server submits KeyValues command for the client

    SVC_LASTMSG = 32, // last known server message

    //
    // client to server
    //

    clc_ClientInfo = 8, // client info (table CRC etc)
    clc_Move = 9, // [CUserCmd]
    clc_VoiceData = 10, // Voicestream data from a client
    clc_BaselineAck = 11, // client acknowledges a new baseline seqnr
    clc_ListenEvents = 12, // client acknowledges a new baseline seqnr
    clc_RespondCvarValue = 13, // client is responding to a svc_GetCvarValue message.
    clc_FileCRCCheck = 14, // client is sending a file's CRC to the server to be verified.
    clc_CmdKeyValues = 16, 
    clc_FileMD5Check = 17, // client is sending a file's MD5 to the server to be verified.
    CLC_LASTMSG = 17 //	last known client message
};


methodmap NM_Protobuf < Handle
{
	// Reads an int32, uint32, sint32, fixed32, sfixed32, or enum value from a protobuf message.
	// 
	// @param field      Field name.
	// @param index      Index into repeated field.
	// @return           Integer value read.
	// @error            Non-existent field, or incorrect field type.
	public native int ReadInt(const char[] field, int index = PB_FIELD_NOT_REPEATED);

	// Reads a float or downcasted double from a protobuf message.
	//
	// @param field      Field name.
	// @param index      Index into repeated field.
	// @return           Float value read.
	// @error            Non-existent field, or incorrect field type.
	public native float ReadFloat(const char[] field, int index = PB_FIELD_NOT_REPEATED);

	// Reads a bool from a protobuf message.
	// 
	// @param field      Field name.
	// @param index      Index into repeated field.
	// @return           Boolean value read.
	// @error            Non-existent field, or incorrect field type.
	public native bool ReadBool(const char[] field, int index = PB_FIELD_NOT_REPEATED);

	// Reads a string from a protobuf message.
	// 
	// @param field      Field name.
	// @param buffer     Destination string buffer.
	// @param maxlength  Maximum length of output string buffer.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void ReadString(const char[] field, char[] buffer, int maxlength, int index = PB_FIELD_NOT_REPEATED);

	// Reads an RGBA color value from a protobuf message.
	// 
	// @param field      Field name.
	// @param buffer     Destination color buffer.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void ReadColor(const char[] field, int buffer[4], int index = PB_FIELD_NOT_REPEATED);

	// Reads an XYZ angle value from a protobuf message.
	//
	// @param field      Field name.
	// @param buffer     Destination angle buffer.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void ReadAngle(const char[] field, float buffer[3], int index = PB_FIELD_NOT_REPEATED);

	// Reads an XYZ vector value from a protobuf message.
	// 
	// @param pb         protobuf handle.
	// @param field      Field name.
	// @param buffer     Destination vector buffer.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void ReadVector(const char[] field, float buffer[3], int index = PB_FIELD_NOT_REPEATED);

	// Reads an XY vector value from a protobuf message.
	//
	// @param field      Field name.
	// @param buffer     Destination vector buffer.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void ReadVector2D(const char[] field, float buffer[2], int index = PB_FIELD_NOT_REPEATED);

	// Gets the number of elements in a repeated field of a protobuf message.
	//
	// @param field      Field name.
	// @return           Number of elements in the field.
	// @error            Non-existent field, or non-repeated field.
	public native int GetRepeatedFieldCount(const char[] field);
	
	// Returns whether or not the named, non-repeated field has a value set.
	//
	// @param field      Field name.
	// @return           True if value has been set, else false.
	// @error            Non-existent field, or repeated field.
	public native bool HasField(const char[] field);

	// Sets an int32, uint32, sint32, fixed32, sfixed32, or enum value on a protobuf message.
	//
	// @param field      Field name.
	// @param value      Integer value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native int SetInt(const char[] field, int value, int index = PB_FIELD_NOT_REPEATED);

	// Sets a float or double on a protobuf message.
	//
	// @param field      Field name.
	// @param value      Float value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void SetFloat(const char[] field, float value, int index = PB_FIELD_NOT_REPEATED);

	// Sets a bool on a protobuf message.
	// 
	// @param field      Field name.
	// @param value      Boolean value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void SetBool(const char[] field, bool value, int index = PB_FIELD_NOT_REPEATED);

	// Sets a string on a protobuf message.
	//
	// @param field      Field name.
	// @param value      String value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void SetString(const char[] field, const char[] value, int index = PB_FIELD_NOT_REPEATED);

	// Sets an RGBA color on a protobuf message.
	// 
	// @param field      Field name.
	// @param color      Color value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void SetColor(const char[] field, const int color[4], int index = PB_FIELD_NOT_REPEATED);

	// Sets an XYZ angle on a protobuf message.
	// 
	// @param field      Field name.
	// @param angle      Angle value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void SetAngle(const char[] field, const float angle[3], int index = PB_FIELD_NOT_REPEATED);

	// Sets an XYZ vector on a protobuf message.
	// 
	// @param field      Field name.
	// @param vec        Vector value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void SetVector(const char[] field, const float vec[3], int index = PB_FIELD_NOT_REPEATED);

	// Sets an XY vector on a protobuf message.
	//
	// @param field      Field name.
	// @param vec        Vector value to set.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void SetVector2D(const char[] field, const float vec[2], int index = PB_FIELD_NOT_REPEATED);

	// Add an int32, uint32, sint32, fixed32, sfixed32, or enum value to a protobuf message repeated field.
	//
	// @param field      Field name.
	// @param value      Integer value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddInt(const char[] field, int value);

	// Add a float or double to a protobuf message repeated field.
	// 
	// @param field      Field name.
	// @param value      Float value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddFloat(const char[] field, float value);

	// Add a bool to a protobuf message repeated field.
	//
	// @param field      Field name.
	// @param value      Boolean value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddBool(const char[] field, bool value);

	// Add a string to a protobuf message repeated field.
	//
	// @param field      Field name.
	// @param value      String value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddString(const char[] field, const char[] value);

	// Add an RGBA color to a protobuf message repeated field.
	// 
	// @param field      Field name.
	// @param color      Color value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddColor(const char[] field, const int color[4]);

	// Add an XYZ angle to a protobuf message repeated field.
	// 
	// @param field      Field name.
	// @param angle      Angle value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddAngle(const char[] field, const float angle[3]);

	// Add an XYZ vector to a protobuf message repeated field.
	//
	// @param field      Field name.
	// @param vec        Vector value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddVector(const char[] field, const float vec[3]);

	// Add an XY vector to a protobuf message repeated field.
	//
	// @param field      Field name.
	// @param vec        Vector value to add.
	// @error            Non-existent field, or incorrect field type.
	public native void AddVector2D(const char[] field, const float vec[2]);

	// Removes a value by index from a protobuf message repeated field.
	//
	// @param field      Field name.
	// @param index      Index into repeated field.
	// @error            Non-existent field, or incorrect field type.
	public native void RemoveRepeatedFieldValue(const char[] field, int index);

	// Retrieve a handle to an embedded protobuf message in a protobuf message.
	//
	// @param field      Field name.
	// @return           Protobuf handle to embedded message.
	// @error            Non-existent field, or incorrect field type.
	public native NM_Protobuf GetMessage(const char[] field);

	// Retrieve a handle to an embedded protobuf message in a protobuf message
	// repeated field.
	//
	// @param field      Field name.
	// @param index      Index in the repeated field.
	// @return           Protobuf handle to embedded message.
	// @error            Non-existent field, or incorrect field type.
	public native NM_Protobuf ReadRepeatedMessage(const char[] field, int index);

	// Adds an embedded protobuf message to a protobuf message repeated field.
	//
	// @param field      Field name.
	// @return           Protobuf handle to added, embedded message.
	// @error            Non-existent field, or incorrect field type.
	public native NM_Protobuf AddMessage(const char[] field);

    /**
    * Send a netmessage Protobuf to a player
    *
    * @param client          Client to send message to.
    *
    * @return                None
    */
    public native void SendToPlayer(int client);

};


/** Do Not Edit Below This Line **/
public Extension:__ext_smnetmessage =
{
	name = "smnetmessage",
	file = "smnetmessage.ext",

	#if defined AUTOLOAD_EXTENSIONS
		autoload = 1,
	#else
		autoload = 0,
	#endif

	#if defined REQUIRE_EXTENSIONS
		required = 1,
	#else
		required = 0,
	#endif
};
