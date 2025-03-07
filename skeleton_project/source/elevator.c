#include "elevator.h"

void elevator_init(Elevator *elev) {
    elevio_init();
    elevio_motorDirection(DIRN_DOWN);
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            elevio_buttonLamp(f, b, 0);
        }
    }

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
    elev->previous_state = IDLE;
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
                if (elev->previous_state != WAITING) {
                    open_and_close_door(elev);
                    elev->previous_state = elev->current_state;
                }
                elevator_expedite_order(elev);
            
                break;
                
            case MOVING:
                check_for_stop(elev);
                elev->previous_state = elev->current_state;
                break;
            }
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
            if (button_active == 1 && floor_requested != elev->floor) {
                elev->reqArray[floor_requested] = 1;
                //printf("%d %d %d %d\n", reqArray[0], reqArray[1], reqArray[2], reqArray[3]);
                elevio_buttonLamp(f, b, button_active);
            }
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
        elevio_floorIndicator(floor); //only when elevator has stopped do the indicator lamp need to switch
        elev->direction = DIRN_STOP;
        elev->floor = floor;
        elev->reqArray[floor] = 0;
        elevio_buttonLamp(floor, 0, 0);
        elevio_buttonLamp(floor, 1, 0);
        elevio_buttonLamp(floor, 2, 0);
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
    elev->door_state = OPEN;
    elevio_doorOpenLamp(1);
    time_t actual = time(NULL);
    time_t duration = 3;
    time_t endwait = actual + duration ;
    while(actual < endwait){
        actual = time(NULL);
        elevator_take_order(elev);
    }
    elev->door_state = CLOSED;
    elevio_doorOpenLamp(0);
}