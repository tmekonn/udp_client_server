#pragma once

#include "Udp.hpp"
#include <thread>
#include <chrono>
#include <atomic>
#include <csignal>
#include "Common.hpp"

class eNodeB {
    public:
        eNodeB(int N, int K, int port);
        ~eNodeB();
        void start();
        void timer();

    private:

        int e_N;
        int e_K;

        UDP e_udp; // UDP server for eNodeB
        int64_t e_max_advance_scheduling_time;
        int64_t *e_downlink_reserved_subframe;
        int64_t *e_uplink_resrved_subframe;


        bool scheduler(struct Request &request, struct Response &response);
};