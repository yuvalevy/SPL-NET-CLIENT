#ifndef DELETEPACKET_H_
#define DELETEPACKET_H_
#pragma once
#include "TFTPPacket.h"
#include <string>

using namespace std;

class DeletePacket :
	public TFTPPacket
{
private:
	string fileName;
public:
	DeletePacket(string fileName);
	string getFileName();
};

#endif