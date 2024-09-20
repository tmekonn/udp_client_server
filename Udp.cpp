#include "Udp.hpp"

// UDP constructor: initialize member variables
UDP::UDP(int p) : p_port(p), u_udp_fd(-1), u_started(false), u_local_address{}, u_remote_address{} {
}

UDP::~UDP(){
    if(u_udp_fd > -1){
        close(u_udp_fd);
    }
}

/*
* Create socket and bind it.
*/
bool UDP::start(){
    if(u_started){
        return true;
    }

    u_udp_fd = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM is for UDP
                                                // SOCK_STREAM is for TCP
    if(u_udp_fd < 0){
        std::cerr << "Failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // initialize the local address
    memset(&u_local_address, 0, sizeof(u_local_address));
    u_local_address.sin_family = AF_INET;
    u_local_address.sin_addr.s_addr = INADDR_ANY;
    u_local_address.sin_port = htons(p_port);

    // bind the socket
    if (bind(u_udp_fd, (struct sockaddr *)&u_local_address, sizeof(u_local_address)) < 0){
        std::cerr << "Failed to bind socket" << std::endl;
        close(u_udp_fd);
        u_udp_fd = -1;
        exit(EXIT_FAILURE);
    }

    u_started = true;

    return u_started;

}

int UDP::receive(char *buf_p, int buf_len, struct sockaddr_in *remote_addr_p){
    
    socklen_t remote_len = sizeof(struct sockaddr_in); // initialize the length of the remote address
    if(! u_started){
        std::cerr << "UDP not started" << std::endl;
        return -1;
    }

    // receive the packet
    int n = recvfrom(u_udp_fd, buf_p, buf_len, 0, (struct sockaddr*)remote_addr_p, (socklen_t *)&remote_len);
    if (n < 0) {
        std::cerr << "Receive failed" << std::endl;
    }

    return n;
}

int UDP::send(char *buf_p, int buf_len, struct sockaddr_in remote_addr){
    if(! u_started){
        std::cerr << "UDP not started" << std::endl;
        return -1;
    }

    // send the packet
    int n = sendto(u_udp_fd, buf_p, buf_len, 0, (struct sockaddr *)&remote_addr, sizeof(remote_addr));
    if (n < 0) {
        std::cerr << "Send failed." << std::endl;
    }

    return n;
}