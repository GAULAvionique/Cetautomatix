#ifndef DATA_CONTAINER_H

#include <stdint.h>

#include "tag_manager.h"

#define DATA_CONTAINER_H

// Buffer de donnees CAN (payload de 8 octets)
extern uint8_t TxData[N_BYTES];    /**< Buffer de donnees CAN (payload de 8 octets). */

/** \brief Reinitialise le payload TxData a zero.
 * \details Si reexecutee, la fonction efface les donnees precedemment ecrites.
 */

void init_data_container(void);

// Fonctions principales (set/get)
/** \brief Ecrit une valeur dans TxData a partir d'un tag.
 * \warning La valeur doit tenir dans la taille du tag.
 * \details En cas de depassement, l'implementation retourne CAN_TG_ERROR_VALUE_TO_BIG.
 * \param tag_name Nom du tag cible.
 * \param value Nouvelle valeur a ecrire.
 * \return Code de succes ou code d'erreur.
 */

int set_value(const char* tag_name, uint32_t value);

/** \brief Lit la valeur associee a un tag.
 *
 * \param tag_name Nom du tag a lire.
 * \param out_value Adresse de sortie qui recoit la valeur extraite.
 * \return CAN_TG_SUCCESS en cas de succes, sinon un code d'erreur negatif.
 *
 */
int get_value(const char* tag_name, uint32_t* out_value);


/** \brief [FONCTION INTERNE] Prepare un masque binaire pour set/get.
 * \details Utilise principalement pour les valeurs strictement inferieures a 8 bits.
 * \param p_bit Position du premier bit (de gauche a droite).
 * \param n_bit Nombre de bits de la valeur.
 * \return Masque binaire aligne sur la position cible.
 *
 */
uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit);
/** \brief Simule un envoi/reception CAN par copie buffer-a-buffer.
 * \param tx_buffer Buffer source (emission).
 * \param rx_buffer Buffer destination (reception).
 */
void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer);

//int CAN_RECEVE

/** \brief Copie un tableau externe dans le buffer global TxData.
 * \param TxDataArray Tableau source de N_BYTES octets.
 */
void set_val_TxData(uint8_t *TxDataArray);


#endif // DATA_CONTAINER_H
