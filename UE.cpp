#include "UE.hpp"
#include "Common.hpp"

std::string eNodeB_ip = "127.0.0.1";

UE::UE(int port, uint32_t ue_id) : u_ue_id(ue_id), u_udp(port) {
       // start UDP client
    if(!u_udp.start()){
        std::cerr << "Failed to start UDP client" << std::endl;
        exit(EXIT_FAILURE);
    } 

}

UE::~UE() {
}

void UE::start(){
}

void UE::run(int L, int T, int m){
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); 
    std::uniform_int_distribution<int> dist(0, 1);
    struct Request request;
    request.type = (RequestType)dist(rng);
    request.data_length = L;
    request.ue_id = u_ue_id;

    struct sockaddr_in eNodeB_addr;
    socklen_t eNodeB_addr_len = sizeof(eNodeB_addr);

    memset(&eNodeB_addr, 0, sizeof(eNodeB_addr));
    eNodeB_addr.sin_family = AF_INET;
    eNodeB_addr.sin_port = htons(eNodeB_PORT);

    if (inet_pton(AF_INET, eNodeB_ip.c_str(), &eNodeB_addr.sin_addr) <= 0){
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto start_time = std::chrono::steady_clock::now();
    while (true){
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
   
        if(elapsed_time > T){
            std::cout << "UE" << u_ue_id << " stopped." << std::endl;
            break;
        }
        this->u_udp.send((char*)&request, sizeof(request), eNodeB_addr);

        char response_buff[1024];
        int n = this->u_udp.receive(response_buff, sizeof(response_buff), &eNodeB_addr);
        if(n > 0){
            struct Response *response = (struct Response *)response_buff;
            std::cout << "UE" << u_ue_id << " received response: " << response->success << std::endl;
        }
    }


}
