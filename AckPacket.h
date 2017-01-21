#ifndef ACKPACKET_H_
#define ACKPACKET_H_
#pragma once
#include "TFTPPacket.h"
class AckPacket :
	public TFTPPacket
{

private:
	short blockNumber;
public:
	AckPacket(short blockNumber);
	short getBlockNum();
};
#endif