#ifndef DATA_CONTAINER_H
#include "../../include/sub_include/data_container.h"
#endif // DATA_CONTAINER_H
#ifndef CONFIG_VALUE_H
#include "../../include/config_value.h"
#endif // CONFIG_VALUE_H
#ifndef PRINT_VAL_H
#include "../../include/sub_include/print_val.h"
#endif // PRINT_VAL_H
#include <stdio.h>

uint8_t TxData[N_BYTES];

void init_data_container(void)
{
    for(int i = 0; i < N_BYTES; i++)
    {
        TxData[i] = 0;
    }
}



void set_val_TxData(uint8_t *TxDataArray){
for (int i = 0; i < N_BYTES; i++){
    TxData[i] = TxDataArray[i];
}
}
uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit)
{
// Preparation du masque
    uint8_t mask_val = 0b1;
    uint8_t mask_temp = 0b1;
    // Construit n_bit bits a 1
    for (int i = 0; i < (n_bit - 1); i++)
    {
        mask_val = mask_val << 1;
        mask_val |= mask_temp;
    }
    // Aligne le masque sur la position cible dans le byte
    return mask_val << (8 - p_bit - n_bit);
}

CAN_TG_STATUE set_value(const char* tag_name, uint32_t value)
{
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return -3; // Tag non trouve

    const uint8_t n_bits = tag->n_bits;
    const uint8_t bit_pos_a = tag->bit_pos_a;
    const uint8_t bit_pos_b = tag->bit_pos_b;
    const int8_t byte_idx_a = tag->byte_idx_a;
    const int8_t byte_idx_b = tag->byte_idx_b;

    // Verification du depassement de la valeur selon n_bits
    if (tag->n_bits < 32 && value >= (1ULL << tag->n_bits))
    {
        printf("[ERREUR] Valeur ٪u trop grande pour le tag ٪s (٪u bits)\n", value, tag_name, tag->n_bits);
        return CAN_TG_ERROR_VALUE_TO_BIG;
    }

    // Cas des valeurs sur plus de 8 bits: repartition entre partie A et partie B.
    if(n_bits > 8){
        uint8_t data_b = (value & 0xFF00) >> 8;
        uint8_t data_a = (value & 0x00FF);
        TxData[byte_idx_a] = data_a;
        // Cas 16 bits: la partie B occupe tout le byte.
        if(n_bits == 16){TxData[tag->byte_idx_b] = data_b;}
        else{
            // Cas 9..15 bits: insertion partielle dans le byte B.
            // Creation du masque binaire
            uint8_t mask = prep_mask(bit_pos_b, n_bits - 8);
            // Alignement de la deuxieme partie
            data_b = data_b << (8 - bit_pos_b - n_bits);
            // Mise a jour selective des bits de la partie B.
            TxData[byte_idx_b] &= ~mask;
            TxData[byte_idx_b] |= data_b;
            return CAN_TG_SUCCESS;
        }
    }
    else if(tag->n_bits == 8)
    {
        uint8_t data_a = (value & 0x00FF);
        TxData[tag->byte_idx_a] = data_a;
        return CAN_TG_SUCCESS;
    }
    else{
        // Valeur <= 8 bits (entre 1 et 7 bits inclusivement)
        // Creation du masque binaire aligne sur la zone cible
        uint8_t mask = prep_mask(bit_pos_a, n_bits);
        uint8_t vrai_val = value;
        // Alignement de la valeur dans le byte
        vrai_val = vrai_val << (8 - bit_pos_a - n_bits);
        TxData[byte_idx_a] &= ~mask;
        TxData[byte_idx_a] |= vrai_val;
    }
    return CAN_TG_SUCCESS;
}

uint16_t convert_8_to_16bit(uint8_t ba, uint8_t bb){
    uint16_t r;
    r = ba | (bb << 8);   // Recompose un entier 16 bits depuis 2 octets.
    return r;
}

int get_value(const char* tag_name, uint32_t* out_value)
{
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return CAN_TG_ERROR_TAG_NOT_FOUND_; // Tag non trouve
    // Copie locale des metadonnees du tag pour simplifier les calculs.
    const uint8_t n_bits = tag->n_bits;
    const uint8_t bpa = tag->bit_pos_a;
    const uint8_t bpb = tag->bit_pos_b;
    const int8_t byta = tag->byte_idx_a;
    const int8_t bytb = tag->byte_idx_b;
    if(tag->byte_idx_b > -1){
            // Declaration conditionnelle conservee pour debug/historique.
            const int8_t byte_indx_b = tag->byte_idx_b;
    }
    if (tag->n_bits == 16)    // cas de 16 bits exactement
    {
        uint8_t ba = TxData[byta];
        uint8_t bb = TxData[bytb];
        *out_value = ba | (bb << 8);   // Recomposition 16 bits complete.
    }
    else if((n_bits < 16) && (n_bits > 8)){// cas d'entre 8 et 16 bits.
        uint8_t ba = TxData[byta];
        uint8_t bb;
        // Creation d'un masque pour extraire la partie B.
        uint8_t mask = prep_mask(bpb, n_bits - 8);
        uint8_t data_temp = TxData[bytb] & mask;
        bb = data_temp >> (8 - bpa - n_bits);
        *out_value = ba | (bb << 8);   // Recomposition avec partie B partielle.

    }
    else
    {
    /** \brief Copie un buffer externe dans TxData.
     * \param TxDataArray Tableau source de N_BYTES octets.
     */
        // Creation du masque d'extraction (<= 8 bits)
        uint8_t mask = prep_mask(bpa, n_bits);


        //uint8_t mask = ((1 << n_bits) - 1) << shift;

    /** \brief Recompose un entier 16 bits depuis deux octets.
     * \param ba Octet de poids faible.
     * \param bb Octet de poids fort.
     * \return Valeur 16 bits combinee.
     */

        uint8_t data_temp = TxData[byta] & mask;
        *out_value = data_temp >> (8 - bpa - n_bits);
    }
    return CAN_TG_SUCCESS;
}


/** \brief Simule une transmission CAN locale.
 * \details Copie simplement tx_buffer dans rx_buffer.
 * \param tx_buffer Buffer d'emission.
 * \param rx_buffer Buffer de reception.
 */
void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer)
{
    for (int i = 0; i < N_BYTES; i++)
    {
        rx_buffer[i] = tx_buffer[i];
    }
}
