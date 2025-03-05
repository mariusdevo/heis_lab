#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevator.h"

int main(){
   /* Elevator elevator = {{0,0,0,0}};
    elevator_init(&elevator);
    elevator_run(&elevator);*/

    time_t actual = time(NULL);
    time_t duration = 3;
    time_t endwait = actual + duration ;
    printf("Hey");
    int floor = elevio_floorSensor();
    while(actual < endwait){
        actual = time(NULL);
        floor = elevio_floorSensor();
        printf(floor);
    }
    printf("Hey");
    return 0;
}
