#pragma once
#include "TFTPPacket.h"
#include "DataPacket.h"
#include <string>

using namespace std;

using byte = char;

class WritePacket :
	public TFTPPacket
{

private:
	string fileName;
	byte* fileToWrite;
	void readFile(string fileName);
	int start;
	const int maxPacketSize = 512;
	short blockNum;
	int fileSize;

public:
	WritePacket(string fileName);
	string getFileName();
	DataPacket* getNextPacket();
	DataPacket * createDataPacket();
	short getBlockNum();
	int getFileSize();
	byte* getFileToWrite();
	~WritePacket();
};


