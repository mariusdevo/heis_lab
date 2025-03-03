#include "elevator.h"

void elevator_init(Elevator *elev) {
    elevio_init();
    elevio_motorDirection(DIRN_DOWN);

    while(1){
        int floor = elevio_floorSensor();
        if(floor == 0){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
    }
    elev->current_state = WAITING;
    elev->direction = DIRN_STOP;
    elev->door_state = CLOSED;
    elev->floor = 0;
}

void elevator_run(Elevator *elev) {

    while(1) {
        elevator_take_order(elev);
        elevio_motorDirection(elev->direction);
        if (elev->direction == DIRN_STOP) {
            elev->current_state = WAITING;
        } else {
            elev->current_state = MOVING;
        }

        States cs = elev->current_state;
        switch (cs) {
            case UNDEFINED:

                break;

            case IDLE:

                break;

            case WAITING:
                //open_and_close_door(elev);
                elevator_expedite_order(elev);
                
                break;

            case MOVING:
                //printf("HELLO\n");
                check_for_stop(elev);
                break;
        }
        //nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
}

void elevator_take_order(Elevator *elev) {
    int button_active;
    int floor_requested;
    int button_pressed;
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            button_active = elevio_callButton(f, b);
            floor_requested = f;
            button_pressed = b;
            if (button_active == 1) {
                elev->reqArray[floor_requested] = 1;
                //printf("%d %d %d %d\n", reqArray[0], reqArray[1], reqArray[2], reqArray[3]);
            }
            elevio_buttonLamp(f, b, button_active);
        }
    }
}

void elevator_expedite_order(Elevator* elev) {
    int move_to_floor = -1;  // Start med en "ugyldig" etasje
    for (int i = 0; i < N_FLOORS; i++) {
        if (elev->reqArray[i] == 1 && i != elev->floor) {  // Sjekk om etasjen er bestilt og ikke er nåværende etasje
            if (move_to_floor == -1 || abs(i - elev->floor) < abs(move_to_floor - elev->floor)) {
                move_to_floor = i;  // Velg den nærmeste bestilte etasjen
            }
        }
    }

    // Etter å ha valgt etasje, sett retningen:
    if (move_to_floor != -1) {
        if (move_to_floor > elev->floor) {
            elev->direction = DIRN_UP;  // Beveg oppover
        } else {
            elev->direction = DIRN_DOWN;  // Beveg nedover
        }
    } else {
        elev->direction = DIRN_STOP;  // Stopp heisen hvis ingen etasje er valgt
    }

}

void check_for_stop(Elevator *elev) {
    int floor = elevio_floorSensor();
    if (floor != -1 && elev->reqArray[floor] == 1) {
        elev->direction = DIRN_STOP;
        elev->floor = floor;
        elev->reqArray[floor] = 0;
    }
}



//req_list functions:
bool is_empty(Elevator* elev) {
    for (int i = 0; i < N_FLOORS; i++) {
        if (elev->reqArray[i] == 1) {
            return false;
        }
    }
    return true;
}

//door functions:
void open_and_close_door(Elevator *elev) {
    time_t actual = time(NULL);
    time_t duration = 3;
    time_t endwait = actual + duration ;
    printf("Hey");
    while(actual < endwait){
        actual = time(NULL);
    }
    printf("Hey");
}