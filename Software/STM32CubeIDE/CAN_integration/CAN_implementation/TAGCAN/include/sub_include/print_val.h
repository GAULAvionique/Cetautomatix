#ifndef PRINT_VAL_H

#include <stdio.h>
#include <stdint.h>

#define PRINT_VAL_H

// Utilitaires d'affichage pour le débogage
/** \brief imprime dans le terminal la représentation 8 bits (binaire) de la valeur passé en argument
 *
 * \param val la valeurs à voire au format mentionner précédament.
 */

void print_bin_8(uint8_t val);
void print_all_data_bin(void);
void print_all_with_title(const char* title);

void print_all_tag_set(void);
void print_all_data(void);
void print_repartition_bit(void);
typedef struct{
    char bi[8];

} tsfp;

extern tsfp gbc2[9];

#endif /* PRINT_VAL_H */
