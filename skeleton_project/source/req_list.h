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



//ArrayStruct
typedef struct {
    int arr[4];  // en req_list trenger å holde maks 4 bestillinger
    int size;     
} ArrayStruct;

void setElement(ArrayStruct as, int index, int value);

int getElement(ArrayStruct as, int index);
