#pragma once
#include "driver/elevio.h"
#include "req_list.h"

typedef enum {
    UNDEFINED = -1,
    IDLE = 0,
    WAITING = 1,
    MOVING = 2,
    REACHED_FLOOR = 3,
} States;

typedef enum {
    OPEN = 0,
    CLOSED = 1,
} DoorStates;

typedef struct {
    States current_state;
    MotorDirection direction;
    DoorStates door_state;
    //int waiting_to_close_door; 1 for waiting, 0 for not waiting
    int floor;
} Elevator;

void elevator_run(Elevator elev);
void elevator_init(Elevator elev);


