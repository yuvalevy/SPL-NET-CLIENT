#pragma once
#include <string>
#include "TFTPPacket.h"

using namespace std;

class LoginPacket :
	public TFTPPacket
{
private:
	string userName;
public:
	LoginPacket(string userName);
	string getUserName();
};

