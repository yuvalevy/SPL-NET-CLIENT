#ifndef DISCONNECTPACKET_H_
#define DISCONNECTPACKET_H_
#pragma once
#include "TFTPPacket.h"
class DisconnectPacket :
	public TFTPPacket
{
public:
	DisconnectPacket();
};
#endif
