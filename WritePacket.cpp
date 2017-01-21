#include "WritePacket.h"
#include <iostream>
#include <fstream>

WritePacket::WritePacket(string file) :TFTPPacket(2), fileName(file), fileToWrite(), start(0), blockNum(1), fileSize(0)
{
	readFile(file);
}

WritePacket::WritePacket(const WritePacket & other) : TFTPPacket(2), fileName(other.fileName), fileToWrite(other.fileToWrite), start(other.start), blockNum(other.blockNum), fileSize(other.fileSize)
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
	if (this != &other) {
		this->blockNum = other.blockNum;
		this->fileName = other.fileName;
		this->fileSize = other.fileSize;
		this->fileToWrite = other.fileToWrite;
		this->start = other.start;
	}
	return *this;
}