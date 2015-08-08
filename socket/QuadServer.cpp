#include "QuadServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>	// cout

#define DEBUG false

using namespace std;

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get socketaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Constructor
QuadServer::QuadServer()
{
	if(DEBUG)
		cout << "Starting Constructor..." << endl;	

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM; // TCP stream socket
	hints.ai_flags = AI_PASSIVE;	// fill in my IP address for me

	//this.hints.ai_family = AF_UNSPEC; // don't care if IPv4 or IPv6
	hints.ai_family = AF_INET; // specifies IPv4
	//this.hints.ai_family = AF_INET6; // specifies IPv6

	if(DEBUG)
		cout << "Ending Constructor!" << endl;
}

// Destructor
QuadServer::~QuadServer()
{
	if(DEBUG)	
		cout << "Entering destructor..." << endl;	
	
	close(sockfd);
	close(client_fd);
	freeaddrinfo(serv);

	if(DEBUG)
		cout << "Exiting destructor..." << endl;
}


void QuadServer::init()
{
	if(DEBUG)
		cout << "Starting initialization routine..." << endl;

	int yes = 1;	

	// gets the appropriate IP address (as a server, gets its own IP address)	
	if((sockfd = getaddrinfo(NULL, PORT_NUM, &hints, &serv)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(sockfd));
		exit(1);
	}

	if(DEBUG)
		cout << "Got address info!" << endl;

	// gets the file descriptor for the socket
	sockfd = socket(serv->ai_family, serv->ai_socktype, serv->ai_protocol);
	if(sockfd < 0)
	{
		cout << "socket() failed!" << endl;
		exit(1);
	}

	if(DEBUG)
		cout << "Started socket!" << endl;

	// checks for errors with programs already on the port
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		cout << "setsockopt() failed!" << endl;
		exit(1);
	}

	if(DEBUG)
		cout << "Got through setsockopt()!" << endl;

	// binds the socket to an appropriate port on the local machine
	if(bind(sockfd, serv->ai_addr, serv->ai_addrlen) != 0)
	{
		cout << "bind() failed!" << endl;
		perror("server: bind");
		exit(1);
	}

	if(DEBUG)
		cout << "Socket bound! Initialization process exiting..." << endl;
}

void QuadServer::listenNow()
{
	if(DEBUG)
		cout << "Starting listening process..." << endl;

	if(listen(sockfd, MAX_LISTEN) != 0)
	{
		cout << "listen() failed!" << endl;
		exit(1);
	}

	if(DEBUG)
		cout << "Ending listening process..." << endl;
}

void QuadServer::reapDeadProcesses()
{
	struct sigaction sa; 

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		cout << "sigaction() failed!" << endl;
		exit(1);
	}
}

void QuadServer::acceptNow()
{
	if(DEBUG)
		cout << "Starting accept process..." << endl;

	char s[INET6_ADDRSTRLEN];
	
	client_addr_size = sizeof(client_addr);
	client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);	
	if(client_fd == -1)
	{
		cout << "accept() failed!" << endl;
		exit(1);
	}

	if(DEBUG)
		cout << "Accept process finished!" << endl;

	inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr),
		s, sizeof(s));
	cout << "Server: got connection from " << s << endl;
}

void QuadServer::sendData(char data[], int data_size)
{
	if(DEBUG)
		cout << "Starting send process..." << endl;
	
	if(send(client_fd, data, data_size, 0) == -1)
	{
		cout << "send() error!" << endl;
		perror("server send");
	}

	if(DEBUG)
		cout << "Ending send process..." << endl;
}

bool QuadServer::receiveData(char data[], int &data_size)
{
	if(DEBUG)
		cout << "Starting receive process..." << endl;

	data_size = recv(client_fd, data, data_size, 0);

	if(data_size == -1)
	{
		cout << "recv() error!" << endl;
		perror("server receive");
		return false;
	}
	else if(data_size == 0)
	{
		cout << "Didn't receive anything! connection closed..." << endl;
		return false;
	}
	
	data[data_size] = '\0';
	return true;
}


bool QuadServer::demandData(char data[], int bytes_wanted)
{
	if(DEBUG)
		cout << "Starting receive process..." << endl;

	int bytes_stored = 0;
	
	while(bytes_stored != bytes_wanted) {
		int received = recv(client_fd, &data[bytes_stored], bytes_wanted - bytes_stored, 0);
	
		if(received == -1)
		{
			cout << "recv() error!" << endl;
			perror("server receive");
			return false;
		}
		else if(received == 0)
		{
			cout << "Didn't receive anything! connection closed..." << endl;
			return false;
		}

		bytes_stored += received;
	}

	return true;
}
