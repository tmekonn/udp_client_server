#include "eNodeB.hpp"


static std::atomic<int64_t> current_time(0);
static std::atomic<bool> eNodeB_stop(false);

static bool K_updated = false;

static int success_count = 0;
static int fail_count = 0;

// Function to print statistics
void printStatistics() {
    std::cout << "Total requests: " << fail_count + success_count << std::endl;
    if (fail_count + success_count > 0) {
        std::cout << "Success rate: " << (success_count * 100) / (fail_count + success_count) << "%" << std::endl;
    } else {
        std::cout << "No requests processed." << std::endl;
    }
}

static void signalHandler(int signum)
{
    if (signum == SIGINT) {
        std::cout << "\nCtrl+C detected. Stopping threads..." << std::endl;
        eNodeB_stop = true;
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }
    return;
}

eNodeB::eNodeB(int N, int K, int port)
    : e_N(N), 
      e_K(K),  
      e_udp(port){
    e_max_advance_scheduling_time = 10*K;
    e_downlink_reserved_subframe = new int64_t[N];
    e_uplink_resrved_subframe = new int64_t[N];

    // initialize reserved subframes to 0
    memset(e_downlink_reserved_subframe, 0, sizeof(e_downlink_reserved_subframe));
    memset(e_uplink_resrved_subframe, 0, sizeof(e_uplink_resrved_subframe));
    
}
eNodeB::~eNodeB(){
    delete[] e_downlink_reserved_subframe;
    delete[] e_uplink_resrved_subframe;
}

void eNodeB::timer(){

    while(!eNodeB_stop){
        current_time++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "eNodeB timer stopped." << std::endl;
}

void eNodeB::start(){
    std::cout << "eNodeB starting..." << std::endl;
    struct Request request;
    struct Response response;

    int request_len = sizeof(struct Request);
    int response_len = sizeof(struct Response);

    struct sockaddr_in ue_addr;
    int received = 0; // number of bytes received
    int err_code = 0;

    if(!e_udp.start()){
        std::cout << "eNodeB failed to start UDP server."<< std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "UDP server started..." << std::endl;

    // set the signal handler for Ctrl+C (SIGINT)
    signal(SIGINT, signalHandler);

    // start timer thread on the current instance of the eNodeB
    std::thread timer_thread(&eNodeB::timer, this);
    

    std::cout << "eNodeB waiting for request..." << std::endl;
    while(!eNodeB_stop){

        received = e_udp.receive((char *)&request, request_len, &ue_addr, &err_code);

     if (received < 0 && err_code == EINTR) {
            std::cout << "KEYBOARD INTERRUPT" << std::endl;
            eNodeB_stop = true;
            break;
        }

        if (received < 0 && err_code == EAGAIN) {
            continue; // Timeout occurred, continue to check stop flag
        }
        // check if we received a valid request data length
        if(received != request_len){
            std::cerr << "Received invalid request." << std::endl;
            continue;
        }

        // update the K value from the first request
        if(!K_updated){
            this->e_K = request.data_length;
            this->e_max_advance_scheduling_time = 10*e_K;
            K_updated = true;
        }
   
        if(scheduler(request, response)){
            
            success_count++;
        }else{
           
            fail_count++;            
                
        }
         // send response to UE
         std::cout << "Sending response to UE ID: " <<request.ue_id<<std::endl;
         e_udp.send((char *)&response, response_len, ue_addr);        

    }
        // Wait for the timer thread to finish
    if (timer_thread.joinable()) {
        timer_thread.join();
    }
 
    printStatistics();
    std::cout << "eNodeB stopped." << std::endl;
    
}
bool eNodeB::scheduler(struct Request &request, struct Response &response){
    
    bool success = false;

    if(request.type == RequestType::UPLINK){
        for (int n = 0; n < e_N; n++){
            // update resrved subframe timestamp 
           if(e_uplink_resrved_subframe[n] < current_time) e_uplink_resrved_subframe[n] = current_time;

           if((e_uplink_resrved_subframe[n] + e_K) < e_max_advance_scheduling_time){
                e_uplink_resrved_subframe[n] += e_K; 
                success = true;
                break;
           }
        }
    } else { // downlink
        for (int n = 0; n < e_N; n++){
            // update resrved subframe timestamp 
           if(e_downlink_reserved_subframe[n] < current_time) e_downlink_reserved_subframe[n] = current_time;

           if((e_downlink_reserved_subframe[n] + e_K) < e_max_advance_scheduling_time){
                e_downlink_reserved_subframe[n] += e_K;    
                success = true;
                break;
           }
        }
    }

    response.success = success;
    response.ue_id = request.ue_id;

    return true;
}