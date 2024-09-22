#include "eNodeB.hpp"

int main(){

    eNodeB enodeb(10, 1000, eNodeB_PORT);// K=1000, N=10
                                  // K will be updated when the first request is received
    enodeb.start();

    return 0;
}