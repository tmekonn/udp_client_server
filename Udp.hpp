#pragma once

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstddef>
#include <unistd.h>


class UDP {
    public:
        /*!
        * Constructor protoype
        */
        UDP(int p);

        /*
        * Destructor prototype
        */
        ~UDP();
        /*
        * start(): creates socket and binds it to port
        */
        bool start();

        /*!
        * isStarted(): 
        */
        bool isStarted();

        /*!
        * receive(): receive packet from remote_addr
        *
        *  @param[in] buf_p     pointer to the buffer for the received data
        */
        int receive(char *buf_p, int buf_len, struct sockaddr_in *remote_addr_p, int *err_code);
        /*!
        * send(): send packet to remote_addr
        *
        * @param[in] buf_p     pointer to the buffer for the data to be transmitted
        * @param[in] buf_len   size of buffer containing the data to be transmitted 
        */
        int send(char *buf_p, int buf_len, struct sockaddr_in remote_addr);

        //int getSocketFd() const; 

    private:
        int u_port;
        int u_udp_fd; // file descriptor
        bool u_started;
        struct sockaddr_in u_local_address;
        struct sockaddr_in u_remote_address;
};
