#include "TFTPPacket.h"

TFTPPacket::TFTPPacket(short opcode):opCode(opcode)
{
}

short TFTPPacket::getOpcode()
{
	return opCode;
}
