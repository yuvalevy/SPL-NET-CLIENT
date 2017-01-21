#include "WritePacket.h"
#include <iostream>
#include <fstream>

WritePacket::WritePacket(string fileName) :TFTPPacket(2), fileName(fileName),blockNum(1)
{
	readFile(fileName);
}

WritePacket::WritePacket(const WritePacket & other) :TFTPPacket(2), fileName(fileName), blockNum(1)
{
	readFile(fileName);
}

void WritePacket::readFile(string fileName)
{
	streampos size;

	ifstream file;
	file.open(fileName, ios::out | ios::binary | ios::ate);
	
	if (file.is_open())
	{
		size = file.tellg();
		fileToWrite = new char[size];
		file.seekg(0, ios::beg);
		file.read(fileToWrite, size);
		file.close();
		fileSize = size;
	}

}

string WritePacket::getFileName()
{
	return fileName;
}

DataPacket* WritePacket::getNextPacket()
{
	if (start > fileSize) {
		return nullptr;
	}
	else if (start == fileSize) {
		return new DataPacket(blockNum, new char[0], 0);
	}
	DataPacket* nextPacket = createDataPacket();

	start += maxPacketSize;
	blockNum++;
	return nextPacket;
}

DataPacket* WritePacket::createDataPacket() {

	int sizePacket = maxPacketSize;

	if (start + sizePacket > fileSize) {
		sizePacket = fileSize - start;
	}
	byte* byteData = new byte[sizePacket];
	int index = 0;
	for (int i = start; i < start + sizePacket; i++) {
		byteData[index] = fileToWrite[i];
		index++;
	}

	 return new DataPacket(blockNum, byteData, sizePacket);
}

short WritePacket::getBlockNum()
{
	return blockNum;
}

int WritePacket::getFileSize()
{
	return fileSize;
}

byte * WritePacket::getFileToWrite()
{
	return fileToWrite;
}

WritePacket::~WritePacket()
{
	if (fileToWrite != 0)
		delete fileToWrite;
}

WritePacket & WritePacket::operator=(const WritePacket & other)
{
	return *this;
}