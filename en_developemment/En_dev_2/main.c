#include "ValvePaire.h"
#include <stdio.h>
#include "data1133.h"
#include <stdbool.h>

int test_vp(){
    /********************************************//**
         * \brief test principal de la struc de la gestion des valves.
         test les éléments suivant:
         - set
         - get_combi
         - trad
        dans des condition normal.
     *
     * \param
     * \param
     * \return
     *
     ***********************************************/

    // create
    Vp* v1 = vp_create();
    int8_t v_1, v_2, result_combi, re1_, re2_;
    v_1 = 3;
    v_2 = 9;
    vp_set_v1(v1,v_1);
    vp_set_v2(v1,v_2);
    result_combi = vp_get_combinaison(v1);
    printf("result combinaison: ");
    afficher_binaire(&result_combi);
    re1_ = vp_trad(&result_combi, 1);
    re2_ = vp_trad(&result_combi, 2);
    printf("retrad: ");
    afficher_binaire(&re1_);
    afficher_binaire(&re2_);
    return 0;
}

int test_vp_bad_val(){
    /********************************************//**
         * \brief testé la struc vp avec de mauvaise valeurs pour confirmé sont comportement
     *
     * \return 0 = succès et fin de l'exécution du test.
     *
     ***********************************************/

    // create
    Vp* v1 = vp_create();
    int8_t v_1, v_2, result_combi, re1_, re2_;
    v_1 = 9999;
    v_2 = 9897897;
    vp_set_v1(v1,v_1);
    vp_set_v2(v1,v_2);
    result_combi = vp_get_combinaison(v1);
    printf("result combinaison: ");
    afficher_binaire(&result_combi);
    re1_ = vp_trad(&result_combi, 1);
    re2_ = vp_trad(&result_combi, 2);
    printf("retrad: ");
    afficher_binaire(&re1_);
    afficher_binaire(&re2_);
    printf("comparer les resultats.");
    bool r1, r2;
    r1 = re1_ == vp_get(v1, 1);
    r2 = re2_ == vp_get(v1, 2);
    return 0;
}

// la gestion des ordres de msg se fait via une chaine de caractère, la rendent ainci très modulable.
// cette chaine de cactère fait ensuite référence a des variable interne d'un sturc.

int main() {
    test_vp();
    test_vp_bad_val();
    // Create an "object" using the constructor
    Vp* valvep = vp_create();
    Da31* da = da31_create();
    int8_t aj = 7;
    da31_set(da,aj, 1, 3);
    if (valvep != NULL){
        vp_set_v1(valvep, 4);
        vp_set_v2(valvep, 1);
        int8_t vr = vp_get_combinaison(valvep);
        printf("%X\n",vr);
        printf("---");
        int8_t result1, result2;
        result1 = vp_trad(&vr, 1);
        result2 = vp_trad(&vr, 2);
        ab2(&result1);
        ab2(&result2);
        printf("fin result");
        vp_destroy(valvep);
    }
    else{
        printf("vp is NULL");
    }
    da31_destroy(da);
    return 0;
}
