#include "elevator.h"

//create global array for queue with zero requests
int reqArray[4] = {0,0,0,0};

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
        update_elevator(elev);


        States cs = elev->current_state;
        switch (cs) {
            case UNDEFINED:

                break;

            case IDLE:

                break;

            case WAITING:
                open_and_close_door(elev);
                elevator_expedite_order(elev);
                
                break;

            case MOVING:
                int floor = elevio_floorSensor();
                if (reqArray[floor] == 1) {
                    elev->direction = DIRN_STOP;
                    elev->floor = floor;
                    reqArray[floor] = 0;
                }
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
            if (button_active == 1) {
                reqArray[floor_requested] = 1;
                printf("%d %d %d %d\n", reqArray[0], reqArray[1], reqArray[2], reqArray[3]);
            }
            elevio_buttonLamp(f, b, button_active);
        }
    }
}

void elevator_expedite_order(Elevator* elev) {
    int move_to_floor = 4;
    for (int i = 0; i < N_FLOORS; i++) {
        if (i != elev->floor) {
            if (reqArray[i] == 1) {
                if (abs(i-elev->floor) < abs(move_to_floor-elev->floor)) {
                    move_to_floor = i;
                }
            }
        }
    }
    if (move_to_floor != 4) {
        if (move_to_floor > elev->floor) {
            elev->direction = DIRN_UP;
        } else {
            elev->direction = DIRN_DOWN;
        }
    } else {
        elev->direction = DIRN_STOP;
    }
}

void update_elevator(Elevator* elev) {
    elevio_motorDirection(elev->direction);
    if (elev->direction == DIRN_STOP) {
        elev->current_state = WAITING;
    } else {
        elev->current_state = MOVING;
    }

    /*if (is_empty() == true) {
        elev->current_state = WAITING;
    } else {
        elev->current_state = MOVING;
    }*/
    
}



//req_list functions:
bool is_empty() {
    for (int i = 0; i < N_FLOORS; i++) {
        if (reqArray[i] == 1) {
            return false;
        } else {
            return true;
        }
    }
}

//door functions:
void open_and_close_door(Elevator *elev) {
    time_t start, end;
    start = time(NULL);  // Get current time in seconds
    end = start;

    while (difftime(end, start) < 3) {
        end = time(NULL);  // Get current time in seconds again
        elevator_take_order(elev);
    }
    printf("HELLO\n");
}