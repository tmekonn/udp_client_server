CXX = g++
CXXFLAGS = -std=c++11 -pthread

all: enodeb client

enodeb: Udp.cpp eNodeB_main.cpp
	$(CXX) $(CXXFLAGS) -o enodeb Udp.cpp eNodeB.cpp eNodeB_main.cpp
client: Udp.cpp UE.cpp UE_main.cpp
	$(CXX) $(CXXFLAGS) -o client Udp.cpp UE.cpp UE_main.cpp



clean:
	rm -r enodeb client 