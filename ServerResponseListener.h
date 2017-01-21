#ifndef SERVERRESPONCELISTENER_H_
#define SERVERRESPONCELISTENER_H_
#pragma once
#include "ClientState.h"
#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"

#include "TFTPPacket.h"	
#include "LoginPacket.h"
#include "DeletePacket.h"
#include "ReadPacket.h"
#include "WritePacket.h"
#include "DirListPacket.h"
#include "DataPacket.h"
#include "AckPacket.h"
#include "ErrorPacket.h"
#include "DisconnectPacket.h"
#include "BCastPacket.h"

using namespace std;

class ServerResponseListener
{
private:
	ConnectionHandler* handler;
	ClientState* clientState;
	MessageEncoderDecoder encdec;
	void handlePacket(TFTPPacket * packet);
	void sendPacket(TFTPPacket * packet);
	void handleData(TFTPPacket * packet);
	void handleAck(TFTPPacket * packet);
	void sendError(short code);
	void handleError(TFTPPacket * packet);
	void printBCast(TFTPPacket * packet);

public:
	ServerResponseListener(ClientState*, ConnectionHandler*);
	void run();
	~ServerResponseListener();
};

#endif