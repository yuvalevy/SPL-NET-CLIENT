#include "ConnectionHandler.h"
#include "CommandLineListener.h"
#include "ServerResponseListener.h"
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>

using namespace std;

int main(int argc, char **argv)
{

//	if (argc < 3) {
	//	cerr << "Usage: " << argv[0] << " host port" << endl << endl;
		//return -1;
	//}
	//string host = argv[1];
	//short port = (short)atoi(argv[2]);

	string host ="192.168.1.11";
	short port = 7777;

	ConnectionHandler* connectionHandler = new ConnectionHandler(host, port);
	if (!connectionHandler->connect()) {
		cerr << "Cannot connect to " << host << ":" << port << endl;
		return 1;
	}
	
	ClientState* state = new ClientState();

	CommandLineListener commandLineListener(state, connectionHandler);
	ServerResponseListener serverResponseListener(state, connectionHandler);
	boost::thread t1(&CommandLineListener::run, &commandLineListener);
	boost::thread t2(&ServerResponseListener::run, &serverResponseListener);

	t1.join();
	t2.join();

	connectionHandler->close();
	return 0;
}