#ifndef MESSAGEENCODERDECODER_H_
#define MESSAGEENCODERDECODER_H_
#pragma once
#include "TFTPPacket.h"	
#include "LoginPacket.h"
#include "DeletePacket.h"
#include "ReadPacket.h"
#include "WritePacket.h"
#include "DirListPacket.h"
#include "DataPacket.h"
#include "AckPacket.h"
#include "ErrorPacket.h"
#include "DisconnectPacket.h"
#include "BCastPacket.h"
#include <vector>


using byte =  char;

enum ReadState
{
	ENDER,
	DATA,
	ACK
};

class MessageEncoderDecoder
{
private:
	const byte ender = '\0';
	vector<byte>  packet;
	byte * opCodeBytes;
	int opCodeSize;
	int opCode;
	ReadState state;
	int sizeExpected;
	short encodedPacketSize;
	byte * encodeLogin(TFTPPacket* packet);
	byte * encodeDelete(TFTPPacket* packet);
	byte * encodeRead(TFTPPacket* packet);
	byte * encodeWrite(TFTPPacket* packet);
	byte * encodeDirlist(TFTPPacket* packet);
	byte * encodeData(TFTPPacket* packet);
	byte * encodeAck(TFTPPacket* packet);
	byte * encodeError(TFTPPacket* packet);
	byte * encodeDisconnect(TFTPPacket* packet);

	void insertBytes(const byte* origin, byte * target, short size);
	void insertShort(short num, byte * target);
	void insertString(string num, byte * target);
	void insertEnder(byte * target);
	short BytesToShort(vector<byte> field);
	short BytesToShort(byte* field);
	void copy(int startIndex, int endIndex, vector<byte> from, byte * toArray);
	ErrorPacket* createErrorPacket();
	BCastPacket * createBCastPacket();
	AckPacket * createAckPacket();
	DataPacket * createDataPacket();
public:
	MessageEncoderDecoder();
	TFTPPacket * next(byte nextByte);
	void init();
	TFTPPacket* decodeNextByte(byte nextByte);
	byte* encode(TFTPPacket* message);
	short getPacketSize();
	byte * encodeBCast(TFTPPacket * pckt);
	~MessageEncoderDecoder();
	MessageEncoderDecoder(const MessageEncoderDecoder& other);
	MessageEncoderDecoder& operator=(const MessageEncoderDecoder& other);
};

#endif