#include "Server.hpp"
#include <iostream>
int main() {
	Server server;

	std::cout << "Connect to 127.0.0.1:8080" << std::endl;
	server.init("0.0.0.0", 8080);
	server.run();
	return 0;
}
