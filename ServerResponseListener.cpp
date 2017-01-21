#include "ServerResponseListener.h"


ServerResponseListener::ServerResponseListener(ClientState * state, ConnectionHandler * handler) :handler(handler), clientState(state), encdec(MessageEncoderDecoder())
{
}

ServerResponseListener::ServerResponseListener(const ServerResponseListener & other) : handler(other.handler), clientState(other.clientState), encdec(other.encdec)
{
}

ServerResponseListener & ServerResponseListener::operator=(const ServerResponseListener & other)
{
	if (this != &other) {
		this->handler = other.handler;
		this->clientState = other.clientState;
		this->encdec = other.encdec;
	}
	return *this;
}

void ServerResponseListener::run()
{
	char single[1];

	while (!(!clientState->getConnection() && clientState->getState() == State::DISCONNECTING))
	{
		if (handler->getBytes(single, 1))
		{
			TFTPPacket* packet = encdec.decodeNextByte(single[0]);
			if (packet != nullptr)
				handlePacket(packet);
		}
	}
}

ServerResponseListener::~ServerResponseListener()
{
}

void ServerResponseListener::handlePacket(TFTPPacket* packet) 
{
	short opCode = packet->getOpcode();

	switch (opCode)
	{
	case 3:
		handleData(packet);
		break;
	case 4:
		handleAck(packet);
		break;
	case 5:
		handleError(packet);
		break;
	case 9:
		printBCast(packet);
		break;
	}

}

void ServerResponseListener::sendPacket(TFTPPacket* packet)
{
	byte* byt = encdec.encode(packet);
	handler->sendBytes(byt, encdec.getPacketSize());
}

void ServerResponseListener::handleData(TFTPPacket* packet)
{
	DataPacket* data = static_cast<DataPacket*>(packet);

	if (clientState->getState() == State::READ)
	{
		short blocknum = clientState->getBlockNum();
		if (data->getBlockNum() == blocknum)
		{
			data->execute(clientState->getFileName());

			AckPacket* ack = new AckPacket(data->getBlockNum());
			sendPacket(ack);
			delete ack;

			if (data->getSize() < 512) // last data
			{
				cout << "RRQ " << clientState->getFileName() << " complete" << endl;;
				clientState->initState();
			}
			else
				clientState->setBlockNum(blocknum + 1);
		}
		else sendError(0);
	}
	else if (clientState->getState() == State::DIRLIST)
	{
		short blocknum = clientState->getBlockNum();
		if (data->getBlockNum() == blocknum)
		{
			AckPacket* ack = new AckPacket(data->getBlockNum());
			sendPacket(ack);
			delete ack;

			clientState->setNextBytes(data->getData(), data->getSize());
			string str = clientState->getNextString();
			while (str != "")
			{
				cout << str << endl;
				str = clientState->getNextString();
			}

			if (data->getSize() < 512) // last data
				clientState->initState();
			else
				clientState->setBlockNum(blocknum + 1);
		}
		else sendError(0);
	}
	else sendError(0);
	
}

void ServerResponseListener::handleAck(TFTPPacket* packet)
{
	AckPacket* ack = static_cast<AckPacket*>(packet);
	cout << "ACK " << ack->getBlockNum() << endl;
	State curState = clientState->getState();
	if (curState == State::LOGININGIN)
	{
		clientState->setConnection(true);
		clientState->setState(State::RUTINE);
	}
	else if (curState == State::DISCONNECTING)
		clientState->setConnection(false);
	else if (curState == State::WRITE) {
		int blockNum = clientState->getBlockNum();
		if (ack->getBlockNum() == blockNum)
		{
			WritePacket* wPacket = clientState->getWritePacket();
			DataPacket* data = wPacket->getNextPacket();

			if (data == nullptr) // last data was sent
			{
				cout << "WRQ " << clientState->getFileName() << " complete"<<endl;
				clientState->initState();
			}
			else
			{	
				blockNum++;
				clientState->setBlockNum(blockNum);
				sendPacket(data);
				delete data;
			}
		}
		else sendError(0);
	}

}

void ServerResponseListener::sendError(short code)
{
	ErrorPacket* error = new ErrorPacket(code);
	sendPacket(error);
	delete error;
	clientState->initState();
}

void ServerResponseListener::handleError(TFTPPacket* packet) {
	ErrorPacket* error = static_cast<ErrorPacket*>(packet);

	if (clientState->getState() == State::READ)
		remove(clientState->getFileName().c_str());
	clientState->initState();

	cout << "Error " << error->getErrorCode() << endl;

	delete error;
}

void ServerResponseListener::printBCast(TFTPPacket* packet)
{
	BCastPacket* bcast = static_cast<BCastPacket*>(packet);

	string added("add");
	if (bcast->getAdded() == '0')
		added= "del";

	cout << "BCAST " << added<<" " << bcast->getFileName()<<endl;
	delete bcast;
}

