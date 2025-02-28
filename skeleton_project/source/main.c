#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevator.h"



int main(){
    //Elevator elevator;
    //elevator_init(elevator);
    ArrayStruct list;
    setElement(list, 0, 1);
    setElement(list, 1, 4);
    fprint("first element:", getElement(list,0), "  Second element: ", getElement(list,1), "  size of array: ", list.size);

    return 0;
}
