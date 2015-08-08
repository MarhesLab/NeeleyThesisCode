#ifndef QUAD_CLIENT_H
#define QUAD_CLIENT_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "QuadServer.h" // for the port # and MAX_DATASIZE definitions

class QuadClient {
  protected:
	std::string serv_ip;
	int sockfd;
	struct addrinfo hints;		// used for setup
	struct addrinfo *serv;  // points to the results of the setup process

	
  public:
	QuadClient();
	~QuadClient();

	void init(const char server_ip[]);
	void connectNow();
	void sendData(char data[], int data_size);
	bool receiveData(char data[], int &data_size);
	bool demandData (char data[], int bytes_wanted);
};

#endif
