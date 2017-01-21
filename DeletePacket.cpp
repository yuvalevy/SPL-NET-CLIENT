#include "DeletePacket.h"

DeletePacket::DeletePacket(string fileName):TFTPPacket(8), fileName(fileName)
{
}

string DeletePacket::getFileName()
{
	return fileName;
}


