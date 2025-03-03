#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevator.h"

int main(){
    Elevator elevator = {{0,0,0,0}};
    elevator_init(&elevator);
    elevator_run(&elevator);

    return 0;
}
