#ifndef TAG_MANAGER_H
#define TAG_MANAGER_H

#ifndef CONFIG_VALUE_H
#include "../config_value.h"
#endif // CONFIG_VALUE_H
#include <stdint.h>
#include <stdbool.h>


/**
 * \brief structure représentent un tag.
 */
typedef struct
{
    char name[MAX_TAG_NAME_LEN];    /**< Nom du tag (il dois être unique et être d'une longueur inférieur à la longueur maximal spécifier par MAX_TAG_NAME_LEN) */
    uint8_t n_bits;    /**< Taille de la valeur (peux être 1, 3, 4, 8, ou 16. si 0, le tag n'ais juste pas encore utilisé) */         // 1, 3, 4, 8, ou 16
    int8_t byte_idx_a;    /**< Index du byte (0 à 7), -1 si non utilisé */
    int8_t byte_idx_b;    /**< Deuxième index pour les variables 16 bits, sinon -1 */
    uint8_t bit_pos_a;      /**< Position du premier bit (de gauche à droite). exemple x0000000 serras 0 et 0x000000 seras 1 */
    uint8_t bit_pos_b;      /**< Position du premier bit (de la deuxième partie en cas d'une valeurs entre 9 et 16 bits. 0 sinon) (de gauche à droite). exemple x0000000 serras 0 et 0x000000 seras 1 */
} TagDef;

// Gestionnaire global de tags

extern TagDef g_tags[MAX_TAGS];    /**< Gestionnaire global de tags. contient l'ensemble des tags (actif et inactif) du gestionnaire de tag */
extern uint8_t g_tag_count;    /**< le nombre de tags actuel (à chaque création réussi d'un nouveau, celui-ci augmente de 1) max: MAX_TAGS */
// Allocation des bits dans chaque byte (0 à 7)
extern uint8_t g_bytes_used[N_BYTES];    /**< Allocation des bits dans chaque byte (0 à 7). (il représente le nombre de bits actuellement assigner à une valeurs dans chacun des bytes) */

// structure de multi gestionnaire de tags (en dévelopement)

/*typedef struct {


}*/

//--------------------------------fonction utilisé de manière général ------------------------
/** \brief recherche le tag et retourne la référence de la structure du tags corespondent au nom.
 * \warning pas suposé être utilisé à l'externe.
 * \param tag_name  le nom du tag à chercher.
 * \return la référence de la structure du tags.
 */
const TagDef* get_tag_def(const char* tag_name);

#endif // TAG_MANAGER_H
