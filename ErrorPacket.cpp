#include "ErrorPacket.h"




ErrorPacket::ErrorPacket(short errorCode, string errorMessage):TFTPPacket(5),errorCode(errorCode),errorMessage(errorMessage)
{
}

ErrorPacket::ErrorPacket(short errorCode):ErrorPacket(errorCode,"")
{
}

short ErrorPacket::getErrorCode()
{
	return errorCode;
}

string ErrorPacket::getErrorMessage()
{
	return errorMessage;
}

