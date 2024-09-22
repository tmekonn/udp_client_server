#include "Udp.hpp"
#include <thread>
#include <chrono>
#include "Common.hpp"

int main(){
    

   
    
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);

    UDP udp(UE_PORT);
    udp.start();

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(eNodeB_PORT);

      if (inet_pton(AF_INET, eNodeB_ip.c_str(), &server_addr.sin_addr) <= 0){
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }  

    while(true){
        char message[] = "Hello from client";
        struct Request request;
        struct Response response;
        request.type = UPLINK;
        request.ue_id = 1;
        request.data_length = 5;

        //e_udp.send((char *)response, response_len, ue_addr);  
        int n = udp.send((char*)&request, sizeof(request), server_addr);
        
        if(n > 0){
            std::cout << "[C]Sent: " << sizeof(request)<<" bytes"<< std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    return 0;
}