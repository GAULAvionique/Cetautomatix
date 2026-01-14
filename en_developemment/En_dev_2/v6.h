#ifndef V6_H_INCLUDED
#define V6_H_INCLUDED

typedef struct {
    int8_t  data[8];
    char  *tag[8];
    int8_t tempo;  // temporaire pour faire les divers opetation
    int8_t data_tempo[8];
    int8_t data_receve[8];
} GestionCAN;


void add_tag(char nom_tag, int n_bit){
    char nom_tag_2;
    // adaptation du nom
    if(n_bit == 1){nom_tag_2 = "1_" + nom_tag + ",";}
    if(n_bit == 3){nom_tag_2 = "3_" + nom_tag + ",";}
    if(n_bit == 4){nom_tag_2 = "4_" + nom_tag + ",";}
    if(n_bit == 8){nom_tag_2 = "8_" + nom_tag + ",";}
    if(n_bit == 16){
        char nom_tag_3;
        nom_tag_2 = "16a_" + nom_tag + ",";
        nom_tag_3 = "16b_" + nom_tag + ",";
    }
}


#endif // V6_H_INCLUDED
