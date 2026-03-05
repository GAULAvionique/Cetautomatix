#ifndef DATA_CONTAINER_H

#include <stdint.h>
#include "tag_manager.h"

#define DATA_CONTAINER_H

// Représente le buffer de données (le payload CAN de 8 octets)
extern uint8_t TagData[N_BYTES];    /**< Représente le buffer de données (le payload CAN de 8 octets) */

/** \brief clean le TxData affin d'être certain de ne pas avoir de donnée poubelle dedans.
 *      Si réexécuté, supprime toute les donné à l'interieurs.
 */

void init_data_container(void);

// Fonctions principales (getters / setters)
/** \brief change la valeur relier au tag.
 * \warning ne dois pas dépasser la taille désigner lors de l'initialisation du tag.
     * \details Change la valeur présent dans TxData à l'endroit spécifier
      automatiquement par le gestionnaire de tag lors de l'initialisation du tag en question.
 * \param tag_name: le tag relier à la valeurs.
 * \param value:    la nouvelle valeurs.
 * \return  le code désigant le succès ou le code d'erreur de l'opération.
 */

int set_value(const char* tag_name, uint32_t value);

/** \brief obtien la valeur désigner par le tag.
 *
 * \param
 * \param la référence de la valeurs où il faut placer la valeurs extraite.
    (comme 'return' est utiliser pour le code d'exécution
 * \return renvoie 1 si réussi, sinon, renvoie le code d'erreur (dans des nombres négatif.)
 *
 */
int get_value(const char* tag_name, uint32_t* out_value);


/** \brief [FONCTION INTERNE] prépare le mask utiliser pour set/get la valeur.
 * (uniquement utiliser pour les valeurs de moins de 8 bits)
 * \param p_bit:    la position du premier bit de la valeur (de gauche à droite)
 * \param n_bit:    le nombre de bits de la valeurs.
 * \return Un masque binaire pour conserver uniquement la valeurs d'intéress
 *    ou retirer uniqment la valeurs d'intéres.
 *
 */
uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit);

/** \brief permet de send des data d'une deuxième manière (au lieux d'utiliser un array qui point au même endrois, il repointe l'array à chaque envoie)
 *
 * \param TxDataA:  Un pointeur vers un array de 8 valeurs
 * \warning Les valeurs de l'array serront overwrite pour laisser place au valeurs du tableau de valeurs.
 *      En théorie, il devrais garder les lien en mémoire, mais je ne peux le garentir.
 * \return l'état de fin de la fonction.
 *
 */

CAN_TG_STATUE manual_send_data(uint8_t *TxDataA);
// Simulation de transmission (remplace send/receve python)
void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer);


uint8_t* simple_get_data_to_send(void);

//int CAN_RECEVE


#endif // DATA_CONTAINER_H
