#include "BCastPacket.h"

BCastPacket::BCastPacket(string file, char add) :TFTPPacket(9), added(add),fileName(file)
{}

string BCastPacket::getFileName()
{
	return fileName;
}

char BCastPacket::getAdded()
{
	return added;
}



