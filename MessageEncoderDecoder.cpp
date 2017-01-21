#include "MessageEncoderDecoder.h"


MessageEncoderDecoder::MessageEncoderDecoder() :packet(vector<byte>()), opCodeBytes(new byte[2]), opCodeSize(0), opCode(-1), state(), sizeExpected(0), encodedPacketSize(0)
{
}

TFTPPacket* MessageEncoderDecoder::next(byte nextByte) {

	TFTPPacket* packetToRet = nullptr;

	if (opCode == -1)
	{
		opCode = BytesToShort(opCodeBytes);
		init();
	}

	switch (state)
	{
	case ENDER:
	{
		if (nextByte == ender) {
			if (opCode == 5)
				packetToRet = createErrorPacket();
			else if (opCode == 9)
				packetToRet = createBCastPacket();
		}
		if (packetToRet == nullptr)
			packet.push_back(nextByte);

		break;
	}
	case ACK:
	{
		packet.push_back(nextByte);
		if (((int)packet.size()) == sizeExpected)
			packetToRet = createAckPacket();
		break;
	}
	case DATA:
	{
		int size = (int)packet.size();
		packet.push_back(nextByte);
		if (size == 2)
			sizeExpected = 4 + BytesToShort(packet);
		else if (size > 2 && size == sizeExpected)
			packetToRet = createDataPacket();
		break;
	}
	}

	if (packetToRet != nullptr) {
		packet.clear();
		opCodeSize = 0;
		opCode = -1;
		sizeExpected = -1;
	}
	return packetToRet;
}

ErrorPacket* MessageEncoderDecoder::createErrorPacket() {

	if (packet.size() >= 2)
	{
		byte* arrPacket = new byte[packet.size()];
		copy(0, packet.size(), packet, arrPacket);
		short errorCode = BytesToShort(arrPacket);
		arrPacket += 2;

		string errorMessage = string(arrPacket,packet.size()-2);
		arrPacket -= 2;
		delete[] arrPacket;

		return new ErrorPacket(errorCode, errorMessage);
	}
	return nullptr;
}

BCastPacket* MessageEncoderDecoder::createBCastPacket() {
	
	char added = packet.at(0) & 0xff;
	byte* arrPacket = new byte[packet.size()-1];
	copy(1, packet.size(), packet, arrPacket);
	string fileName = string(arrPacket, packet.size() - 1); 
	delete[] arrPacket;

	return new BCastPacket(fileName, added);
}

AckPacket* MessageEncoderDecoder::createAckPacket() {
	short blockNum = BytesToShort(packet);
	return new AckPacket(blockNum);
}

DataPacket* MessageEncoderDecoder::createDataPacket() {
	byte* arrPacket = new byte[4];
	copy(0, 4, packet, arrPacket);

	short dataSize = BytesToShort(arrPacket);
	arrPacket += 2;
	short blockNum = BytesToShort(arrPacket);
	arrPacket += 2;

	arrPacket -= 4;
	delete[] arrPacket;
	size_t strsize = packet.size() - 4;
	byte* data = new byte[strsize];
	copy(4, packet.size(), packet, data);

	return new DataPacket(blockNum, data, dataSize);
}

void MessageEncoderDecoder::init() {
	switch (opCode) {
	case 3:
		state = ReadState::DATA;
		sizeExpected = -1;
		break;

	case 4:
		state = ReadState::ACK;
		sizeExpected = 2;
		break;

	case 5://error
		state = ReadState::ENDER;
		break;

	case 9://bcast
		state = ReadState::ENDER;
		break;
	}
}

TFTPPacket* MessageEncoderDecoder::decodeNextByte(byte nextByte) {
	
	if (opCodeSize >= 2)
	{
		return next(nextByte);
	}

	opCodeBytes[opCodeSize] = nextByte;
	opCodeSize++;
	return nullptr;
}

