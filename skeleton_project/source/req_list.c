#include "req_list.h"


//Definisjoner for Req_list
void add_req(Req new_request, Req_list reqlist) {
    
}

Req fetch_req() {
    Req req;
    return req;
}

bool is_empty() {
    return false;
}




//definisjoner for ArrayStruct
void setElement(ArrayStruct as, int index, int value) {
    if (index >= 0 && index < as.size) {
        as.arr[index] = value;
    }
}

int getElement(ArrayStruct as, int index) {
    if (index >= 0 && index < as.size) {
        return as.arr[index];
    }
    return -1;
}