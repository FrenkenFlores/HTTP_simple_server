//
// Created by FrenkenFlores on 01.07.2021.
//

#ifndef HTTP_SIMPLE_SERVER_SERVER_HPP
#define HTTP_SIMPLE_SERVER_SERVER_HPP
#include <string>
#include <sys/select.h>
class Server {
public:
	int init(std::string ip, int port);
	int run();

private:
	int				m_fd;
	int				m_port;
	std::string 	m_ip;
	fd_set			m_set;
};

#endif //HTTP_SIMPLE_SERVER_SERVER_HPP
