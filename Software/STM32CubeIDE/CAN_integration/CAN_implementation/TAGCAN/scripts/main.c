#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#ifndef CONFIG_VALUE_H
#include "../include/config_value.h"
#endif // CONFIG_VALUE_H
#ifndef DATA_CONTAINER_H
#include "../include/sub_include/data_container.h"
#endif // DATA_CONTAINER_H
#ifndef GESTION_TAG_H
#include "../include/gestion_tag.h"
#endif // GESTION_TAG_H
#ifndef PRINT_VAL_H
#include "../include/sub_include/print_val.h"
#endif // PRINT_VAL_H

int passed_tests = 0; /**< le nombre total de test réussi */
int total_tests = 0; /**< le nombre total de test effectué */

/** \brief Affiche un resultat de test avec format uniforme.
 *
 * \param name Le nom du test (peut aussi servir de description courte).
 * \param condition Resultat du test (true si reussi, false sinon).
 *
 */
void TEST(const char* name, const bool condition)
{
    if(condition)
    {
        printf("success\t\t\t%s\n", name);
        passed_tests++;
    }
    else
    {
        printf("fail\t\t\t%s\n", name);
    }
    total_tests++;
}
/** \brief Affiche un test d'egalite entre deux entiers.
 *
 * \param name Meme usage que TEST.
 * \param val_1 Premiere valeur a comparer.
 * \param val_2 Deuxieme valeur a comparer.
 *
 */
void TEST_EQ_INT(const char* name, const int val_1, const int val_2)
{
    bool condition = val_1 == val_2;
    if(condition)
    {
        printf("success\t\t\t%s\n", name);
        passed_tests++;
    }
    else
    {
        printf("fail\t\t\t%s", name);
        printf("\t%d != ", val_1);
        printf("%d\n", val_2);
    }
    total_tests++;
}







/** \brief Execute un scenario de demonstration des fonctions d'impression.
 * \details Initialise des tags/valeurs puis affiche metadonnees et repartition.
 */
void test_print(void){
printf("test des fonction d'impression.\n");
// prepa du gt
init_data_container();
init_tag_manager();

// set the tags
set_tag("a1",2);
set_tag("agh5",5);
set_tag("patap",16);
set_tag("patat",12);
set_tag("uni1",1);
set_tag("triot", 3);

// set the value
set_value("a1",3);
set_value("agh5",42);
set_value("patap",45);
set_value("patat",12);
set_value("uni1",1);
set_value("triot", 3);

print_all_tag_set();
print_all_data();
print_repartition_bit();
}




/** \brief Scenario de test manuel des operations de masque/shift.
 */
void somme_other_test(void){
printf("----------------------test mask ------------------------\n");
    printf("\t(0,8): ");
    print_bin_8(prep_mask(0, 8));
    printf("\n");
    printf("\t(4,4): ");
    print_bin_8(prep_mask(4, 4));
    printf("\n");
    printf("\t(4,3): ");
    print_bin_8(prep_mask(4, 3));
    printf("\n");
    printf("\t(1,4): ");
    print_bin_8(prep_mask(1, 4));
    printf("\n");
    printf("\t(5,1): ");
    print_bin_8(prep_mask(5, 1));
    printf("\n");
    printf("------------------------------test shift----------------------\n");
    uint8_t val1 = 0b01110000;
    uint8_t bi = 0b01110100;
    printf("val1: ");
    print_bin_8(val1);
    printf("\n");
    int p_bit = 4;
    int n_bit = 4;
    // eq logic pour la prise de la valeur
    uint8_t vrai_val = val1 >> (8 - p_bit - n_bit);
    // prepa du mask
    uint8_t mask_val = 0b1;
    uint8_t mask_temp = 0b1;
    // mask des bits de la valeurs
    for (int i = 0; i < (n_bit - 1); i++)
    {
        mask_val = mask_val << 1;
        mask_val |= mask_temp;
    }
    // mask pour conserver uniquement la valeurs dans le bytes
    uint8_t mask_val_p = mask_val << (8 - p_bit - n_bit);

    uint8_t byte_sans_val = bi;
    byte_sans_val &= ~mask_val_p;


    //mask_val = mask_val >> (8 - n_bit);
    printf("vrai_val: ");
    print_bin_8(vrai_val);
    printf("\n");
    printf("mask: ");
    print_bin_8(mask_val);
    printf("\n");
    printf("mask2: ");
    print_bin_8(mask_val_p);
    printf("\n");
    printf("byte avant: ");
    print_bin_8(bi);
    printf("\n");
    printf("byte sans val: ");
    print_bin_8(byte_sans_val);
    printf("\n");


    printf("test final\n");

    init_data_container();
    init_tag_manager();

}

