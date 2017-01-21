#include "DataPacket.h"
#include <fstream>


DataPacket::DataPacket(short blockNum, const char* byte, short datasize):TFTPPacket(3),blockNum(blockNum), data(byte), size(datasize)
{

}

void DataPacket::execute(string filename)
{
	ofstream f(filename, ios::out | ios::app | ios::binary);
	f.write(this->data, this->size);
	f.close();
}

const char* DataPacket::getData()
{
	return data;
}

short DataPacket::getSize()
{
	return size;
}

short DataPacket::getBlockNum()
{
	return blockNum;
}

DataPacket::DataPacket(const DataPacket & other):TFTPPacket(3), blockNum(other.blockNum), data(other.data), size(other.size)
{
}

DataPacket & DataPacket::operator=(const DataPacket & other)
{
	if (this != &other) {
		this->blockNum = other.blockNum;
		this->data = other.data;
		this->size = other.size;
	}
	return *this;
}

DataPacket::~DataPacket()
{
	delete data;
}

