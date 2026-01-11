#ifndef DATA1133_H_INCLUDED
#define DATA1133_H_INCLUDED
#include <stdint.h>
#include <stdbool.h>
typedef struct Da31 Da31;

Da31* da31_create();
void da31_destroy(Da31* v);
void da31_update(Da31* v, int8_t* val, int n_val, int n_bit);
void da31_set(Da31* v, int8_t val, int n_val, int n_bit);
void da31_trad(int8_t combinaison, int n_val, int n_bit);
int8_t da31_get_combinaison(Da31* v);

/*
void da31_set_v(Da31* v, int8_t val, int n);
void da31_set_v1(Da31* v, int8_t val);
void da31_set_v1b1(Da31* v, int8_t val);
void da31_set_v1b3(Da31* v, int8_t val);
void da31_set_v2b1(Da31* v, int8_t val);
void da31_set_v2b3(Da31* v, int8_t val);


void da31_set_v2(Da31* v, int8_t val);
void da31_update_val1_bit1(Da31* v, int8_t* val);
void da31_update_val2_bit1(Da31* v, int8_t* val);
void da31_update_val1_bit3(Da31* v, int8_t* val);
void da31_update_val2_bit3(Da31* v, int8_t* val);



void da31_update_v2(Da31* v, int8_t* val, int n_bit);   // n_bit == 1 or 3
void da31_update_v1(Da31* v, int8_t* val, int n_bit);   // n_bit == 1 or 3

//void da31_update_v1(Da31* v, int8_t* val, int n);
*/
bool cnb4(int8_t val);
void ab2(int8_t* n_);

#endif // DATA1133_H_INCLUDED