byte* MessageEncoderDecoder::encode(TFTPPacket* pckt) {

	switch (pckt->getOpcode())
	{
	case 1:
		return encodeRead(pckt);
	case 2:
		return encodeWrite(pckt);
	case 3:
		return encodeData(pckt);
	case 4:
		return encodeAck(pckt);
	case 5:
		return encodeError(pckt);
	case 6:
		return encodeDirlist(pckt);
	case 7:
		return encodeLogin(pckt);
	case 8:
		return encodeDelete(pckt);
	case 9:
		return encodeBCast(pckt);
	case 10:
		return encodeDisconnect(pckt);
	}
	
	return nullptr;
}

short MessageEncoderDecoder::getPacketSize()
{
	return encodedPacketSize;
}

byte* MessageEncoderDecoder::encodeBCast(TFTPPacket* pckt) {

	BCastPacket* bcastPacket = static_cast<BCastPacket*>(pckt);
	short opCode=bcastPacket->getOpcode();
	string fileName=bcastPacket->getFileName();
	char added = bcastPacket->getAdded();
	short packetSize = 4 + fileName.size();
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];
	insertShort(opCode, bytes); bytes += 2;
	bytes[0] = added; bytes++;
	insertString(fileName, bytes); bytes += fileName.size();
	bytes[0] = ENDER; bytes++;

	bytes -= packetSize;
	return bytes;
}

MessageEncoderDecoder::~MessageEncoderDecoder()
{
}

MessageEncoderDecoder::MessageEncoderDecoder(const MessageEncoderDecoder & other) :packet(other.packet), opCodeBytes(other.opCodeBytes), opCodeSize(other.opCodeSize), opCode(other.opCode), state(other.state), sizeExpected(other.sizeExpected), encodedPacketSize(other.encodedPacketSize)
{
}

MessageEncoderDecoder & MessageEncoderDecoder::operator=(const MessageEncoderDecoder & other)
{
	if (this != &other) {
		this->encodedPacketSize = other.encodedPacketSize;
		this->opCode = other.opCode;
		this->opCodeBytes = other.opCodeBytes;
		this->opCodeSize = other.opCodeSize;
		this->packet = other.packet;
		this->sizeExpected = other.sizeExpected;
		this->state = other.state;
	}
	return *this;
}

byte* MessageEncoderDecoder::encodeLogin(TFTPPacket* pckt)
{
	LoginPacket* loginPacket = static_cast<LoginPacket*>(pckt);
	string userName = loginPacket->getUserName();
	short opcode = loginPacket->getOpcode();

	short packetSize = 3 + ((short )userName.size());
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];

	insertShort(opcode, bytes); bytes += 2;
	insertString(userName, bytes); bytes += userName.size();
	insertEnder(bytes); bytes += 1;

	bytes -= packetSize;
	return bytes;
}

byte * MessageEncoderDecoder::encodeDelete(TFTPPacket * pckt)
{
	DeletePacket* deletePacket = static_cast<DeletePacket*>(pckt);
	string fileName = deletePacket->getFileName();
	short opcode = deletePacket->getOpcode();

	short packetSize = 3 +((short) fileName.size());
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];

	insertShort(opcode, bytes); bytes += 2;
	insertString(fileName, bytes); bytes += fileName.size();
	insertEnder(bytes); bytes += 1;

	bytes -= packetSize;
	return bytes;
}

byte * MessageEncoderDecoder::encodeRead(TFTPPacket * pckt)
{
	ReadPacket* readPacket = static_cast<ReadPacket*>(pckt);
	string fileName = readPacket->getFileName();
	short opcode = readPacket->getOpcode();

	short packetSize = 3 + ((short)fileName.size());
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];

	insertShort(opcode, bytes); bytes += 2;
	insertString(fileName, bytes); bytes += fileName.size();
	insertEnder(bytes); bytes += 1;

	bytes -= packetSize;
	return bytes;
}

byte * MessageEncoderDecoder::encodeWrite(TFTPPacket * pckt)
{
	WritePacket* writePacket = static_cast<WritePacket*>(pckt);
	string fileName = writePacket->getFileName();
	short opcode = writePacket->getOpcode();

	short packetSize = 3 + ((short)fileName.size());
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];

	insertShort(opcode, bytes); bytes += 2;
	insertString(fileName, bytes); bytes += fileName.size();
	insertEnder(bytes); bytes += 1;

	bytes -= packetSize;
	return bytes;
}

