#include "data1133.h"
#include <stdlib.h> // for malloc/free
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h> // Pour CHAR_BI

#define MASK_3BIT ((1U << 0) | (1U << 1) | (1U << 2))
#define MASK_1BIT 1U

int8_t make_masque(int n_bit){

};

bool cnb4(int8_t val){
    //TODO: À IMPLÉMENTÉ;
    if((val&0xF0) == 0){return true;}
    else{return false;}
}
void ab2(int8_t* n_) {
    /********************************************//**
         * \brief affiche au format binaire la valeur passé en argument.
     *
     * \param la valeurs à print.
     *
     ***********************************************/

    int i;
    int8_t n = *n_;
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
};
void print_bit(int8_t* val, char msg){
    /********************************************//**
         * \brief imprimer les bits d'une valeurs, mais avec un messages en plus.
     *
     * \param
     * \param
     * \return
     *
     ***********************************************/

    printf(&msg);
    ab2(val);
};
struct Da31{
    int8_t* v1; /**< val1, 1 bit */
    int8_t* v2; /**< val2, 1 bit */
    int8_t* v3;/**< val1, 3 bits */
    int8_t* v4;/**< val2, 3 bits */
};

// constructor
Da31* da31_create(){
    /********************************************//**
         * \brief la création d'un objet de gestion des donnée de la forme binaire suivantes:
          xyaaabbb
          où chaque lettre différente représente une donnée.
     *
     * \param
     * \param
     * \return
     *
     ***********************************************/

    Da31* v = (Da31*)malloc(sizeof(struct Da31));
    if (v != NULL){
        int8_t v1, v2, v3, v4;
        v1 = 0;v2 = 0; v3 = 0; v4 = 0;
        v->v1 = &v1;
        v->v2 = &v2;
        v->v3 = &v3;
        v->v4 = &v4;
    };
    return v;
};
void da31_destroy(Da31* v){
    /********************************************//**
         * \brief suppression de cette objet (appel obligatoire affin de ne pas avoir de fuite de mémoire !!)
     *
     * \param Da31*
     *
     ***********************************************/

    free(v);
};

//int8_t* da31-prepa_val_n_bit()

void da31_update(Da31* v, int8_t* val, int n_val, int n_bit){
    /********************************************//**
         * \brief update par référence la valeurs contenu à la bonne position et au bon nombre de bit
     *
     * \param v: la struct. instencier.
     * \param val: pointeur vers la nouvelle valeur (peut être le même pointeur que la valeur elle même)
     * \param n_val: 1 ou 2
     * \param n_bit: 1 ou 3
     * \return
     *
     ***********************************************/

    ab2(val);
    printf("val\n");
    //print_bit(val, "test");
    int8_t te;
    if(n_val == 1 || n_val == 2){
    // conformisation de la donné
    if(n_bit == 1){
        te = *val & MASK_1BIT;
        if(te == *val){
            if(n_val == 1){
                v->v1 = val;
            }
            else{
                v->v2 = val;
            };
        }
        else{
            printf("la donnée passé à été modifier car elle n'ais pas conforme au nombre de bit de la donnée. Donnée: ");printf(val);
            printf(" valeur mise: ");
            printf(&te);printf("\n");
            if(n_val == 1){v->v1 = &te;}
            else{v->v2 = &te;}
        }
    }
    else{
        if(n_bit == 3){
            te = *val & MASK_3BIT;
            if(te == *val){
                if(n_val == 1){v->v3 = val;}
                else{v->v4 = val;}
            }
            else{printf("...");
                printf("la donnée passé à été modifier car elle n'ais pas conforme au nombre de bit de la donnée. Donnée: ");printf(val);
                printf(" valeur mise: ");
                printf(&te);printf("\n");
                if(n_val == 1){v->v3 = &te;}
                else{v->v4 = &te;}
            }}
        else{
            printf("indice de bit invalide. doit être 1 ou 3");
        }
    };
    //free(&n_val);
    //free(&n_bit);}
}};
void da31_set(Da31* v, int8_t val, int n_val, int n_bit){
    /** \brief  Modifier l'une des valeurs (des 4 valeurs que ce gestionnaire gère)
     *       avec une copie de la valeur passé en argument
     *
     * \param val: la valeur désiré
     * \param n_val: le numéro de la valeur (1 ou 2)
     * \param n_bit: le nombre de bit(s) de la valeur. (1 ou 3)
     * \return void
     *
     */

    da31_update(v, &val, n_val, n_bit);
    //free(&n_val);
    //free(&n_bit);
};
/*void da31_trad(int8_t combinaison, int n_val, int n_bit){

};*/
/*int8_t da31_get_combinaison(Da31* v){
    int8_t m1, m2, m3, m4;
    // preparation des valeurs sous forme de masque
    m1 = *(v->v1);
};*/

/*
void da31_update_val2_bit1(Da31* v, int8_t* val){
    int8_t vt = (*val << 1) & 0xF ;
    ab2(&vt);
    int8_t masque = *val & 0xF; v->v1 = &masque;}    // masque is use to be sur to have a 4 bit value
//void da31_update_v2(Da31* v, int8_t* val){int8_t masque = *val & 0xF; v->v2 = &masque;}
//void da31_update_v3(Da31* v, int8_t* val){int8_t masque = *val & 0xF; v->v2 = &masque;}
//void da31_update_v4(Da31* v, int8_t* val){int8_t masque = *val & 0xF; v->v2 = &masque;}

//void da31_update_v2(Da31* v, int8_t* val){v->v2 = val;}

void da31_update_val_bit1(Da31* v, int8_t* val, int n){
    if(n == 0){da31_update_val1_bit1(v, val);}
    else{da31_update_val2_bit1(v, val);}
}*/
int8_t da31_get_combinaison(Da31* v){
    /********************************************//**
         * \brief obtenir la version combiner des différentes valeurs sous forme d'un seul int8_t.
     *
     * \param v: la struc. instancier.
     * \return la combinaisons des valeurs
     *
     ***********************************************/

    ab2(v->v1);
    int8_t v9 = (*(v->v2)<<3) | *(v->v1);
    ab2(&v9);
    int8_t result = *(v->v2)<<4 | *(v->v1);
    return result;
};
