#pragma once
#include "TFTPPacket.h"
#include <string>

using namespace std;

class ErrorPacket :
	public TFTPPacket
{
private:
	short errorCode;
	string errorMessage;
public:
	ErrorPacket(short errorCode,string errorMessage);
	ErrorPacket(short errorCode);
	short getErrorCode();
	string getErrorMessage();
};

