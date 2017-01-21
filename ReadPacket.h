#ifndef READPACKET_H_
#define READPACKET_H_
#pragma once
#include "TFTPPacket.h"
#include <string>


using namespace std;

class ReadPacket :
	public TFTPPacket
{
private:
	string fileName;
public:
	ReadPacket(string fileName);
	string getFileName();
};

#endif