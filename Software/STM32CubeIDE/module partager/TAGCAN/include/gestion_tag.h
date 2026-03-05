#ifndef GESTION_TAG_H
#define GESTION_TAG_H

// fonction qui gère les tags
/** \brief Initialise/réinitialise le gestionnaire de tags.
 * \note après sont appel, l'ensemble des tags doivent être redéfinie avec l'aide de `set_tag`
 */

void init_tag_manager(void);

/** \brief crée un nouveau tag.
 *
 * \param tag_name  le nom (id) qui serras relier à la valeurs
 * \param n_bit     la taille de la valeur.
 *      ne peux être que 1, 3, 4, 8 ou 16. Sinon, une erreur est générer.
 * \return un code décrivant le succès de l'opération ou (dans le cas d'une valeurs négatif) le code d'erreur correspondent.
 */
int set_tag(const char* tag_name, uint8_t n_bit);

#endif /* GESTION_TAG_H */
