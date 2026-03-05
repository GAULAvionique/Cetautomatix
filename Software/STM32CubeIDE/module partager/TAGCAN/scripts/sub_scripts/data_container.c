#include "../../include/sub_include/data_container.h"
#include "../../include/config_value.h"
#include "../../include/sub_include/print_val.h"
#include <stdio.h>

uint8_t TagData[N_BYTES];

void init_data_container(void)
{
    for(int i = 0; i < N_BYTES; i++)
    {
        TagData[i] = 0;
    }
}




uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit)
{
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
    return mask_val << (8 - p_bit - n_bit);
}

CAN_TG_STATUE set_value(const char* tag_name, uint32_t value)
{
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return CAN_TG_ERROR_TAG_NOT_FOUND_; // Tag non trouvé

    const uint8_t n_bits = tag->n_bits;
    const uint8_t bit_pos_a = tag->bit_pos_a;
    const uint8_t bit_pos_b = tag->bit_pos_b;
    const int8_t byte_idx_a = tag->byte_idx_a;
    const int8_t byte_idx_b = tag->byte_idx_b;

    // Vérification de débordement de valeur
    if (tag->n_bits < 32 && value >= (1ULL << tag->n_bits))
    {
        printf("[ERREUR] Valeur %u trop grande pour le tag %s (%u bits)\n", value, tag_name, tag->n_bits);
        return CAN_TG_ERROR_VALUE_TO_BIG;
    }


    // dans le cas des valeurs entre 9 et 16 bits, ont extrait d'abord les 2 valeurs désirer et ont les place enssuite dans les endroite correspondent
    if(n_bits > 8){
        print_all_with_title("avant modification");
        uint8_t data_b = (value & 0xFF00) >> 8;
        uint8_t data_a = (value & 0x00FF);
        TagData[byte_idx_a] = data_a;
        print_all_with_title("après modification du a");
        // si la donnée est de 16 bits, simplement la set
        if(n_bits == 16){TagData[tag->byte_idx_b] = data_b;}
        else{
            // ont traite cette valeurs
            // Création du masque binaire
            uint8_t mask = prep_mask(bit_pos_b, n_bits - 8);
            // déplacement de la deuxième partie (de manière adéqua)
            data_b = data_b << (8 - bit_pos_b - n_bits);
            // changement des bits de la deuxième partie.
            TagData[byte_idx_b] &= ~mask;
            //print_all_with_title("sans la valeur b");
            TagData[byte_idx_b] |= data_b;
            //print_all_with_title("avec la nouvelle valeur b");
            return CAN_TG_SUCCESS;
        }
    }
    else if(tag->n_bits == 8)
    {
        uint8_t data_a = (value & 0x00FF);
        TagData[tag->byte_idx_a] = data_a;
        return CAN_TG_SUCCESS;
    }
    else{
        // Valeur <= 8 bits (entre 1 et 7 bits inclusivement)
        // Création du masque binaire aligné à droite puis décalé
        uint8_t mask = prep_mask(bit_pos_a, n_bits);
        uint8_t vrai_val = value;
        //print_all_with_title("avant modif");print_all_data_bin();
        // déplacement de la vrai valeur
        vrai_val = vrai_val << (8 - bit_pos_a - n_bits);
        TagData[byte_idx_a] &= ~mask;
        //print_all_with_title("sans la valeur a");print_all_data_bin();
        TagData[byte_idx_a] |= vrai_val;
    }   //print_all_with_title("avec la valeur a");print_all_data_bin();
    return CAN_TG_SUCCESS;
}

uint16_t convert_8_to_16bit(uint8_t ba, uint8_t bb){
    uint16_t r;
    r = ba | (bb << 8);   // the B should be the one wo can be less tan 8 bit.
    return r;
}

int get_value(const char* tag_name, uint32_t* out_value)
{
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return CAN_TG_ERROR_TAG_NOT_FOUND_; // Tag non trouvé
    // la sortie temporaire des valeurs ici sont juste pour voire les val. lors du débugages.
    const uint8_t n_bits = tag->n_bits;
    const uint8_t bpa = tag->bit_pos_a;
    const uint8_t bpb = tag->bit_pos_b;
    const int8_t byta = tag->byte_idx_a;
    const int8_t bytb = tag->byte_idx_b;
    if(tag->byte_idx_b > -1){
            //conditionnal declaration (avoid declaring for nofing)
            const int8_t byte_indx_b = tag->byte_idx_b;
    }
    if (tag->n_bits == 16)    // cas de 16 bits exactement
    {
        uint8_t ba = TagData[byta];
        uint8_t bb = TagData[bytb];
        *out_value = ba | (bb << 8);   // the B should be the one wo can be less tan 8 bit.
    }
    else if((n_bits < 16) && (n_bits > 8)){// cas d'entre 8 et 16 bits.
        uint8_t ba = TagData[byta];
        uint8_t bb;
        // création d'un masque
        uint8_t mask = prep_mask(bpb, n_bits - 8);
        uint8_t data_temp = TagData[bytb] & mask;
        bb = data_temp >> (8 - bpa - n_bits);
        *out_value = ba | (bb << 8);   // the B should be the one wo can be less tan 8 bit.

    }
    else
    {
        // création du mask
        uint8_t mask = prep_mask(bpa, n_bits);

        //uint8_t mask = ((1 << n_bits) - 1) << shift;

        uint8_t data_temp = TagData[byta] & mask;
        int decalage = 8 - bpa - n_bits;
        *out_value = data_temp >> decalage ;
    }
    return CAN_TG_SUCCESS;
}


void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer)
{
    for (int i = 0; i < N_BYTES; i++)
    {
        rx_buffer[i] = tx_buffer[i];
    }
}



CAN_TG_STATUE manual_send_data(uint8_t *TxDataA){
    for (int i = 0; i < N_BYTES; i++)
    {
        TxDataA[i] = TagData[i];
    }
}
uint8_t* simple_get_data_to_send(void){
    return &TagData;
}

