#include "AckPacket.h"

AckPacket::AckPacket(short blockNumber):TFTPPacket(4),blockNumber(blockNumber)
{
}

short AckPacket::getBlockNum()
{
	return blockNumber;
}


