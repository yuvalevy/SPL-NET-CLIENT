#include "ReadPacket.h"

ReadPacket::ReadPacket(string fileName) :TFTPPacket(1), fileName(fileName)
{
}

string ReadPacket::getFileName()
{
	return fileName;
}


