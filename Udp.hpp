#pragma once

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

class UDP {
    public:
        UDP(int p);
        ~UDP();
        bool start();
        int receive(char *buf_p, int buf_len, struct sockaddr_in *remote_addr_p);
        int send(char *buf_p, int buf_len, struct sockaddr_in remote_addr);

    private:
        int p_port;
        int u_udp_fd; // file descriptor
        bool u_started;
        struct sockaddr_in u_local_address;
        struct sockaddr_in u_remote_address;
};
