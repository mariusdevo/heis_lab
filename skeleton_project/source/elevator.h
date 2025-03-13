#pragma once
#include "time.h"
#include "stdbool.h"
#include "driver/elevio.h"
#include "stdlib.h"

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
    int reqArray[4];
    States current_state;
    States previous_state;
    MotorDirection direction;
    DoorStates door_state;
    int floor;
    int prev_reg_floor;
} Elevator;

void elevator_run(Elevator* elev);
void elevator_init(Elevator* elev);
void elevator_take_order(Elevator* elev);
void elevator_expedite_order(Elevator* elev);
void check_for_stop(Elevator *elev);

//req_list functions:
bool is_empty(Elevator* elev);


//door functions
void open_and_close_door(Elevator *elev);

//stopButton
void stopButton_open_door(Elevator *elev);


