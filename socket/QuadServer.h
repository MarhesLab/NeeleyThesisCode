#ifndef QUAD_SERVER_H
#define QUAD_SERVER_H

#define PORT_NUM "3490"
#define MAX_LISTEN 5
#define MAX_DATASIZE 100

#include <string.h>	// memset
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <signal.h>

class QuadServer {
  protected:
	int sockfd, client_fd;	// file descriptors for the server socket and comm-with-client socket
	struct addrinfo hints;		// used for setup
	struct addrinfo *serv;  // points to the results of the setup process

	struct sockaddr_storage client_addr;	// info about the connected client
	socklen_t client_addr_size;
	
  public:
	QuadServer();
	~QuadServer();

	void init();
	void listenNow();
	void reapDeadProcesses();
	void acceptNow();
	void sendData(char data[], int data_size);
	bool receiveData(char data[], int &data_size);
	bool demandData( char data[], int bytes_wanted);
};

#endif
