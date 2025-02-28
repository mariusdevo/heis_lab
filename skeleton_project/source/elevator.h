#pragma once
#include "time.h"
#include "stdbool.h"
#include "driver/elevio.h"

extern int reqArray[];

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

void elevator_run(Elevator* elev);
void elevator_init(Elevator* elev);
void elevator_take_order(Elevator* elev);
void elevator_expedite_order(Elevator* elev);
void update_elevator(Elevator* elev);
void something(Elevator *elev);



//req_list functions:
bool is_empty();


//door functions
void open_and_close_door(Elevator *elev);