/** \brief Execute les tests unitaires du module.
 *      Les resultats sont affiches dans le terminal.
 *
 */

void run_all_tests(void)
{
    uint32_t val = 0;

    printf("=== Lancement des tests unitaires ===\n");

    // ---------------------------------------------------------
    // TEST 1 : Cas d'erreurs de set_tag
    // ---------------------------------------------------------
    printf("Test 1: Erreurs de set_tag... ");
    init_tag_manager(); // On part d'un état propre

    // Tailles invalides (ex: 0 retourne CAN_TG_ERROR_SIZE_VALUE_INVALIDE)
    printf("\tTailles invalides\n");
    TEST_EQ_INT("0",set_tag("inv1", 0), CAN_TG_ERROR_SIZE_VALUE_INVALIDE);
    TEST_EQ_INT("2",set_tag("inv2", 2), CAN_TG_SUCCESS);
    TEST_EQ_INT("5",set_tag("inv5", 5), CAN_TG_SUCCESS);

    // Tag en double (retour attendu: CAN_TG_ERROR_NAME_ALREADY_USE)
    printf("\tTag en double\n");
    TEST_EQ_INT("première insertion",set_tag("TAG_A", 4), SUCCES_TO_SET); // Succès
    TEST_EQ_INT("duplica détection",set_tag("TAG_A", 3), CAN_TG_ERROR_NAME_ALREADY_USE); // Erreur : existe déjà
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 2 : Dépassement de l'espace mémoire (8 octets max)
    // ---------------------------------------------------------
    printf("Test 2: Remplissage memoire (8 bytes max)... ");
    init_tag_manager();

    // On ajoute 4 tags de 16 bits (4 * 2 = 8 octets complets)
    printf("\tpreparation du GT\n");
    TEST_EQ_INT("ajout de valeurs 16 bit (1/4)",set_tag("T1", 16), SUCCES_TO_SET);
    TEST_EQ_INT("ajout de valeurs 16 bit (2/4)",set_tag("T2", 16), SUCCES_TO_SET);
    TEST_EQ_INT("ajout de valeurs 16 bit (3/4)",set_tag("T3", 16), SUCCES_TO_SET);
    TEST_EQ_INT("ajout de valeurs 16 bit (4/4)",set_tag("T4", 16), SUCCES_TO_SET);

    // Le prochain tag doit etre refuse faute d'espace libre.
    printf("\ttest de Dépassement de l'espace mémoire (8 octets max)\n");
    TEST_EQ_INT("avec 1 bit",set_tag("T5", 1), CAN_TG_ERROR_NOT_ENOUNG_SPACE);
    TEST_EQ_INT("avec 16 bits",set_tag("T6", 16), CAN_TG_ERROR_NOT_ENOUNG_SPACE);
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 3 : Cas d'erreurs de set_value et get_value
    // ---------------------------------------------------------
    printf("Test 3: Erreurs et depassements de valeurs... ");
    init_tag_manager();
    init_data_container();
    printf("\tpreparation du GT\n");
    TEST_EQ_INT("prepa... ",set_tag("BIT_1", 1), SUCCES);
    TEST_EQ_INT("prepa... ",set_tag("BIT_3", 3), SUCCES);
    TEST_EQ_INT("prepa... ",set_tag("BIT_4", 4), SUCCES);

    // Tag inexistant (retour attendu: -3)
    printf("\tTag inexistant\n");
    TEST_EQ_INT("test set",set_value("FANTOME", 5), -3);
    TEST_EQ_INT("test get",get_value("FANTOME", &val), -3);

    // Depassement de capacite:
    // NOTE: l'implementation actuelle renvoie CAN_TG_ERROR_VALUE_TO_BIG (-10),
    // mais ces assertions historiques verifient encore -4.
    printf("\tDépassement de capacité\n");
    // - Un tag de 1 bit ne peut contenir que 0 ou 1
    TEST_EQ_INT("Un tag de 1 bits[0 ou 1]",set_value("BIT_1", 2), -4);
    // - Un tag de 3 bits ne peut contenir que de 0 à 7
    TEST_EQ_INT("Un tag de 3 bits [0 à 7]",set_value("BIT_3", 8), -4);
    // - Un tag de 4 bits ne peut contenir que de 0 à 15
    TEST_EQ_INT("Un tag de 4 bits [0 à 15]", set_value("BIT_4", 16), -4);
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 4 : Indépendance des écritures (Masquage binaire)
    // ---------------------------------------------------------
    printf("Test 4: Indépendance des bits contigus... ");
    init_tag_manager();
    init_data_container();

    // On crée une configuration qui remplit exactement 1 octet (1 + 3 + 4 = 8 bits)
    printf("\tpreparation du GT\n");
    TEST_EQ_INT("preparation [set_tag] (1/3)",set_tag("T_1", 1), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (2/3)",set_tag("T_3", 3), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (3/3)",set_tag("T_4", 4), SUCCES);

    // On assigne les valeurs maximales à chaque tag
    printf("\tpreparation du GT (partie 2)\n");
    TEST_EQ_INT("preparation [set_tag] (1) 1 bits (1/3)",set_value("T_1", 1), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (7) 3 bits (2/3)",set_value("T_3", 7), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (15) 4 bits (3/3)",set_value("T_4", 15), SUCCES);

    // On vérifie qu'elles sont bien mémorisées
    printf("\t\n");
    get_value("T_1", &val);
    TEST_EQ_INT("get value T_1",val, 1);
    get_value("T_3", &val);
    TEST_EQ_INT("get value T_3",val, 7);
    get_value("T_4", &val);
    TEST_EQ_INT("get value T_4",val, 15);

    // On modifie T_3 sans toucher aux autres
    printf("\tmodification de T_3 sans toucher aux autres\n");
    TEST_EQ_INT("",set_value("T_3", 2), 1);

    // On vérifie que T_1 et T_4 n'ont pas bougé
    printf("\t\n");
    get_value("T_1", &val);
    TEST_EQ_INT("verif valeur [encien]",val, 1);
    get_value("T_3", &val);
    TEST_EQ_INT("verif valeur [nouvel]",val, 2); // Nouvelle valeur
    get_value("T_4", &val);
    TEST_EQ_INT("verif valeur [encien]",val, 15);
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 5 : Variables de 8 bits
    // ---------------------------------------------------------
    printf("Test 5: Variables 8 bits... ");
    init_tag_manager();
    init_data_container();
    printf("\t\n");
    TEST_EQ_INT("prapa de la valeur",set_tag("V8", 8), SUCCES);

    // Test des bornes 0 et 255
    printf("\tTest des bornes 0 et 255\n");
    set_value("V8", 0);
    get_value("V8", &val);
    TEST_EQ_INT("val = 0",val, 0);
    set_value("V8", 255);
    get_value("V8", &val);
    TEST_EQ_INT("val = 255",val, 255);
    printf("\tErreur dépassement\n");
    TEST_EQ_INT("val: [256]",set_value("V8", 256), -4); // Assertion historique, a realigner avec -10
    printf("OK\n");

    //--------------------------------------
    // TEST 6: Tout les test de 4 bits
    //--------------------------------------
    printf("\ttest de tags à 4 bits (tout les type possible)\n");
    init_tag_manager();
    init_data_container();
    printf("test d'inserssion des tags\n");
    TEST_EQ_INT("ajout 1 tags à 4 bits ", set_tag("ma_val1", 4),SUCCES);
    TEST_EQ_INT("ajout 1 tags après 4 bits ", set_tag("ma_2", 4),SUCCES);
    printf("le dernier cas n'ais pas encore testé!!!!\n");
    printf("test  d'ajout de valeurs\n");
    TEST_EQ_INT("set val",set_value("ma_val1", 3),1);
    TEST_EQ_INT("set val 2",set_value("ma_2", 9),1);
    get_value("ma_val1", &val);
    TEST_EQ_INT("get val 1",val,3);;
    get_value("ma_2", &val);
    TEST_EQ_INT("get val 2",val,9);;
    //printf("=== TOUS LES TESTS SONT PASSES AVEC SUCCES ! ===\n\n");
}

int main(void)
{


    // Il suffit d'appeler la fonction de test au début de votre main
    run_all_tests();
    test_print();


    // ... suite de votre code (simulation CAN, etc.) ...
    return 0;
}
