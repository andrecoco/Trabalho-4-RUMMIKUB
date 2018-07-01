#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>
#include "funcoes.h"
#include "struct.h"
#define AZUL 9
#define VERDE 10
#define VERMELHO 12
#define ROSA 13
#define AMARELO 14

int main()
{
    //SETANDO COR DO CONSOLE (BRANCO CLARO)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);

    //BOAS VINDAS
    printf("### BEM VINDO AO RUMMIKUB! ###\n");
    Sleep(1200);
    printf("LEIA O README");
    tres_pontinhos();
    system("cls");

    //CRIANDO/INICIALIZANDO O MONTE DE CARTAS
    t_carta monte_baralho[106]; //CRIA O MONTE
    embaralhar_monte(monte_baralho); //CRIA O MONTE EMBARALHADO (DE ARQUIVO OU RANDOM)

    //PREPARACAO PARA INICIO DO JOGO
    int numero_jogadores;
    while(true)
    {
        printf("QUANTOS JOGADORES? (1-5) : ");
        fflush(stdin);
        scanf("%d", &numero_jogadores);
        if(numero_jogadores < 1 || numero_jogadores > 5)
        {
            printf("ENTRADA INVALIDA!\n");
        }
        else
        {
            break;
        }
    }

    //CRIANDO AS "MAOS" DOS JOGADORES
    t_mao jogadores[numero_jogadores]; //CRIA UMA MAO PARA CADA JOGADOR
    inicializar_maos(monte_baralho, jogadores, numero_jogadores); //DISTRIBUI 14 CARTAS A CADA JOGADOR

    //CRIA TODOS OS POSSIVEIS MONTES NA MESA
    t_mesa mesa[35]; //35 MONTES SAO O LIMITE
    inicializar_mesa(mesa);

    //INICIO DO JOGO
    int jogador_atual = 0;
    int pontos_jogadores[numero_jogadores]; //USADA PARA SALVAR O NUMERO DE PONTOS DE CADA JOGADOR
    while(true) //LOOP DO JOGO
    {
        //TROCA PARA O PROXIMO JOGADOR
        if(jogador_atual == numero_jogadores)
        {
            jogador_atual = 1;
        }
        else
        {
            jogador_atual++;
        }
        int flag;
        int jogador_ganhador;
        flag = jogada(monte_baralho, jogadores, mesa, numero_jogadores, jogador_atual); //REALIZA A JOGADA
        flag = testa_vitoria(jogadores, jogador_atual); //VERIFICA SE O JOGADOR NAO ESTA COM NENHUMA CARTA
        if(flag) //CASO MONTE VAZIO OU ALGUEM ESVAZIOU A MAO
        {
          int pontos_obtidos = 0;
          contagem_pontos(jogadores, numero_jogadores, pontos_jogadores, &jogador_ganhador, &pontos_obtidos);
          printf("PARABENS JOGADOR %d !!\nOBTEVE %d PONTOS\n",jogador_ganhador, pontos_obtidos); //ANUNCIA GANHADOR (AQUELE COM MENOS PONTOS NA MAO (0 CASO TENHA ESVAZIADO A MAO))
          system("PAUSE");
          break;
        }
    }
    return 0;
}
