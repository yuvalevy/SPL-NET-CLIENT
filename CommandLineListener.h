#ifndef COMMANDLINELISTENER_H_
#define COMMANDLINELISTENER_H_
#pragma once
#include <string>
#include <iostream>
#include "ClientState.h"
#include <vector>

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
#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"

using namespace std;

class CommandLineListener
{
private:
	ClientState* clientState;
	ConnectionHandler* handler;
	MessageEncoderDecoder encdec;
	vector<string> validCommands;
	bool validateCommand(string command);
	void processCommand(string command);
	bool validateParamLess(string param);
	bool validateShort(string param);
	bool validateString(string param, string paramname);
public:
	CommandLineListener(ClientState*, ConnectionHandler*);
	void run();
	~CommandLineListener();
};

#endif