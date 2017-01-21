#include "ClientState.h"
#include <iostream>

using namespace std;
ClientState::ClientState():isConnected(false),blockNum(-1),curState(State::RUTINE),fileName(string()),writePacket(nullptr)
{
}

ClientState::~ClientState()
{
}

void ClientState::setState(State s)
{
	this->curState = s;
}

State ClientState::getState()
{
	return curState;
}

void ClientState::setBlockNum(short blocknum)
{
	this->blockNum = blocknum;
}

short ClientState::getBlockNum()
{
	return blockNum;
}

void ClientState::setFileName(string name)
{
	this->fileName = name;
}

string ClientState::getFileName()
{
	return fileName;
}

void ClientState::setConnection(bool connection)
{
	isConnected = connection;
}

bool ClientState::getConnection()
{
	return isConnected;
}

void ClientState::initState()
{
	curState = State::RUTINE;
	fileName = "";
	blockNum = -1;
	if (writePacket != nullptr) {
		delete writePacket;
		writePacket = nullptr;
	}
}

void ClientState::setWritePacket(WritePacket* packet)
{
	writePacket = packet;
}

WritePacket * ClientState::getWritePacket()
{
	return writePacket;
}

string ClientState::getNextString()
{
	string filename = "";

	for (size_t i = 0; i < stringbytes.size(); i++)
	{
		if (stringbytes.at(i) == seperator)
		{
			byte* arrPacket = new byte[i];
			copy(0, i, stringbytes, arrPacket);
			filename = string(arrPacket, i);
			delete[] arrPacket;
			stringbytes.erase(stringbytes.begin(), stringbytes.begin() + i+1);
			break;
		}
	}

	return filename;
}

void ClientState::setNextBytes(const byte * byteArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		stringbytes.push_back(byteArray[i]);
	}
}

ClientState::ClientState(const ClientState & other) :isConnected(false), blockNum(-1), curState(State::RUTINE), fileName(string()), writePacket(nullptr),nextString(vector<byte>()),stringbytes(vector<byte>())
{
}

ClientState & ClientState::operator=(const ClientState & other)
{
	return *this;
}

void ClientState::copy(int startIndex, int endIndex, vector<byte> from, byte* toArray) {
	int index = 0;
	for (int i = startIndex; i < endIndex; i++)
	{
		toArray[index] = from.at(i);
		index++;
	}
}
