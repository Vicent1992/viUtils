#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> 
#include <signal.h>
#include <sys/syscall.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#define LOG_TAG "socketServer"

#include "socketServer.h"
#include "log.h"

int SocketServer::socketSend()
{
	int senbyte = 0;
	char message_data[1024];

	log_func;

	memset(message_data, 0, sizeof(message_data));
	strcpy(message_data, "hello a server!");
    senbyte = sendto(mSockFd, message_data, sizeof(message_data), 0,
                     (struct sockaddr *)&mAddr,sizeof(struct sockaddr_in));

    log_dbg("socketSend:\n\tsenbyte = %d \n\tmessage_data:%s\n",senbyte, message_data);
	return 0;
}


int SocketServer::socketRecv()
{
	int recbyte = 0;
	unsigned int addr_len = sizeof(struct sockaddr_in);
	char databuffer[1024];

	log_func;

	recbyte = recvfrom(mSockFd, databuffer, sizeof(databuffer), 0 , \
					   (struct sockaddr *)&mAddr ,&addr_len);
	log_dbg("socketRecv:\n\trecbyte = %d \n\tdatabuffer:%s\n\n",recbyte, databuffer);

	return 0;
}


int SocketServer::initUDPServer()
{
    int optval = 1;
	int senbyte = 0;

    int message_num = 0;
	char message_data[1024];

	log_func;


    if((mSockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        log_err("init socket error\n");
		return -1;
    }

	setsockopt(mSockFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int)); 
    bzero(&mAddr,sizeof(mAddr));
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(mPort);
    mAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	return 0;
}

int SocketServer::initUDPClient()
{
	int optval = 1; 
	struct timeval ti;	 
	ti.tv_sec = 1; 
	ti.tv_usec = 1000 * 300;

	log_func;

	if((mSockFd = socket(AF_INET,SOCK_DGRAM, 0)) < 0){
		log_err("init socket error\n");
		return -1;
	}

	setsockopt(mSockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)); 
	bzero (&mAddr, sizeof(mAddr));
	mAddr.sin_family=AF_INET;
	mAddr.sin_port=htons(mPort);
	mAddr.sin_addr.s_addr=htonl(INADDR_ANY) ;
	if (bind(mSockFd, (struct sockaddr *)&mAddr, sizeof(mAddr))<0) {
		log_err("bind socket error\n");
		return -1;
	}
	setsockopt(mSockFd,SOL_SOCKET,SO_RCVTIMEO,&ti,sizeof(ti));

	return 0;
}

int SocketServer::initUDP()
{
	int ret;
	log_func;
	if(isServer)
		ret = initUDPServer();
	else 
		ret = initUDPClient();
	return ret;
}

int SocketServer::initTCPServer()
{
	log_func;
	return 0;
}

int SocketServer::initTCPClient()
{
	log_func;
	return 0;

}

int SocketServer::initTCP()
{
	int ret;
	log_func;

	if(isServer)
		ret = initTCPServer();
	else 
		ret = initTCPServer();

	return ret;

}

int SocketServer::init()
{
	int ret;
	log_func;

	if(isUdp)
		ret = initUDP();
	else 
		ret = initTCP();

	return ret;
		
}

int SocketServer::deInit()
{
	log_func;

	if (mSockFd > 0)
		close(mSockFd);
    mSockFd = -1;
}

SocketServer::SocketServer(bool Server, bool Udp)
	:isUdp(Udp)
	,isServer(Server)
	,mPort(8888)
	,mSockFd(-1)
{
	log_func;
	log_dbg("isServer %d isUdp %d\n", isServer, isUdp);

}

SocketServer::~SocketServer()
{
	log_func;

}

void SocketServer::setPort(int port)
{
	log_func;
	mPort = port;
}

int SocketServer::getPort()
{
	log_func;
	return mPort;
}

static int exit_flag = 0;

void sigint_handler(int sig)
{
    exit_flag = 1;
}

int main(int argc, char*argv[])
{
	bool isServer; 
	bool isUdp;

	if (argc < 3) {
		log_err("%s [bool isServer] [bool isUdp]\n", argv[0]);
		return 0;
	}

	isServer = atoi(argv[1]);
	isUdp = atoi(argv[2]);

	SocketServer *SS = new SocketServer(isServer, isUdp);
	if (SS == NULL) {
		log_err("SocketServer instantiation failed");
		return -1;
	}
		

	signal(SIGINT, sigint_handler);

	if (SS->init() != 0) {
		log_err("SocketServer init failed");
		return -1;
	}
		

	while (!exit_flag) {
		if (isServer) {
			SS->socketSend();
			sleep(1);
		} else {
			SS->socketRecv();
		}
			
	}

	SS->deInit();

	if(SS != NULL) {
		delete SS;
		SS = NULL;
	}
	
	log_dbg("vicent ====== hello\n");
	return 0;
}
