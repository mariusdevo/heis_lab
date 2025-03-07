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
        //update floorindicator
        if (elevio_floorSensor() != -1) {
            elevio_floorIndicator(elevio_floorSensor());
        }
        //necessary floor registration for stopButton
        if (elevio_floorSensor() != -1) {
            elev->prev_reg_floor = elevio_floorSensor();
        }

        elevator_take_order(elev);
        elevio_motorDirection(elev->direction);

        if (elev->direction == DIRN_STOP) {
            elev->current_state = WAITING;
        } else {
            elev->current_state = MOVING;
        }
        if (elevio_stopButton() == 1) {
            elev->current_state = IDLE;
        }

        States cs = elev->current_state;
        int floor;
        switch (cs) {
            case UNDEFINED:
            
            break;
            
            case IDLE:
                elevio_stopLamp(1);
                //stoppknappen er trykket, så vi stopper heisen
                floor = elevio_floorSensor();
                elev->direction = DIRN_STOP;
                elevio_motorDirection(DIRN_STOP);

                //slett køen og fjern alle lys
                for(int i = 0; i < N_FLOORS; i++) {
                    elev->reqArray[i] = 0;
                    if (i == 0) {
                        elevio_buttonLamp(i, 0, 0);
                        elevio_buttonLamp(i, 2, 0);
                    } else if (i == 1 || i == 2) {
                        elevio_buttonLamp(i, 0, 0);
                        elevio_buttonLamp(i, 1, 0);
                        elevio_buttonLamp(i, 2, 0);
                    } else if (i == 3) {
                        elevio_buttonLamp(i, 1, 0);
                        elevio_buttonLamp(i, 2, 0);
                    }
                }
        
                while (elevio_stopButton() == 1) {
                    if (floor != -1) {
                        stopButton_open_door(elev);
                    }
                }
                elev->previous_state = elev->current_state;
                elev->current_state = WAITING;
                elev->floor = -1;
                elevio_stopLamp(0);
                break;
            
            case WAITING:
                if (elev->previous_state == MOVING) {
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
            
            //if stopbutton pressed-> floor=-1 we dont care about floor_requested != elev->floor
            if (button_active == 1 && elev->floor == -1) {
                elev->reqArray[floor_requested] = 1;
                elevio_buttonLamp(f, b, button_active);

            } else if (button_active == 1) {
                elev->reqArray[floor_requested] = 1;
                elevio_buttonLamp(f, b, button_active);
            }
        }
    }
}

void elevator_expedite_order(Elevator* elev) {
    int move_to_floor = -1;  // Start med en "ugyldig" etasje, og dersom det ikke er noen bestillinger bruker vi move_to_floor som -1 til å sette retning til dirn_stop
    for (int i = 0; i < N_FLOORS; i++) {
        if (elev->reqArray[i] == 1 && elev->floor == -1) {
            if (move_to_floor == -1 || abs(i - elev->prev_reg_floor) < abs(move_to_floor - elev->prev_reg_floor)) {
                move_to_floor = i;  // Velg den nærmeste bestilte etasjen
            }
        }

        else if (elev->reqArray[i] == 1 && i != elev->floor) {  // Sjekk om etasjen er bestilt og ikke er nåværende etasje
            if (move_to_floor == -1 || abs(i - elev->floor) < abs(move_to_floor - elev->floor)) {
                move_to_floor = i;  // Velg den nærmeste bestilte etasjen
            }
        }
    }

    int temp_floor;
    if (elev->floor == -1) {
        temp_floor = elev->prev_reg_floor;
    } else {
        temp_floor = elev->floor;
    }

    // Etter å ha valgt etasje, sett retningen:
    if (move_to_floor != -1) {
        if (move_to_floor > temp_floor) {
            elev->direction = DIRN_UP;  
        } else if (move_to_floor == temp_floor) { //spesialtilfelle hvis stoppKnappen er trykket
            if (temp_floor == 0) {
                elev->direction = DIRN_DOWN;
            } else {
                elev->direction = DIRN_UP;
            }
        } else {
            elev->direction = DIRN_DOWN;  
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
    time_t endwait = actual + duration;
    while(actual < endwait){
        actual = time(NULL);
        elevator_take_order(elev);
        while (elevio_obstruction()) {
            actual = time(NULL);
            elevator_take_order(elev);
            endwait = actual + duration;
        }
    }

    elev->door_state = CLOSED;
    elevio_doorOpenLamp(0);
}

//stopButton functions:
void stopButton_open_door(Elevator *elev) {
    elev->door_state = OPEN;
    elevio_doorOpenLamp(1);
    time_t actual = time(NULL);
    time_t duration = 3;
    time_t endwait = actual + duration ;
    while(actual < endwait){

        actual = time(NULL);
        if (elevio_stopButton() == 1) {
            endwait = actual + duration;
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }

        while (elevio_obstruction()) {
            actual = time(NULL);
            endwait = actual + duration;
        }
    }
    elev->door_state = CLOSED;
    elevio_doorOpenLamp(0);
}

void three_second_timer() {
    time_t actual = time(NULL);
    time_t duration = 3;
    time_t endwait = actual + duration ;
    while(actual < endwait){
        actual = time(NULL);
    }
}