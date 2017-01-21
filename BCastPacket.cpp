#include "BCastPacket.h"

BCastPacket::BCastPacket(string fileName, char added):TFTPPacket(9),fileName(fileName),added(added)
{
}

string BCastPacket::getFileName()
{
	return fileName;
}

char BCastPacket::getAdded()
{
	return added;
}

BCastPacket::BCastPacket(const BCastPacket & other) :TFTPPacket(9), fileName(fileName), added(added)
{
}

BCastPacket & BCastPacket::operator=(const BCastPacket & other)
{
	return *this;
}


