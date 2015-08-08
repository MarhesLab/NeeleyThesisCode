#include <iostream>

#include "QuadServer.h"

using namespace std;

int main()
{
	char message[] = "Testing testing 1 2 3";
	char received[100];	
	int received_size = sizeof(received);

	QuadServer server;
	
	server.init();
	server.listenNow();
	
	server.acceptNow();
	
		
	// Send to the client
	server.sendData(message, sizeof(message));
	
	// Receive from the client
	if(server.receiveData(received, received_size))
		cout << "Received " << received_size << " bytes from client...\n"
			<< "data = \"" << received << "\"" << endl;
	else
		cout << "Nothing received from the client. Exiting..." << endl;

	// Close everything down		
	return 0;
}
