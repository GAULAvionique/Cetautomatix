#ifndef CONFIG_VALUE_H
#define CONFIG_VALUE_H

/** Nombre maximal de tags geres simultanement. */
#define MAX_TAGS 32
/** Longueur maximale d'un nom de tag (incluant le '\0'). */
#define MAX_TAG_NAME_LEN 16
/** Taille du payload CAN en octets. */
#define N_BYTES 8
/** Nombre maximal de gestionnaires (reserve/extension). */
#define MAX_N_GT 3

/** Code d'erreur: corruption interne detectee. */
extern const int CAN_TG_ERROR_INTERNAL_CORUPTION;
/** Code d'erreur: nom de tag trop long. */
extern const int CAN_TG_ERROR_TAG_NAME_TO_BIGS;
/** Code d'erreur: nom de tag invalide. */
extern const int CAN_TG_ERROR_TAG_NAME_INVALIDE;
/** Code d'erreur: tag introuvable. */
extern const int CAN_TG_ERROR_TAG_NOT_FOUND;
/** Code d'erreur: espace memoire insuffisant. */
extern const int CAN_TG_ERROR_NOT_ENOUNG_SPACE;
/** Code d'erreur: pointeur NULL recu. */
extern const int CAN_TG_ERROR_NULL_POINTOR;
/** Code d'erreur: fin de chaine non trouvee. */
extern const int CAN_TG_ERROR_TRY_TO_FIND_END_CARACTER;
/** Code d'erreur: taille de valeur invalide. */
extern const int CAN_TG_ERROR_SIZE_VALUE_INVALIDE;
/** Code d'erreur: echec de subdivision/allocation. */
extern const int CAN_TG_ERROR_SUBDIVISION_FAIL;
/** Code d'erreur: trop de tags declares. */
extern const int CAN_TG_ERROR_TOO_MAY_TAGS;
/** Code d'erreur: nom de tag deja utilise. */
extern const int CAN_TG_ERROR_NAME_ALREADY_USE;
/** Code de succes historique pour set_tag/set_value. */
extern const int SUCCES_TO_SET;
/** Code de succes generique historique. */
extern const int SUCCES;
// Codes de retour utilises par l'API TAGCAN
/** \brief Codes de retour du module TAGCAN. */
typedef enum {
    CAN_TG_ERROR_NOT_ENOUNG_SPACE_ = -2,
    CAN_TG_SUCCESS = 1,
    CAN_TG_ERROR_TAG_NOT_FOUND_ = -3,
    CAN_TG_ERROR_TAG_NAME_TO_BIGS_ = -4,
    CAN_TG_ERROR_TOO_MAY_TAGS_ = -5,
    CAN_TG_ERROR_SIZE_VALUE_INVALIDE_ = -6,
    CAN_TG_ERROR_NAME_ALREADY_USE_ = -7,
    CAN_TG_ERROR_SUBDIVISION_FAIL_ = -8,
    CAN_TG_ERROR_TAG_NAME_INVALIDE_=-9,
    CAN_TG_ERROR_NULL_POINTOR_ = -2,
    CAN_TG_ERROR_TRY_TO_FIND_END_CARACTER_ = -21,
    CAN_TG_ERROR_INTERNAL_CORUPTION_ = -30,
    CAN_TG_ERROR_VALUE_TO_BIG = -10,
} CAN_TG_STATUE;

/** \brief Modes d'utilisation CAN (envoi/reception). */
typedef enum {
    RECEVE_ONLY,    /**< Reception seule: les updates entrantes peuvent modifier la valeur; l'ecriture locale est bloquee. */
    SEND_ONLY,    /**< Emission seule: la valeur locale est emise, les updates entrantes ne la modifient pas. */
    SEND_AND_RECEVE,    /**< Emission + reception: ecriture locale et updates entrantes toutes deux autorisees. */
} CAN_MODE;

#endif // CONFIG_VALUE_H
