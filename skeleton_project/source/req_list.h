#pragma once
#include "stdbool.h"
#include "req.h"
#include "driver/elevio.h"

typedef struct {
    Req rl[N_FLOORS];
    Req front;
    Req rear;
} Req_list;

void add_req(Req new_request, Req_list reqlist);
Req fetch_req();
bool is_empty();