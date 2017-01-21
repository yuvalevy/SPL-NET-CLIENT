#ifndef DIRLISTPACKET_H_
#define DIRLISTPACKET_H_
#pragma once
#include "TFTPPacket.h"
class DirListPacket :
	public TFTPPacket
{
public:
	DirListPacket();
};

#endif