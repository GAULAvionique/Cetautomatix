#include "test_utils.h"
#include <stdbool.h>
#include <stdio.h>
extern int total_tests;
extern int passed_tests;

void TEST_(const char* name, const bool condition){
    if(condition){printf("\tsuccess\t%s\n", name);}
    else{printf("\tfail\t%s\n", name);}
}
void TEST_EQ_INT_(const char* name, const int val_1, const int val_2){
    bool condition = val_1 == val_2;
    if(condition){
        printf("\tsuccess\t%s\n", name);
    passed_tests++;}
    else{
        printf("\tfail\t%s", name);
        printf("\t%d != ", val_1);
        printf("%d\n", val_2);
    }
    total_tests++;
}
