#pragma once
#include "driver/elevio.h"

typedef struct {
    MotorDirection direction;
    int requested_floor;
} Req;

Req create_request(MotorDirection dir, int req_floor);