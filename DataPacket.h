#pragma once
#include <string>
#include "TFTPPacket.h"

using namespace std;

class DataPacket :
	public TFTPPacket
{
private:
	short blockNum;
	const char* data;
	short size;
public:
	DataPacket(short blockNum, const char * byte, short datasize);
	void execute(string filename);
	const char* getData();
	short getSize();
	short getBlockNum();
	~DataPacket();
};

