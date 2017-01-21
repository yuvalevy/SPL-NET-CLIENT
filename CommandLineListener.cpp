#include "CommandLineListener.h"


CommandLineListener::CommandLineListener(ClientState* state,ConnectionHandler* hndlr) :clientState(state),handler(hndlr),validCommands(vector<string>())
{
	validCommands.push_back("RRQ");
	validCommands.push_back("WRQ");
	validCommands.push_back("DATA");
	validCommands.push_back("ACK");
	validCommands.push_back("ERROR");
	validCommands.push_back("DIRQ");
	validCommands.push_back("LOGRQ");
	validCommands.push_back("DELRQ");
	validCommands.push_back("BCAST");
	validCommands.push_back("DISC");
}

CommandLineListener::CommandLineListener(const CommandLineListener & other) : validCommands(vector<string>())
{
}

bool CommandLineListener::validateCommand(string command)
{
	size_t index = command.find_first_of(' ');
	if (index == string::npos)
		index = command.size();

	string item = command.substr(0, index);
	return find(validCommands.begin(), validCommands.end(), item)!=validCommands.end();
}

void CommandLineListener::processCommand(string command)
{
	size_t index = command.find_first_of(' ');
	string params;
	
	if (index == string::npos)
		index = command.size();
	else
		params = command.substr(index + 1, command.size());

	string opcodeName = command.substr(0, index);

	vector<string>::iterator iter = find(validCommands.begin(), validCommands.end(), opcodeName);
	int opcode = distance(validCommands.begin(), iter) + 1;

	TFTPPacket* packet = nullptr;
	State change = State::RUTINE;
	bool isValid = true;
	bool deletepacket = true;

	switch (opcode)
	{
	case 1:
	{
		isValid = validateString(params, "FILE NAME");
		if (isValid)
		{
			packet = new ReadPacket(params);
			clientState->setFileName(params);
			clientState->setBlockNum(1);
			change = State::READ;
		}
		break;
	}
	case 2:
	{
		isValid = validateString(params, "FILE NAME");
		if (isValid)
		{
			deletepacket = false;
			packet = new WritePacket(params);
			clientState->setWritePacket(static_cast<WritePacket*>(packet));
			clientState->setFileName(params);
			clientState->setBlockNum(0);
			change = State::WRITE;
		}
		break;
	}
	case 3:
	{
		packet = new DataPacket(0, new byte[0], 0);
		break;
	}
	case 4:
	{
		isValid = validateShort(params);
		if (isValid)
		{
			short blockNum = (short)stoi(params);
			packet = new AckPacket(blockNum);
		}
		break;
	}
	case 5:
	{
		packet = new ErrorPacket(0);
		break;
	}
	case 6:
	{
		isValid = validateParamLess(params);
		if (isValid)
		{
			packet = new DirListPacket();
			clientState->setBlockNum(1);
			change = State::DIRLIST;
		}
		break;
	}
	case 7:
	{
		isValid = validateString(params, "USER NAME");
		if (isValid)
		{
			change = State::LOGININGIN;
			packet = new LoginPacket(params);
		}
		break;
	}
	case 8:
	{
		isValid = validateString(params, "FILE NAME");
		if (isValid)
			packet = new DeletePacket(params);
		break;
	}
	case 9:
	{
		packet = new BCastPacket("error", '1');
		break;
	}
	case 10:
	{
		isValid = validateParamLess(params) && clientState->getState() == State::RUTINE;
		if (isValid) 
		{
			if (clientState->getConnection())
				change = State::DISCONNECTING;
			packet = new DisconnectPacket();
		}
		break;
	}
	}

	if (isValid)
	{
		byte* bytes = encdec.encode(packet);
		handler->sendBytes(bytes, (int)encdec.getPacketSize());
		clientState->setState(change);
		delete bytes;
		if (deletepacket)
			delete packet;
	}
}

void CommandLineListener::run()
{
	string command;
	while (!(clientState->getConnection() && command == "DISC"))
	{
 		getline(cin, command);
		if (validateCommand(command))
			processCommand(command); 
		else
			cout << "INVALID INPUT" << endl;
	}
}

bool CommandLineListener::validateParamLess(string param)
{
	bool isParamLess = param == "";

	if (!isParamLess)
		cout << "No parameters expected" << endl;

	return isParamLess;
}

bool CommandLineListener::validateShort(string param)
{
	try 
	{
		int num = stoi(param); 
		(short)num;
	}
	catch (exception ex)
	{
		cout << "expecting short parameter " << endl;
		return false;
	}

	return true;
}

bool CommandLineListener::validateString(string param,string paramname) 
{
	bool isEmpty = param == "";
	if (isEmpty) 
		cout << "expecting string parameter " << paramname << endl;

	return !isEmpty;
}



CommandLineListener & CommandLineListener::operator=(const CommandLineListener & other)
{
	return *this;
}
