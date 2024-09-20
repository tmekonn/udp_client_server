#include "Udp.hpp"
#include <thread>
#include <chrono>

#define PORT 8080

int main(){
    UDP udp(PORT);
    udp.start();

    while(true){
        char buf[1024];
        struct sockaddr_in remote_addr;
        int n = udp.receive(buf, sizeof(buf), &remote_addr);
        if(n > 0){
            buf[n] = '\0';
            std::cout << "Received: " << buf << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}