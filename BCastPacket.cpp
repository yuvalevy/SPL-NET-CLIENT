#include "BCastPacket.h"

BCastPacket::BCastPacket(string file, char add) :TFTPPacket(9), fileName(file), added(add)
{}

string BCastPacket::getFileName()
{
	return fileName;
}

char BCastPacket::getAdded()
{
	return added;
}



