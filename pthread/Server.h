#ifndef __SERVER_H__
#define __SERVER_H__

class Server {
public:

	int start();
	int stop();
	int pause();
	int release();

	Server();
	~Server();

public:
};

#endif

