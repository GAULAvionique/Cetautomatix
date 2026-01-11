#ifndef VALVEPAIRE_H_INCLUDED
#define VALVEPAIRE_H_INCLUDED
#include <stdint.h>
typedef struct Vp Vp;

Vp* vp_create();
void vp_destroy(Vp* v);
void vp_set_v(Vp* v, int8_t val, int n);
void vp_set_v1(Vp* v, int8_t val);
void vp_set_v2(Vp* v, int8_t val);
void vp_update_v1(Vp* v, int8_t* val);
void vp_update_v2(Vp* v, int8_t* val);
int8_t vp_get(Vp* v, int n);
void vp_update_v(Vp* v, int8_t* val, int n);
int8_t vp_get_combinaison(Vp* v);
int8_t vp_trad(int8_t* combinaison, int n);

void afficher_binaire(int8_t* n_);
//void vp_(Vp* v, );
//void vp_(Vp* v, );

#endif // VALVEPAIRE_H_INCLUDED
