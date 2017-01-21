#pragma once
#include <string>
#include <vector>
#include "WritePacket.h"

enum State
{
	RUTINE,
	READ,
	WRITE,
	DIRLIST,
	LOGININGIN,
	DISCONNECTING
};

using namespace std;
using byte = char;
class ClientState
{
private:
	State curState;
	short blockNum;
	string fileName;
	bool isConnected;
	WritePacket* writePacket;
	vector<byte> nextString;
	vector<byte> stringbytes;
	char seperator = '\0';
	void copy(int startIndex, int endIndex, vector<byte> from, byte * toArray);
public:
	ClientState();
	~ClientState();
	void setState(State);
	State getState();
	void setBlockNum(short);
	short getBlockNum();
	void setFileName(string);
	string getFileName();
	void setConnection(bool);
	bool getConnection();
	void initState();
	void setWritePacket(WritePacket*);
	WritePacket* getWritePacket();
	string getNextString();
	void setNextBytes(const byte*,int);
};

