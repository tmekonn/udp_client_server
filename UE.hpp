#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <random> 
#include <ctime>  
#include "Udp.hpp"
#include "Common.hpp"


class UE {
    public:
        UE(int port, uint32_t ue_id);
        ~UE();
        void start();
        void run(int L, int T, int m);

    private:
        
        uint32_t u_ue_id;
        UDP u_udp; // UDP client for UE
};