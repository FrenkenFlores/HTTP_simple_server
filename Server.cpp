//
// Created by FrenkenFlores on 01.07.2021.
//
#define BACKLOG 100

#include "Server.hpp"
#include <unistd.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netdb.h>

int Server::init(std::string ip, int port) {
	int yes = 1;
	addrinfo hint;
	addrinfo *hints_list;
	addrinfo *ptr;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	int error = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hint, &hints_list);
	if (error) {
		std::cerr << gai_strerror(error);
		exit(1);
	}
	for (ptr = hints_list; ptr != nullptr; ptr = ptr->ai_next) {
		if ((m_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1) {
			perror("Socket error");
			continue;
		}
		if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
			perror("setsockopt error");
			exit (2);
		}
		if (bind(m_fd, ptr->ai_addr, ptr->ai_addrlen) == -1) {
			close(m_fd);
			perror("Bind error");
			continue;
		}
		break;
	}
	freeaddrinfo(hints_list);
	if (ptr == nullptr) {
		perror("Init error");
		exit(2);
	}
	if (listen(m_fd, BACKLOG) == -1) {
		perror("Listen error");
		exit(3);
	}
	FD_ZERO(&m_set);
	FD_SET(m_fd, &m_set);
	return (0);
}

int Server::run() {
	while (true) {
		fd_set set = m_set;
		timeval timeout = {1, 0};
		if (select(m_fd + 1, &set, nullptr, nullptr, &timeout) > 0) {
			std::cout << "Got connection" << std::endl;

			int client_fd = accept(m_fd, nullptr, nullptr);
			std::ifstream file;
			file.open("../index.html");
			if (file.is_open()) {
				std::string buf;
				std::ostringstream msg;
				msg << "HTTP/1.1 200 OK\r\n";
				msg << "Content-Type: text/html\r\n";
				msg << "\r\n";
				while (std::getline(file, buf))
					msg << buf;
				send(client_fd, msg.str().c_str(), msg.str().size() + 1, 0);
				sleep(2);
				close(client_fd);
			}

		} else {
//			std::cout << "No connections" << std::endl;
			continue;
		}
	}
}