#ifndef GESTION_TAG_H
#define GESTION_TAG_H

// fonction qui gere les tags
/** \brief Initialise/reinitialise le gestionnaire de tags.
 * \note Apres cet appel, l'ensemble des tags doit etre redefini via `set_tag`.
 */

void init_tag_manager(void);

/** \brief Cree un nouveau tag.
 *
 * \param tag_name Le nom (id) relie a la valeur.
 * \param n_bit La taille de la valeur (implementation actuelle: 1..16 bits).
 * \return Un code de succes ou d'erreur.
 */
int set_tag(const char* tag_name, uint8_t n_bit);

#endif /* GESTION_TAG_H */

#ifndef GESTION_TAG_H
#define GESTION_TAG_H
#include <stdint.h>
// fonction qui gere les tags
/** \brief Initialise/reinitialise le gestionnaire de tags.
 * \note Apres cet appel, l'ensemble des tags doit etre redefini via `set_tag`.
 */

void init_tag_manager(void);

/** \brief Cree un nouveau tag.
 *
 * \param tag_name Le nom (id) relie a la valeur.
 * \param n_bit La taille de la valeur (implementation actuelle: 1..16 bits).
 * \return Un code de succes ou d'erreur.
 */
int set_tag(const char* tag_name, uint8_t n_bit);

#endif /* GESTION_TAG_H */
