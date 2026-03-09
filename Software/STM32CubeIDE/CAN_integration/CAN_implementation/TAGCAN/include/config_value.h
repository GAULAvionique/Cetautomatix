#ifndef CONFIG_VALUE_H
#define CONFIG_VALUE_H

#define MAX_TAGS 32
#define MAX_TAG_NAME_LEN 16
#define N_BYTES 8
#define MAX_N_GT 3

extern const int CAN_TG_ERROR_INTERNAL_CORUPTION;
extern const int CAN_TG_ERROR_TAG_NAME_TO_BIGS;
extern const int CAN_TG_ERROR_TAG_NAME_INVALIDE;
extern const int CAN_TG_ERROR_TAG_NOT_FOUND;
extern const int CAN_TG_ERROR_NOT_ENOUNG_SPACE;
extern const int CAN_TG_ERROR_NULL_POINTOR;
extern const int CAN_TG_ERROR_TRY_TO_FIND_END_CARACTER;
extern const int CAN_TG_ERROR_SIZE_VALUE_INVALIDE;
extern const int CAN_TG_ERROR_SUBDIVISION_FAIL;
extern const int CAN_TG_ERROR_TOO_MAY_TAGS;
extern const int CAN_TG_ERROR_NAME_ALREADY_USE;
extern const int SUCCES_TO_SET;
extern const int SUCCES;
// יטאג
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

typedef enum {
    RECEVE_ONLY,    /**< In this mode, the 'packet' cannot  be wrinten to from 'set_value', but the value (assuming it's the correct id) while be updated if the id match. */
    SEND_ONLY,    /**< In this mode, the 'packet' linked with can only send data. In other words, if an other CAN perif send a 'paquet' with the same ID, it's while not modifie he value. */
    SEND_AND_RECEVE,    /**< In this mode, you can modifie the value, send the value and receve an update form the CAN line. (In other word, it's the less free of contrainte, but also the most suceptible to have unwished change of value.) */
} CAN_MODE;

#endif // CONFIG_VALUE_H
