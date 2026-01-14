
#include "group8.h"
#include <stdlib.h> // for malloc/free
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h> // Pour CHAR_BI

int8_t g8_trad_8(int8_t* combinaison, int n){
    if(n == 1){
        return *combinaison & 0xF;
    }
    else{
        if(n == 2){
            return (*combinaison &0xF0)<<4;
        }
    }
};


void afficher_binaire(int16_t* n_) {
    int i;
    int16_t n = *n_;
    // Détermine le nombre de bits (ex: 32 bits pour un int)
    int nombre_de_bits = sizeof(n) * CHAR_BIT;
    printf("value of b: %X [%d]\n",n,n);
    // Boucle de l'octet de poids fort (le plus à gauche) à l'octet de poids faible (le plus à droite)
    for (i = nombre_de_bits - 1; i >= 0; i--) {
        // Masque pour isoler le bit à la position 'i' (1 << i)
        // ET bit à bit (&) avec le nombre 'n'
        // Si le résultat est non nul, le bit est 1, sinon il est 0
        if ((n & (1 << i)) != 0) {
            printf("1");
        } else {
            printf("0");
        }
        // Optionnel : Ajouter un espace tous les 8 bits pour la lisibilité
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}
struct Group8{
    int8_t* v1; // not suited for system working with less tan 8 bit
    int8_t* v2; // not suited for system working with less tan 8 bit
};

// constructor
Group8* g8_create(){
    /********************************************//**
         * \brief création d'une instance Group8.
     *
     * \return pointeur vers une instance Group8.
     *
     ***********************************************/

    Group8* v = (Group8*)malloc(sizeof(struct Group8));
    if (v != NULL){
        int8_t v1, v2;
        v1 = 0;
        v2 = 0;
        v->v1 = &v1;
        v->v2 = &v2;
    };
    return v;
};
void g8_destroy(Group8* v){free(v);};

int8_t g8_get(Group8* v, int n){
    /********************************************//**
         * \brief obtenir l'une valeurs.
     *
     * \param v: 'instance' de Group8
     * \param n = 1 ou 2    le numéro de la valve. (chaque instance en comprend 2)
     * \return la valeurs à cette instance (il ne s'agit pas du pointeur vers la valeurs.)
     *
     ***********************************************/

    if(n==1 || n==2){
    if(n==1){return *(v->v1);}
    else{return *(v->v2);}
}};


void g8_set_v1(Group8* v, int8_t val){g8_update_v1(v, &val);};
void g8_set_v2(Group8* v, int8_t val){g8_update_v2(v, &val);};

void g8_update_v1(Group8* v, int8_t* val){int8_t masque = *val & 0xF; v->v1 = &masque;}    // masque is use to be sur to have a 4 bit value
void g8_update_v2(Group8* v, int8_t* val){int8_t masque = *val & 0xF; v->v2 = &masque;}

//void g8_update_v2(Group8* v, int8_t* val){v->v2 = val;}

void g8_set_v(Group8* v, int8_t val, int n){
    /********************************************//**
         * \brief définir une valeurs en copien la valeurs passé en segond argument.
     *
     * \param val: la valeurs à placer
     * \param n = 1 ou 2.
     *
     ***********************************************/

    if(n == 0){g8_update_v1(v, &val);}
    else{g8_update_v2(v, &val);}
}
int16_t g8_get_combinaison(Group8* v){
    /********************************************//**
         * \brief traduire la combinaison d'état de sous état en une seul valeurs int16_t.
         ce qui permet de regroupé tout les valeurs dans des bloque de 16 bits.
     *
     * \param
     * \return
     *
     ***********************************************/

    afficher_binaire(v->v1);
    int16_t v9 = (*(v->v2)<<8) | *(v->v1);
    afficher_binaire(&v9);
    int16_t result = *(v->v2)<<8 | *(v->v1);
    return result;
}


