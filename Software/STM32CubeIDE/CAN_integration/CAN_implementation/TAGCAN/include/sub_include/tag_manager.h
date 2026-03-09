#ifndef TAG_MANAGER_H
#define TAG_MANAGER_H

#ifndef CONFIG_VALUE_H
#include "../config_value.h"
#endif // CONFIG_VALUE_H
#include <stdint.h>
#include <stdbool.h>


/**
 * \brief Structure de definition d'un tag.
 */
typedef struct
{
    char name[MAX_TAG_NAME_LEN];    /**< Nom logique du tag. Doit etre unique. */
    uint8_t n_bits;    /**< Taille en bits (implementation actuelle: 1..16, 0 si slot inutilise). */
    int8_t byte_idx_a;    /**< Index du byte de la partie A (0..7), -1 si non utilise. */
    int8_t byte_idx_b;    /**< Index du byte de la partie B (tags > 8 bits), sinon -1. */
    uint8_t bit_pos_a;      /**< Position de debut dans le byte A (de gauche a droite). */
    uint8_t bit_pos_b;      /**< Position de debut dans le byte B pour les tags > 8 bits, 0 sinon. */
} TagDef;

// Etat global du gestionnaire de tags

extern TagDef g_tags[MAX_TAGS];    /**< Table des tags (actifs et inactifs). */
extern uint8_t g_tag_count;    /**< Nombre de tags actifs (max: MAX_TAGS). */
// Nombre de bits deja utilises dans chaque byte (0..7)
extern uint8_t g_bytes_used[N_BYTES];    /**< Occupation courante en bits pour chaque byte du payload. */

// structure de multi gestionnaire de tags (en dévelopement)

/*typedef struct {


}*/

//--------------------------------fonctions d'acces generales ------------------------
/** \brief Recherche un tag et retourne sa definition.
 * \warning API surtout utilisee en interne par le module de valeurs.
 * \param tag_name Nom du tag a chercher.
 * \return Pointeur vers le tag si trouve, sinon NULL.
 */
const TagDef* get_tag_def(const char* tag_name);

#endif // TAG_MANAGER_H
