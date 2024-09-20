#include "Udp.hpp"
#include <thread>
#include <chrono>

#define SERVER_PORT 8080
#define CLIENT_PORT 9090

int main(){
    

    std::string server_ip = "127.0.0.1";
    
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);

    UDP udp(CLIENT_PORT);
    udp.start();

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

      if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0){
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }  

    while(true){
        char message[] = "Hello from client";
        int n = udp.send(message, sizeof(message), server_addr);
        
        if(n > 0){
            std::cout << "Sent: " << message << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    return 0;
}