byte * MessageEncoderDecoder::encodeDirlist(TFTPPacket * pckt)
{
	byte* bytes = new byte[2];
	this->encodedPacketSize = 2;
	DirListPacket* drlstPacket = static_cast<DirListPacket*>(pckt);
	insertShort(drlstPacket->getOpcode(), bytes);
	
	return bytes;
}

byte * MessageEncoderDecoder::encodeData(TFTPPacket * pckt)
{
	DataPacket* dataPacket = static_cast<DataPacket*>(pckt);
	const byte* data = dataPacket->getData();
	short datasize = dataPacket->getSize();
	short blocknumber = dataPacket->getBlockNum();
	short opcode = dataPacket->getOpcode();

	short packetSize = 6 + datasize;
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];

	insertShort(opcode, bytes); bytes += 2;
	insertShort(datasize, bytes); bytes += 2;
	insertShort(blocknumber, bytes); bytes += 2;
	insertBytes(data, bytes, datasize); bytes += datasize;

	bytes -= packetSize;
	return bytes;
}

byte * MessageEncoderDecoder::encodeAck(TFTPPacket * pckt)
{
	AckPacket* ackPacket = static_cast<AckPacket*>(pckt);
	short blocknum = ackPacket->getBlockNum();
	short opcode = ackPacket->getOpcode();

	short packetSize = 4;
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];

	insertShort(opcode, bytes); bytes += 2;
	insertShort(blocknum, bytes); bytes += 2;
	
	bytes -= packetSize;
	return bytes;
}

byte * MessageEncoderDecoder::encodeError(TFTPPacket * pckt)
{
	ErrorPacket* errorPacket = static_cast<ErrorPacket*>(pckt);
	string msg = errorPacket->getErrorMessage();
	short errcode = errorPacket->getErrorCode();
	short opcode = errorPacket->getOpcode();

	// cannot be filesize more than 32000...
	short packetSize = 5 + ((short)msg.size());
	this->encodedPacketSize = packetSize;
	byte* bytes = new byte[packetSize];

	insertShort(opcode, bytes); bytes += 2;
	insertShort(errcode, bytes); bytes += 2;
	insertString(msg, bytes); bytes += msg.size();
	insertEnder(bytes); bytes += 1;

	bytes -= packetSize;
	return bytes;
}

byte * MessageEncoderDecoder::encodeDisconnect(TFTPPacket * pckt)
{
	byte* bytes = new byte[2];
	this->encodedPacketSize = 2;
	DisconnectPacket* disPacket = static_cast<DisconnectPacket*>(pckt);
	insertShort(disPacket->getOpcode(), bytes);
	return bytes;
}

void MessageEncoderDecoder::insertBytes(const byte * origin, byte * target, short size)
{
	for (short i = 0; i < size; i++)
	{
		target[i] = origin[i];
	}
}

void MessageEncoderDecoder::insertShort(short num, byte* target)
{
	target[0] = ((num >> 8) & 0xFF);
	target[1] = (num & 0xFF);
}

void MessageEncoderDecoder::insertString(string str, byte* target)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		target[i] = str.at(i);
	}
}

void MessageEncoderDecoder::insertEnder(byte* target)
{
	target[0] = ender;
}

short MessageEncoderDecoder::BytesToShort(vector<byte> field) {
	short result = (short)((field.at(0) & 0xff) << 8);
	result += (short)(field.at(1) & 0xff);
	return result;
}

short MessageEncoderDecoder::BytesToShort(byte* field) {
	short result = (short)((field[0] & 0xff) << 8);
	result += (short)(field[1] & 0xff);
	return result;
}

void MessageEncoderDecoder::copy(int startIndex, int endIndex, vector<byte> from, byte* toArray) {
	int index = 0;
	for (int i = startIndex; i < endIndex; i++)
	{	

		toArray[index] = from.at(i);
		index++;
	}
}