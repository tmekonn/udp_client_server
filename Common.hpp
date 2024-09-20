#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

enum RequestType { UPLINK, DOWNLINK };

struct Request {
    uint32_t ue_id;
    RequestType type;
    int data_length;  // L
};

struct Response {
    bool success;
    uint32_t ue_id;
};

enum UEMode { UPLINK_ONLY, DOWNLINK_ONLY, MIXED };

#endif // COMMON_H