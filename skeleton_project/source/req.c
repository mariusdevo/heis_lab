#include "req.h"

Req create_request(MotorDirection dir, int req_floor) {
    Req new_request;
    new_request.direction = dir;
    new_request.requested_floor = req_floor;
    return new_request;
}