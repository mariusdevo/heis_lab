#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



int main(){
    elevio_init();
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");
    elevio_motorDirection(DIRN_DOWN);
    elevio_doorOpenLamp(0);

    while(1){
        int floor = elevio_floorSensor();

        if(floor == 0){
            elevio_motorDirection(DIRN_STOP);
        }
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
