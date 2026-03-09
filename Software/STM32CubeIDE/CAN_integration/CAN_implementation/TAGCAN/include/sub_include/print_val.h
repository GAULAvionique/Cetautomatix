#ifndef PRINT_VAL_H

#include <stdio.h>
#include <stdint.h>

#define PRINT_VAL_H

// Utilitaires d'affichage pour le debogage
/** \brief Affiche la representation binaire (8 bits) d'une valeur.
 *
 * \param val Valeur a afficher en binaire sur 8 bits.
 */

void print_bin_8(uint8_t val);

/** \brief Affiche tout le payload TxData en binaire.
 * \details Chaque octet est affiche sur 8 bits, dans l'ordre du buffer.
 */
void print_all_data_bin(void);

/** \brief Affiche TxData en binaire avec un titre en prefixe.
 * \param title Texte affiche avant la ligne binaire.
 */
void print_all_with_title(const char* title);

/** \brief Affiche la table des tags declares et leurs metadonnees.
 */
void print_all_tag_set(void);

/** \brief Affiche toutes les valeurs logiques relues via get_value.
 */
void print_all_data(void);

/** \brief Affiche la repartition des bits utilises dans la trame CAN.
 */
void print_repartition_bit(void);

/** \brief Buffer de rendu pour l'affichage de repartition bit-a-bit. */
typedef struct{
    char bi[8];

} tsfp;

/** Espace global de rendu utilise par les fonctions d'impression de repartition. */
extern tsfp gbc2[9];

#endif /* PRINT_VAL_H */
