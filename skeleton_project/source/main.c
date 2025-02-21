#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



int main(){
    elevio_init();
    //hello bitch
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");
    elevio_motorDirection(DIRN_DOWN);

    while(1){
        int floor = elevio_floorSensor();

        if(floor == 0){
            elevio_motorDirection(DIRN_STOP);
        }
    }

    return 0;
}
