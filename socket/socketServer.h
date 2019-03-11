#ifndef __SOCKET_SERVER__
#define __SOCKET_SERVER__

class SocketServer {
private:
	int initUDPServer();
	int initTCPServer();
	int initUDPClient();
	int initTCPClient();
	int initUDP();
	int initTCP();
	
public:
	int init();
	int deInit();
	int socketSend();
	int socketRecv();
	
	void setPort(int port);
	int getPort();

	SocketServer(bool Server, bool Udp = true);
	~SocketServer();

public:
	int mSockFd;
	int mPort;
	struct sockaddr_in mAddr;
	char Ipaddr[20];
	bool isUdp;
	bool isServer;
};

#endif