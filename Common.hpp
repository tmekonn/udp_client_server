#pragma once

#include <cstdint>
#include <string>

#define eNodeB_PORT 8080
#define UE_PORT 9090



enum RequestType {UPLINK, DOWNLINK};

struct Request {
    uint32_t ue_id;
    RequestType type;
    int data_length;  // L
};

struct Response {
    bool success;
    uint32_t ue_id;
};

enum UEMode {UPLINK_ONLY, DOWNLINK_ONLY, MIXED};

