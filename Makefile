CXX = g++
CXXFLAGS = -std=c++11 -pthread

all: server client

server: Udp.cpp Server_main.cpp
	$(CXX) $(CXXFLAGS) -o server Udp.cpp Server_main.cpp
client: Udp.cpp Client_main.cpp
	$(CXX) $(CXXFLAGS) -o client Udp.cpp Client_main.cpp



clean:
	rm -r server client