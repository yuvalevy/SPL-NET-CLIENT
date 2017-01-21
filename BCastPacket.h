#ifndef BCASTPACKET_H_
#define BCASTPACKET_H_
#pragma once
#include "TFTPPacket.h"
#include <string>

using namespace std;

class BCastPacket :
	public TFTPPacket
{
private:
	char added;
	string fileName;
public:
	BCastPacket(string fileName,char added);
	string getFileName();
	char getAdded();
};

#endif