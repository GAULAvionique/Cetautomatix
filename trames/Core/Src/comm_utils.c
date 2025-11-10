#include "comm_utils.h"

/* =========================================================
 * CRC-16/CCITT-FALSE
 *  - Poly  : 0x1021
 *  - Init  : 0xFFFF
 *  - RefIn : false
 *  - RefOut: false
 *  - XorOut: 0x0000
 * ========================================================= */
uint16_t comm_crc16_ccitt(const void* data, uint16_t len) {
    const uint8_t* p = (const uint8_t*)data;
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < len; i++) {
        crc ^= ((uint16_t)p[i] << 8);
        for (uint8_t b = 0; b < 8; b++) {
            if (crc & 0x8000) {
                crc = (uint16_t)((crc << 1) ^ 0x1021);
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

/* =========================================================
 * COBS (Consistent Overhead Byte Stuffing) - Encode
 *
 * Transforme un flux binaire arbitraire en un flux SANS octet 0x00.
 * On peut ainsi utiliser 0x00 comme délimiteur de fin de trame sur UART.
 *
 * Entrée :
 *   - in[0..len-1]
 * Sortie :
 *   - out : buffer de taille >= len + len/254 + 1
 * Retour :
 *   - nombre d'octets écrits dans 'out' (sans le délimiteur 0x00 final)
 *
 * ========================================================= */
uint16_t comm_cobs_encode(const uint8_t* in, uint16_t len, uint8_t* out) {
    const uint8_t*  src = in;
    const uint8_t*  end = in + len;
    uint8_t*        code_ptr = out;      /* Adresse du code en cours */
    uint8_t*        dst = out + 1;       /* Là où on écrit les données */
    uint8_t         code = 1;            /* Valeur du code courant (>=1, <=255) */

    while (src < end) {
        if (*src == 0x00) {
            /* On termine le bloc courant */
            *code_ptr = code;
            code_ptr  = dst++;           /* Nouveau code à écrire plus tard */
            code      = 1;
            src++;                       /* On saute le zero */
        } else {
            *dst++ = *src++;
            code++;
            if (code == 0xFF) {
                /* Bloc plein (254 octets non-nuls) : on flush */
                *code_ptr = code;
                code_ptr  = dst++;
                code      = 1;
            }
        }
    }

    /* Flush final */
    *code_ptr = code;

    return (uint16_t)(dst - out);
}
