#ifndef STRUCT_H
#define STRUCT_H

//DEFININDO AS STRUCTs
typedef struct cartas //STRUCT DE UMA CARTA
{
    char valor;
    char naipe;
} t_carta; //TIPO CARTA

typedef struct jogadores //STRUCT DE UMA "MAO" DE UM JOGADOR
{
    t_carta carta[106]; //CADA JOGADOR PODE TER NO MAX 106 CARTAS
    unsigned int primeira_jogada:1; //MARCA SE A PRIMEIRA JOGADA JA FOI FEITA (0 == NAO, 1 == SIM)
} t_mao;

typedef struct mesa //STRUCT DE UM MONTE NA MESA
{
    t_carta carta[13]; //CADA MONTE NA MESA TEM NO MAXIMO 13 CARTAS
} t_mesa;
#endif

