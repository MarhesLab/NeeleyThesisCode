#include <iostream>

#include "QuadClient.h"

using namespace std;

int main()
{
	char message[] = "Message received!";
	char received[100];	
	int received_size = sizeof(received);
	
	QuadClient client;

	client.init("127.0.0.1");
	client.connectNow();

	// Receive data from the server
	if(client.receiveData(received, received_size))
		cout << "Received " << received_size << " bytes from server\n"
			<< "data = \"" << received << "\"" << endl;
	else
		cout << "Nothing received from the client. Exiting..." << endl;

	// Send data to the server
	client.sendData(message, sizeof(message));
	
	// Close everything down		
	return 0;
}
