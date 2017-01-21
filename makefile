CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lboost_locale -lboost_thread

all: TFTPClient 

TFTPClient: bin/AckPacket.o bin/BCastPacket.o bin/ClientState.o bin/CommandLineListener.o bin/ConnectionHandler.o bin/DataPacket.o bin/DeletePacket.o bin/DirListPacket.o bin/DisconnectPacket.o bin/ErrorPacket.o bin/LoginPacket.o bin/MessageEncoderDecoder.o bin/ReadPacket.o bin/ServerResponseListener.o bin/TFTPPacket.o bin/WritePacket.o bin/Main.o 
	@echo 'start TFTPClient'
	g++ -o bin/TFTPclient bin/AckPacket.o bin/BCastPacket.o bin/ClientState.o bin/CommandLineListener.o bin/ConnectionHandler.o bin/DataPacket.o bin/DeletePacket.o bin/DirListPacket.o bin/DisconnectPacket.o bin/ErrorPacket.o bin/LoginPacket.o bin/MessageEncoderDecoder.o bin/ReadPacket.o bin/ServerResponseListener.o bin/TFTPPacket.o bin/WritePacket.o bin/Main.o $(LDFLAGS)
	@echo 'end TFTPClient'

bin/AckPacket.o: src/AckPacket.cpp
	@echo 'make AckPacket'
	g++ $(CFLAGS) -o bin/AckPacket.o src/AckPacket.cpp

bin/BCastPacket.o: src/BCastPacket.cpp
	@echo 'make BCastPacket'
	g++ $(CFLAGS) -o bin/BCastPacket.o src/BCastPacket.cpp

bin/ClientState.o: src/ClientState.cpp
	@echo 'make BCastPacket'
	g++ $(CFLAGS) -o bin/ClientState.o src/ClientState.cpp

bin/DataPacket.o: src/DataPacket.cpp
	g++ $(CFLAGS) -o bin/DataPacket.o src/DataPacket.cpp

bin/DeletePacket.o: src/DeletePacket.cpp
	g++ $(CFLAGS) -o bin/DeletePacket.o src/DeletePacket.cpp

bin/DirListPacket.o: src/DirListPacket.cpp
	g++ $(CFLAGS) -o bin/DirListPacket.o src/DirListPacket.cpp

bin/DisconnectPacket.o: src/DisconnectPacket.cpp
	g++ $(CFLAGS) -o bin/DisconnectPacket.o src/DisconnectPacket.cpp

bin/ErrorPacket.o: src/ErrorPacket.cpp
	g++ $(CFLAGS) -o bin/ErrorPacket.o src/ErrorPacket.cpp

bin/LoginPacket.o: src/LoginPacket.cpp
	g++ $(CFLAGS) -o bin/LoginPacket.o src/LoginPacket.cpp
	
bin/ReadPacket.o: src/ReadPacket.cpp
	g++ $(CFLAGS) -o bin/ReadPacket.o src/ReadPacket.cpp

bin/WritePacket.o: src/WritePacket.cpp
	g++ $(CFLAGS) -o bin/WritePacket.o src/WritePacket.cpp

bin/ServerResponseListener.o: src/ServerResponseListener.cpp
	g++ $(CFLAGS) -o bin/ServerResponseListener.o src/ServerResponseListener.cpp

bin/MessageEncoderDecoder.o: src/MessageEncoderDecoder.cpp
	g++ $(CFLAGS) -o bin/MessageEncoderDecoder.o src/MessageEncoderDecoder.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp
	
bin/CommandLineListener.o: src/CommandLineListener.cpp
	g++ $(CFLAGS) -o bin/CommandLineListener.o src/CommandLineListener.cpp

bin/TFTPPacket.o: src/TFTPPacket.cpp
	g++ $(CFLAGS) -o bin/TFTPPacket.o src/TFTPPacket.cpp

bin/Main.o: src/Main.cpp
	@echo 'make main'
	g++ $(CFLAGS) -o bin/Main.o src/Main.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
