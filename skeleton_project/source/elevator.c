#include "elevator.h"

void elevator_run(Elevator elev) {
    States cs = elev.current_state;

    switch (cs) {
        case UNDEFINED:

            break;

        case IDLE:

            break;

        case WAITING:

            break;

        case MOVING:

            break;

        case REACHED_FLOOR:

            break;
    }
}

void elevator_init(Elevator elev) {
    elevio_init();
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");
    elevio_motorDirection(DIRN_DOWN);

    while(1){
        int floor = elevio_floorSensor();
        if(floor == 0){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
    }

    elev.current_state = WAITING;
    elev.direction = DIRN_STOP;
    elev.door_state = CLOSED;
    elev.floor = 0;
}