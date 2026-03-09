
#include "../../include/sub_include/tag_manager.h"
#ifndef CONFIG_VALUE_H
#include "../../include/config_value.h"
#endif // CONFIG_VALUE_H
#ifndef GESTION_TAG_H
#include "../../include/gestion_tag.h"
#endif // GESTION_TAG_H
#ifndef PRINT_VAL_H
#include "../../include/sub_include/print_val.h"
#endif // PRINT_VAL_H
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Etat global du gestionnaire de tags
TagDef g_tags[MAX_TAGS];
uint8_t g_tag_count = 0;
uint8_t g_bytes_used[N_BYTES] = {0};

void init_tag_manager(void)
{
    g_tag_count = 0;
    // Remise a zero de l'occupation des bytes.
    memset(g_bytes_used, 0, sizeof(g_bytes_used));
    // Reinitialisation complete de tous les slots de tags.
    for (int i = 0; i < MAX_TAGS; i++)
    {
        g_tags[i].name[0] = '\0';   // Slot considere vide quand name[0] == '\0'.
        g_tags[i].n_bits = 0;
        g_tags[i].byte_idx_a = -1;
        g_tags[i].byte_idx_b = -1;
        g_tags[i].bit_pos_a = 0;
        g_tags[i].bit_pos_b = 0;
    }
}

/** \brief [INTERNE] Retourne l'index du premier slot tag libre.
 * \details Un slot est considere libre si son nom est NULL ou vide ('\\0').
 * \return Index du slot libre, ou CAN_TG_ERROR_TOO_MAY_TAGS si aucun slot disponible.
 */
int get_first_tags_vide(void)
{
    for (int i = 0; i < MAX_TAGS; i++)
    {
        // Cas defensif: un nom NULL est traite comme slot libre.
        if(g_tags[i].name == NULL)
        {
            return i;
        }
        // Premier slot libre: nom vide.
        if(g_tags[i].name[0] == '\0')
        {
            return i;
        }
    }
    // Cas de garde: aucun slot libre detecte.
    return CAN_TG_ERROR_TOO_MAY_TAGS;
}

/** \brief [INTERNE] Enregistre un nom de tag dans un slot.
 * \warning Les verifications d'entree sont censees etre faites en amont.
 * \param n_tag Index du slot (0..MAX_TAGS-1).
 * \param tag_name Nom logique du tag.
 *
 */

void set_name_tag(const int n_tag, const char* tag_name)
{
    // Nettoie le buffer avant copie du nom.
    for (int i = 0; i < MAX_TAG_NAME_LEN - 2; i++)
    {
        g_tags[n_tag].name[i]=' ';
    }
    // Copie du nom dans la structure.
    strncpy(g_tags[n_tag].name, tag_name, MAX_TAG_NAME_LEN - 1);
    // Termine explicitement la chaine.
    g_tags[n_tag].name[MAX_TAG_NAME_LEN - 1] = '\0';
    // Debug optionnel:
    //printf("le nom du tag est: [%s]\n", g_tags[n_tag].name);
}

// Initialise les champs communs d'un tag et met a jour l'occupation memoire.
/** \brief [INTERNE] Initialise les champs communs d'un tag.
 * \param n_tag Index du slot tag cible.
 * \param tag_name Nom logique a enregistrer.
 * \param n_bits Taille de la valeur en bits.
 * \param byte_idx_a Byte principal de stockage (partie A).
 */
void general_set_tag(const int n_tag,const char* tag_name,
                     uint8_t n_bits, int8_t byte_idx_a)
{
    set_name_tag(n_tag, tag_name);
    g_tags[n_tag].n_bits = n_bits;

    g_tags[n_tag].byte_idx_a = byte_idx_a;
    // Le tag est desormais actif.
    g_tag_count++;

    // Mise a jour de l'occupation des bits dans le byte principal.
    if(n_bits >= 8)
    {
        g_bytes_used[byte_idx_a] = 8;
    }
    else
    {
        g_bytes_used[byte_idx_a] += n_bits;
    }
}

// [INTERNE] Creation d'un tag sur 9..16 bits (2 bytes potentiels).
// n_bits_total: taille totale de la valeur (8..16).
/** \brief [INTERNE] Cree un tag avec repartition potentielle sur 2 bytes.
 * \param tag_name Nom logique du tag.
 * \param byte_idx_a Byte de la partie A.
 * \param byte_idx_b Byte de la partie B.
 * \param n_bits_total Taille totale en bits.
 */
void set_new_tag_9_to_16(const char* tag_name, const int8_t byte_idx_a, const int8_t byte_idx_b, uint8_t n_bits_total)
{
    // Recherche d'un slot libre.
    int n_tag = get_first_tags_vide();
    general_set_tag(n_tag, tag_name, n_bits_total, byte_idx_a);    // Initialisation commune.
    // Pour > 8 bits, une partie B est utilisee.
    if(n_bits_total > 8)
    {

        g_tags[n_tag].byte_idx_b = byte_idx_b;
        g_tags[n_tag].bit_pos_b = g_bytes_used[byte_idx_b];
        g_bytes_used[byte_idx_b] += n_bits_total - 8;
        g_tags[n_tag].bit_pos_a = 0;
        /* A ce stade, la repartition est validee: on peut finaliser
           l'occupation et les positions des bits pour A et B. */
    }
}

/** \brief Trouve le premier byte ayant assez de place libre.
 *
 * \param siz Taille demandee (en bits).
 * \param retur_var Sortie: index du byte selectionne.
 * \return CAN_TG_SUCCESS ou CAN_TG_ERROR_NOT_ENOUNG_SPACE_.
 *
 */
