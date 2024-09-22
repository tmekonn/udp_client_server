#include "UE.hpp"
#include <cstdlib>  // for atoi
#include <getopt.h> // for getopt
void runUEs(int M, int T, int L, int m) {
    std::vector<std::thread> UE_threads;

    for (int i = 0; i < M; ++i) {
        uint32_t ue_id = i + 1; // Assign unique UE ID
        UE* ue = new UE(UE_PORT + i, ue_id); // Create UE instance with unique port and ID
        UE_threads.emplace_back(&UE::run, ue, L, T, m); // Start UE thread
    }

    for (auto& thread : UE_threads) {
        thread.join(); // Wait for all threads to finish
    }
}

int main(int argc, char* argv[]) {
    // Set default values for M, T, L, and m
    int M = 5;        // Default value for M: number of UEs
    int T = 60000;    // Default value for T: duration of the simulation
    int L = 100;      // Default value for L: data length
    int m = DOWNLINK_ONLY;        // Default value for m: UE mode

    // Parse command-line arguments using getopt
    int opt;
    while ((opt = getopt(argc, argv, "M:T:L:m:")) != -1) {
        switch (opt) {
            case 'M':
                M = std::atoi(optarg); // Convert the argument to an integer
                break;
            case 'T':
                T = std::atoi(optarg);
                break;
            case 'L':
                L = std::atoi(optarg);
                break;
            case 'm':
                m = std::atoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [-M <value>] [-T <value>] [-L <value>] [-m <value>]\n";
                return 1;
        }
    }

     runUEs(M, T, L, m);
    return 0;
}