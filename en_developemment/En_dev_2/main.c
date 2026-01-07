#include "ValvePaire.h"
#include <stdio.h>

int main() {
    // Create an "object" using the constructor
    Vp* valvep = vp_create();
    if (valvep != NULL){
        vp_set_v1(valvep, 4);
        vp_set_v2(valvep, 1);
        int8_t vr = vp_get_combinaison(valvep);
        printf("%X\n",vr);
        printf("---");
        vp_destroy(valvep);
    }
    else{
        printf("vp is NULL");
    }

    return 0;
}
