#include "LoginPacket.h"


LoginPacket::LoginPacket(string userName):TFTPPacket(7), userName(userName)
{
}

string LoginPacket::getUserName()
{
	return userName;
}

