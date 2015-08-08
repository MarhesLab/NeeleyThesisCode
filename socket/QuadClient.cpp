#include "QuadClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define DEBUG false

using namespace std;

// get socketaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


// constructor
QuadClient::QuadClient()
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM; // TCP stream socket

	//this.hints.ai_family = AF_UNSPEC; // don't care if IPv4 or IPv6
	hints.ai_family = AF_INET; // specifies IPv4
	//this.hints.ai_family = AF_INET6; // specifies IPv6
}


QuadClient::~QuadClient()
{
	close(sockfd);
	freeaddrinfo(serv);
}


void QuadClient::init(const char server_ip[])
{
	serv_ip = string(server_ip);

	// gets the appropriate IP address (that of the ground control station)	
	if((sockfd = getaddrinfo(server_ip, PORT_NUM, &hints, &serv)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(sockfd));
		exit(1);
	}

	// gets the file descriptor for the socket
	sockfd = socket(serv->ai_family, serv->ai_socktype, serv->ai_protocol);
	if(sockfd < 0)
	{
		cout << "socket() failed!" << endl;
		exit(1);
	}
}


void QuadClient::connectNow()
{
	cout << "Connecting to " << serv_ip << "..." << endl;

	char s[INET6_ADDRSTRLEN];

	while(connect(sockfd, serv->ai_addr, serv->ai_addrlen) != 0)
	{
		usleep(10000);
	}

	if(DEBUG)
		cout << "Connect process finished!" << endl;

	inet_ntop(serv->ai_family, get_in_addr((struct sockaddr *)serv->ai_addr),
		s, sizeof(s));
	cout << "Client: connected to " << s << endl;
}

void QuadClient::sendData(char data[], int data_size)
{
	if(DEBUG)
		cout << "Starting send process..." << endl;
	
	if(send(sockfd, data, data_size, 0) == -1)
	{
		cout << "send() error!" << endl;
	}

	if(DEBUG)
		cout << "Ending send process..." << endl;
}

bool QuadClient::receiveData(char data[], int &data_size)
{
	if(DEBUG)
		cout << "Starting receive process..." << endl;

	data_size = recv(sockfd, data, data_size, 0);

	if(data_size == -1)
	{
		cout << "recv() error!" << endl;
		return false;
	}
	else if(data_size == 0)
	{
		cout << "Didn't receive anything!" << endl;
		return false;
	}
	
	data[data_size] = '\0';
	return true;
}



bool QuadClient::demandData(char data[], int bytes_wanted)
{
	if(DEBUG)
		cout << "Starting demand process..." << endl;

	int bytes_stored = 0;

	while(bytes_stored != bytes_wanted) {
		int received = recv(sockfd, &data[bytes_stored], bytes_wanted - bytes_stored, 0);
	
		if(received == -1)
		{
			cout << "recv() error!" << endl;
			return false;
		}
		else if(received == 0)
		{
			cout << "Didn't receive anything!" << endl;
			return false;
		}

		bytes_stored += received;
	}

	return true;	
}
