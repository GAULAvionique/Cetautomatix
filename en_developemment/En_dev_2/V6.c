#include <stdio.h>
#include <stdint.h>
#include "v6.h"






void add_tag(char nom_tag, int n_bit){
    char nom_tag_2;
    // adaptation du nom
    if(n_bit == 1){nom_tag_2 = "1_" + nom_tag + ",";}
    if(n_bit == 3){nom_tag_2 = "3_" + nom_tag + ",";}
    if(n_bit == 4){nom_tag_2 = "4_" + nom_tag + ",";}
    if(n_bit == 8){nom_tag_2 = "8_" + nom_tag + ",";}
    if(n_bit == 16){
        char nom_tag_3;
        nom_tag_2 = "16a_" + nom_tag + ",";
        nom_tag_3 = "16b_" + nom_tag + ",";
    }
}




