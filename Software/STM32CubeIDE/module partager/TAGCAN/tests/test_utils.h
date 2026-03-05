#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <stdbool.h>
extern int total_tests;
extern int passed_tests;

//#define TEST(name, condition) \
//    do { \
//        total_tests++; \
//        if (condition) { \
//            printf("\tsuccess\t%s\n", name); \
//            passed_tests++; \
//        } else { \
//            printf("\tfail\t%s\n", name); \
//        } \
//    } while(0)
//
void TEST(const char* name, const bool condition);
void TEST_EQ_INT(const char* name, const int val_1, const int val_2);

#endif