CAN_TG_STATUE find_byte_with_enoung_space(uint8_t siz, int8_t* retur_var)
{
    // Pour > 8 bits, la 1re recherche se limite a un bloc de 8 bits.
    if(siz > 8)
    {
        siz = 8;
    }
    // Pour les tailles impaires: priorite aux bytes a place impaire restante.
    if(siz%2 > 0)
    {
        // Heuristique de placement.
        for (int8_t i = 0; i < N_BYTES; i++)
        {
            {
                if(((8 - g_bytes_used[i])%2 > 0) && ((8 - g_bytes_used[i]) >= siz))
                {
                    *retur_var=i;
                    return CAN_TG_SUCCESS;
                }
            }
        }
    }
    // Sinon, recherche sequentielle standard.
    for (int8_t i = 0; i < N_BYTES; i++)
    {
        if((8 - g_bytes_used[i]) >= siz)
        {
            *retur_var=i;
            return CAN_TG_SUCCESS;
        }
    }
    return CAN_TG_ERROR_NOT_ENOUNG_SPACE_;
}


/** \brief Variante de recherche en excluant le byte deja utilise par la partie A.
 *
 * \param siz_total Taille totale (9..16).
 * \param posi_premier_bit Byte deja retenu pour la partie A.
 * \param retur_var Sortie: index du byte pour la partie B.
 * \return CAN_TG_SUCCESS ou CAN_TG_ERROR_NOT_ENOUNG_SPACE_.
 *
 */

CAN_TG_STATUE find_byte2_with_enoung_space(uint8_t siz_total, int8_t posi_premier_bit,int8_t* retur_var)
{
    uint8_t siz = siz_total - 8;
    // Meme heuristique que find_byte_with_enoung_space.
    if(siz%2 > 0)
    {
        // Priorite aux bytes dont l'espace restant est impair.
        for (int8_t i = 0; i < N_BYTES; i++)
        {
            if(posi_premier_bit == i)
            {
                continue;
            }
            if(((8 - g_bytes_used[i])%2 > 0) && ((8 - g_bytes_used[i]) >= siz))
            {
                *retur_var=i;
                return CAN_TG_SUCCESS;
            }
        }
    }
    // Sinon, recherche sequentielle standard.
    for (uint8_t i = 0; i < N_BYTES; i++)
    {
        if(posi_premier_bit == i)
        {
            continue;
        }
        if((8 - g_bytes_used[i]) >= siz)
        {
            *retur_var=i;
            return CAN_TG_SUCCESS;
        }
    }
    return CAN_TG_ERROR_NOT_ENOUNG_SPACE_;
}



// API publique: declaration de tag.
/** \brief Declare un nouveau tag et reserve son espace dans TxData.
 * \param tag_name Nom logique du tag.
 * \param siz Taille de la valeur en bits (implementation actuelle: 1..16).
 * \return Code de succes ou code d'erreur CAN_TG_STATUE.
 */
CAN_TG_STATUE set_tag(const char* tag_name, uint8_t siz){
    CAN_TG_STATUE statue_actuel;
    // Limite du nombre de tags.
    if(g_tag_count >= MAX_TAGS){
        return CAN_TG_ERROR_TOO_MAY_TAGS;
    }
    // Validation longueur nom (implementation actuelle basee sur sizeof(pointer)).
    if(sizeof(tag_name) >= MAX_TAG_NAME_LEN){
        return CAN_TG_ERROR_TAG_NAME_TO_BIGS;
    }
    // Interdit les doublons de nom.
    if(get_tag_def(tag_name) != NULL){
        return CAN_TG_ERROR_NAME_ALREADY_USE;
    }
    // Nom vide invalide.
    if(tag_name[0]=='\0'){
        return CAN_TG_ERROR_TAG_NAME_INVALIDE;
    }
    // Tailles acceptees: 1..16.
    if(siz <= 0 || siz > 16){return CAN_TG_ERROR_SIZE_VALUE_INVALIDE;}

    //------------------------- algo de placement memoire.

    if(siz>8){
        // Garde defensive (deja couverte plus haut).
        if(siz>16){
            return CAN_TG_ERROR_SIZE_VALUE_INVALIDE_;
        }
        else{
            uint8_t bytea, byteb;    // Index des bytes reserves.
            statue_actuel = find_byte_with_enoung_space(siz, &bytea);
            if(statue_actuel != CAN_TG_SUCCESS){return statue_actuel;}// propagation du statut
            statue_actuel = find_byte2_with_enoung_space(siz, bytea, &byteb);
            if(statue_actuel != CAN_TG_SUCCESS){
                return statue_actuel;   // propagation du statut
            }
            set_new_tag_9_to_16(tag_name, bytea, byteb, siz);
            return CAN_TG_SUCCESS;
        }
    }
    else{
        uint8_t bytea;
        statue_actuel = find_byte_with_enoung_space(siz, &bytea);
        if(statue_actuel != CAN_TG_SUCCESS){return statue_actuel;}// propagation du statut
        int n_tag = get_first_tags_vide();
        general_set_tag(n_tag, tag_name, siz, bytea);
        return CAN_TG_SUCCESS;
    }


};

/** \brief Recherche un tag actif par son nom.
 * \param tag_name Nom du tag recherche.
 * \return Pointeur sur TagDef si trouve, sinon NULL.
 */
const TagDef* get_tag_def(const char* tag_name)
{
    for (int i = 0; i < g_tag_count; i++)
    {
        if (strncmp(g_tags[i].name, tag_name, MAX_TAG_NAME_LEN) == 0)
        {
            return &g_tags[i];
        }
    }
    return NULL; // Tag non trouve
};

