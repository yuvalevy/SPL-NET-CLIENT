#include "AckPacket.h"

AckPacket::AckPacket(short blockNumber):TFTPPacket(4),blockNumber(blockNumber)
{
}

short AckPacket::getBlockNum()
{
	return blockNumber;
}

AckPacket::AckPacket(const AckPacket & other) :TFTPPacket(4), blockNumber(other.blockNumber)
{
}

AckPacket & AckPacket::operator=(const AckPacket & other)
{
	return *this;
}


