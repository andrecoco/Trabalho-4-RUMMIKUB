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

//FUNCOES
void tres_pontinhos()
{
    Sleep(300);
    printf(".");
    Sleep(300);
    printf(".");
    Sleep(300);
    printf(".");
    Sleep(300);
}

void char_colorido(char texto, int cor) //IMPRIME UM TEXTO COLORIDO
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, cor);
    printf("%c", texto);
    SetConsoleTextAttribute(hConsole, 15); //VOLTA PRO PADRAO
}

void inicializar_monte(t_carta monte_baralho[]) //INICIALIZA O MONTE COM AS CARTAS ORDENADS (LE DO ARQUIVO PADRÃO)
{
    int i = 0; //CONTADOR
    FILE *baralho_ordenado;
    baralho_ordenado = fopen("baralho_ordenado.txt", "r");
    if(baralho_ordenado == NULL)
    {
        printf("ERRO AO LER ARQUIVO PADRAO DO BARALHO!\n");
        system("PAUSE");
        return;
    }
    while(!(feof(baralho_ordenado)) && i < 106)
    {
        char valor_lido;
        char naipe_lido;
        valor_lido = getc(baralho_ordenado);
        while(valor_lido == '\n')
        {
            valor_lido = getc(baralho_ordenado);
        }
        naipe_lido = getc(baralho_ordenado);
        while(naipe_lido == '\n')
        {
            naipe_lido = getc(baralho_ordenado);
        }
        //barra_n = getc(baralho_ordenado); //LE OS \n
        monte_baralho[i].valor = valor_lido;
        monte_baralho[i].naipe = naipe_lido;
        i++;
    }
    fclose(baralho_ordenado);
}

void inicializar_cartas(t_mao jogadores[], int numero_jogadores) //INICIALZIA A MAO DOS JOGADORES COM "NADA"
{
    int i;
    int j;
    for(i = 0; i<numero_jogadores; i++)
    {
        jogadores[i].primeira_jogada = 0; //0 == NAO FEZ A PRIMEIRA JOGADA AINDA
        for(j = 0; j<106; j++)
        {
            jogadores[i].carta[j].valor = '0';
            jogadores[i].carta[j].naipe = '0';
        }
    }
}

void inicializar_maos(t_carta monte_baralho[], t_mao jogadores[], int numero_jogadores) //DISTRIBUI AS CARTAS
{
    inicializar_cartas(jogadores, numero_jogadores);
    int i,j; //CONTADORES
    int k = 0; //INDICE DO MONTE_BARALHO
    //DISTRIBUI 14 PECAS PARA CADA JOGADOR
    for(i = 0; i < 14; i++)
    {
        for(j = 0; j<numero_jogadores; j++)
        {
            jogadores[j].carta[i].valor = monte_baralho[k].valor;
            jogadores[j].carta[i].naipe = monte_baralho[k].naipe;
            monte_baralho[k].valor = '0'; //MARCA AS CARTAS DO MONTE COMO "ZERO"
            monte_baralho[k].naipe = '0'; //POIS JA FORAM RETIRADAS DO MONTE
            k++;
        }
    }
}

void inicializar_mesa(t_mesa mesa[])
{
    int i;
    int j;
    for(i = 0; i < 35; i++)
    {
        for(j = 0; j<13; j++)
        {
            mesa[i].carta[j].valor = '0';
            mesa[i].carta[j].naipe = '0';
        }
    }
}

int embaralhar_monte(t_carta monte_baralho[])  //CRIA UM MONTE EM ORDEM ALEATORIA OU LE DE UM ARQUIVO A ORDEM
{
    char entrada;
    int modo = 0; //GUARDA SE O USUARIO ESCOLHEU ALEATORIA (1) OU ARQUIVO (2)
    while(true)
    {
        printf("DESEJA LER A ORDEM DAS CARTAS DO ARQUIVO? (S/N): ");
        fflush(stdin);
        entrada = getchar();
        if(entrada == 's' || entrada == 'S')
        {
            modo = 2; //LER DE ARQUIVO
            break;
        }
        else if(entrada == 'n' || entrada == 'N')
        {
            modo = 1; //NAO LER DE ARQUIVO (ALEATORIO)
            break;
        }
        else
        {
            printf("ENTRADA INVALIDA\n");
        }
    } //SAI DAQUI COM MODO == 1 (ALEATORIO) ou MODO == 2 (LER DE ARQUIVO)

    if(modo == 1) //MONTE ALEATORIO
    {
        inicializar_monte(monte_baralho); //INICIALIZA TODAS AS CARTAS (EM ORDEM)
        //INICIALIZANDO O RAND
        time_t t;
        srand((unsigned)time(&t)); //INICIALIZA RAND

        //EMBARALHA O BARALHO
        t_carta j;
        int i,k;
        for(i=0; i < 106; i++)
        {
            j = monte_baralho[i];
            k = rand() % 106;
            monte_baralho[i].naipe = monte_baralho[k].naipe;
            monte_baralho[i].valor = monte_baralho[k].valor;
            monte_baralho[k].naipe = j.naipe;
            monte_baralho[k].valor = j.valor;
        }
    }
    if(modo == 2) //LER BARALHO DE ARQUIVO
    {
        int i = 0;
        FILE *baralho;
        baralho = fopen("baralho.txt","r");
        if(baralho == NULL)
        {
            printf("ERRO AO ENCONTRAR BARALHO!\n");
            system("PAUSE");
            return 1;
        }
        while(!(feof(baralho)) && i < 106)
        {
            char valor_lido;
            char naipe_lido;
            valor_lido = getc(baralho);
            while(valor_lido == '\n')
            {
                valor_lido = getc(baralho);
            }
            naipe_lido = getc(baralho);
            while(naipe_lido == '\n')
            {
                naipe_lido = getc(baralho);
            }
            monte_baralho[i].valor = valor_lido;
            monte_baralho[i].naipe = naipe_lido;
            i++;
        }
        fclose(baralho);
    }
    return 0;
}

void imprimir_tela(t_mao jogadores[], int numero_jogadores, t_mesa mesa[], int jogador_atual)
{
    system("cls");
    int flag = 1; //FLAG PARA MARCAR SE ALGUM MONTE FOI IMPRIMIDO OU NAO
    int i; //CONTADOR PRO "FOR"
    int j; //CONTADOR PARA O NUMERO DA CARTA
    printf("### RUMMIKUB ###\n");
    printf("### MESA ###\n\n");
    for(i=0; i<35; i++)
    {
        if(mesa[i].carta[0].valor == '0') //CASO MONTE NAO TENHA CARTA
        {
            continue;
        }
        printf("MONTE %d\n", i);
        flag = 0; //MARCA QUE UM MONTE FOI IMPRIMIDO
        for(j=0; j<13; j++)
        {
            switch (mesa[i].carta[j].naipe)
            {
            case '$':
                char_colorido(mesa[i].carta[j].valor,VERDE);
                char_colorido(mesa[i].carta[j].naipe,VERDE);
                printf(" | ");
                break;
            case '@':
                char_colorido(mesa[i].carta[j].valor,AMARELO);
                char_colorido(mesa[i].carta[j].naipe,AMARELO);
                printf(" | ");
                break;
            case '!':
                char_colorido(mesa[i].carta[j].valor,ROSA);
                char_colorido(mesa[i].carta[j].naipe,ROSA);
                printf(" | ");
                break;
            case '#':
                char_colorido(mesa[i].carta[j].valor,AZUL);
                char_colorido(mesa[i].carta[j].naipe,AZUL);
                printf(" | ");
                break;
            case '*':
                char_colorido(mesa[i].carta[j].valor,VERMELHO);
                char_colorido(mesa[i].carta[j].naipe,VERMELHO);
                printf(" | ");
                break;
            }
        }
        printf("\n");
    }
    if(flag)
    {
        printf("\n|| MESA VAZIA ||\n\n");
    }
    printf("\n### MAOS ###\n");
    for(i=0; i<numero_jogadores; i++)
    {
        j = 0;
        printf("____________\n");
        printf(" JOGADOR: %d\n ", i+1);
        while(true)
        {
            if(jogadores[i].carta[j].valor == '0')
            {
                break;
            }
            switch (jogadores[i].carta[j].naipe)
            {
            case '$':
                char_colorido(jogadores[i].carta[j].valor,VERDE);
                char_colorido(jogadores[i].carta[j].naipe,VERDE);
                printf("(%d)| ", j);
                break;
            case '@':
                char_colorido(jogadores[i].carta[j].valor,AMARELO);
                char_colorido(jogadores[i].carta[j].naipe,AMARELO);
                printf("(%d)| ", j);
                break;
            case '!':
                char_colorido(jogadores[i].carta[j].valor,ROSA);
                char_colorido(jogadores[i].carta[j].naipe,ROSA);
                printf("(%d)| ", j);
                break;
            case '#':
                char_colorido(jogadores[i].carta[j].valor,AZUL);
                char_colorido(jogadores[i].carta[j].naipe,AZUL);
                printf("(%d)| ", j);
                break;
            case '*':
                char_colorido(jogadores[i].carta[j].valor,VERMELHO);
                char_colorido(jogadores[i].carta[j].naipe,VERMELHO);
                printf("(%d)| ", j);
                break;
            }
            //printf("%c%c ", jogadores[i].carta[j].valor, jogadores[i].carta[j].naipe);
            j++;
        }
        printf("\n");
    }
    printf("\n");
    printf("JOGADOR %d, FACA SUA JOGADA:\n", jogador_atual);
}

void backup_jogo(t_carta monte_baralho[], t_carta monte_baralho_back[], t_mao jogadores[], t_mao jogadores_back[], t_mesa mesa[], t_mesa mesa_back[], int numero_jogadores) //BACKUP DO ESTADO ATUAL DO JOGO
{
    int i,j;
    for(i = 0; i<106; i++) //BACKUP MONTE
    {
        monte_baralho_back[i].valor = monte_baralho[i].valor;
        monte_baralho_back[i].naipe = monte_baralho[i].naipe;
    }
    for(i = 0; i<numero_jogadores; i++) //BACKUP JOGADORES
    {
        for(j = 0; j<106; j++)
        {
            jogadores_back[i].carta[j].valor = jogadores[i].carta[j].valor;
            jogadores_back[i].carta[j].naipe = jogadores[i].carta[j].naipe;
        }
        jogadores_back[i].primeira_jogada = jogadores[i].primeira_jogada;
    }
    for(i = 0; i<35; i++) //BACKUP DA MESA
    {
        for(j = 0; j<13; j++)
        {
            mesa_back[i].carta[j].valor = mesa[i].carta[j].valor;
            mesa_back[i].carta[j].naipe = mesa[i].carta[j].naipe;
        }
    }
}

void resetar_jogo(t_carta monte_baralho[], t_carta monte_baralho_back[], t_mao jogadores[], t_mao jogadores_back[], t_mesa mesa[], t_mesa mesa_back[], int numero_jogadores) //RESETA O JOGO ATUAL USANDO O BACKUP
{
    int i,j;
    for(i = 0; i<106; i++) //BACKUP MONTE
    {
        monte_baralho[i].valor = monte_baralho_back[i].valor;
        monte_baralho[i].naipe = monte_baralho_back[i].naipe;
    }
    for(i = 0; i<numero_jogadores; i++) //BACKUP JOGADORES
    {
        for(j = 0; j<106; j++)
        {
            jogadores[i].carta[j].valor = jogadores_back[i].carta[j].valor;
            jogadores[i].carta[j].naipe = jogadores_back[i].carta[j].naipe;
        }
        jogadores[i].primeira_jogada = jogadores_back[i].primeira_jogada;
    }
    for(i = 0; i<35; i++) //BACKUP DA MESA
    {
        for(j = 0; j<13; j++)
        {
            mesa[i].carta[j].valor = mesa_back[i].carta[j].valor;
            mesa[i].carta[j].naipe = mesa_back[i].carta[j].naipe;
        }
    }
}

int descer_peca(t_mesa mesa[],t_mao jogadores[],int jogador_atual,int posicao_peca, int monte) //ADICIONA A PECA DO JOGADOR A ALGUM MONTE
{
    int i;
    jogador_atual --; //PARA ACERTAR A CONTAGEM DOS INDICES (QUE COMECA EM 0)
    //printf("ESTADO:\nPOS PEC: %d\nPECA DESCIDA: %c %c\nPOS MONT: %d\n",posicao_peca, jogadores[jogador_atual].carta[posicao_peca].valor, jogadores[jogador_atual].carta[posicao_peca].naipe, monte);
    //system("PAUSE");
    if(jogadores[jogador_atual].carta[posicao_peca].valor == '0')
    {
        return 1; //PECA NAO EXISTE
    }
    else if(mesa[monte].carta[0].valor == '0')  //CASO O MONTE NAO EXISTA
    {
        if(monte == 0 || mesa[monte-1].carta[0].valor != '0') //SE O MONTE ANTERIOR EXISTIR (OU NAO EXISTIR NENHUM), CRIE O MONTE
        {
            mesa[monte].carta[0].valor = jogadores[jogador_atual].carta[posicao_peca].valor;
            mesa[monte].carta[0].naipe = jogadores[jogador_atual].carta[posicao_peca].naipe;
            jogadores[jogador_atual].carta[posicao_peca].valor = '0';
            jogadores[jogador_atual].carta[posicao_peca].naipe = '0';
            //DESLOCAR TODAS AS PECAS RESTANTES
            for(i = posicao_peca; i<105; i++)
            {
                jogadores[jogador_atual].carta[i] = jogadores[jogador_atual].carta[i+1];
            }
        }
        else
        {
            return 2; //MONTE INVALIDO
        }
    }
    else
    {
        int j = 0;
        while(mesa[monte].carta[j].valor != '0' && j<13) //DESCOBRE QUAL A POSICAO QUE NAO POSSUI CARTA AINDA
        {
            j++;
        }
        if(j == 13)
        {
            return 3; //MONTE CHEIO!
        }
        mesa[monte].carta[j].valor = jogadores[jogador_atual].carta[posicao_peca].valor;
        mesa[monte].carta[j].naipe = jogadores[jogador_atual].carta[posicao_peca].naipe;
        jogadores[jogador_atual].carta[posicao_peca].valor = '0';
        jogadores[jogador_atual].carta[posicao_peca].naipe = '0';
        //DESLOCAR TODAS AS PECAS RESTANTES
        for(i = posicao_peca; i<105; i++)
        {
            jogadores[jogador_atual].carta[i] = jogadores[jogador_atual].carta[i+1];
        }
        //printf("SEGUNDA SUBS OCORREU\n");
    }
    return 0;
}

int pegar_peca(t_mesa mesa[],t_mao jogadores[],int jogador_atual,int posicao_monte,int monte) //ADICIONA PECA DA MESA A MAO DO JOGADOR (PODE SER USADO PARA DESFAZER PECAS DESCIDAS ERRONEAMENTE)
{
    int i = 0;
    jogador_atual --; //COMPENSAR PELO FATO DO VETOR COMECAR CONTAGEM NO 0
    if(monte < 35 && posicao_monte < 13)
    {
        if(mesa[monte].carta[posicao_monte].valor == '0')
        {
            return 1;
        }
        while(i <106 && jogadores[jogador_atual].carta[i].valor != '0') //PEGA O INDICE DA PRIMEIRA CARTA VAZIA DO JOGADOR
        {
            i++;
        }
        jogadores[jogador_atual].carta[i].valor = mesa[monte].carta[posicao_monte].valor;
        jogadores[jogador_atual].carta[i].naipe = mesa[monte].carta[posicao_monte].naipe;
        //DESLOCA AS OUTRAS PECAS DO MONTE (PARA OCUPAR O LUGAR)
        for(i = posicao_monte; i < 12; i++)
        {
          mesa[monte].carta[i].valor = mesa[monte].carta[i+1].valor;
          mesa[monte].carta[i].naipe = mesa[monte].carta[i+1].naipe;
        }
        mesa[monte].carta[12].valor = '0';
        mesa[monte].carta[12].naipe = '0';
    }
    else
    {
        return 2;
    }
    return 0;
}

int manipular_mesa(t_mesa mesa[],int monte_1,int monte_2,int posicao_monte_1,int posicao_monte_2) //MUDA A POSICAO DE UMA PECA
{
    //VERIFICAR SE A PECA EXISTE
    if(mesa[monte_1].carta[posicao_monte_1].valor == '0')
    {
        return 1; //PECA NAO EXISTE
    }
    //VERIFICA SE A POSICAO DESTINO EH VALIDA (DEVE SER NO INICIO, MEIO, OU NA ULTIMA POSICAO NAO-NULA DE UM MONTE)
    if(posicao_monte_2 != 0 && mesa[monte_2].carta[posicao_monte_2 - 1].valor == 0) //CASO O VALOR ANTES SEJA NULO
    {
        return 2; //POSICAO INVALIDA
    }

    //VERIFICAR SE MONTE "DESTINO" ESTA CHEIO
    int i = 0;
    while(mesa[monte_2].carta[i].valor != '0')
    {
        i++;
        if(i == 13) //POSICAO 13 NAO EXISTE NA MESA (TEM NO MAX 13 CARTAS)
        {
            return 3; //MONTE ESTA CHEIO
        }
    }
    //MUDA A POSICAO
    int pos;
    //DESLOCA AS PECAS DO SEGUNDO MONTE PARA REBER A NOVA PECA NA POSICAO CORRETA
    for(pos = 12; pos > posicao_monte_2; pos--)
    {
        mesa[monte_2].carta[pos].valor = mesa[monte_2].carta[pos-1].valor; //DESLOCA TODAS PARA A ESQUERDA
        mesa[monte_2].carta[pos].naipe = mesa[monte_2 ].carta[pos-1].naipe;
    }
    //COLOCA A PECA
    mesa[monte_2].carta[posicao_monte_2].valor = mesa[monte_1].carta[posicao_monte_1].valor;
    mesa[monte_2].carta[posicao_monte_2].naipe = mesa[monte_1].carta[posicao_monte_1].naipe;
    //AJEITA O MONTE ANTIGO
    mesa[monte_1].carta[posicao_monte_1].valor = '0';
    mesa[monte_1].carta[posicao_monte_1].naipe = '0';
    for(pos = posicao_monte_1; pos<12; pos++)
    {
        mesa[monte_1].carta[pos].valor =  mesa[monte_1].carta[pos+1].valor;
        mesa[monte_1].carta[pos].naipe =  mesa[monte_1].carta[pos+1].naipe;
    }
    return 0;
}

int conferir_jogada(t_mao jogadores[], t_mao jogadores_back[], t_mesa mesa[], int jogador_atual) //CONFERE SE A JOGADA FOI VALIDA
{
    /*ESSA FUNCAO DETERMINA SE TODOS OS MONTES DA MESA SAO VALIDOS E SE O JOGADOR NAO PEGOU PECAS DA MESA
              - RETORNA 0 SE OS MONTES SAO VALIDOS E PODE TERMINAR A JOGADA
              - RETORNA -1 SE ALGUM MONTE FOR INVALIDO
              - RETORNA 1 SE OS MONTES SAO VALIDOS, MAS JOGADOR PRECISA COMPRAR UMA PECA */
    jogador_atual --; //PARA COMPENSAR O FATO DOS VETORES COMECAREM NO 0 AO INVES DO 1
    //CONFERIR MONTES (SE FORMAM SETs VALIDO)
    int monte; //SAO ATE 35 MONTES (0-34)
    int n_montes = 0; //GUARDA NUMERO DE MONTES NAO-NULOS
    for(monte = 0; monte<35; monte++)
    {
        if(mesa[monte].carta[0].valor != '0')
        {
            n_montes ++;
        }
        else
        {
            break;
        }
    } //SAI DAQUI COM O NUMERO DE MONTES NAO-NULOS
    if(n_montes > 0) //CASO A MESA N ESTEJA VAZIA, VERIFIQUE OS MONTES
    {
        for(monte = 0; monte < n_montes; monte++) //EXECUTA OS TESTES DOS MONTES NAO-NULOS, CASO UM MONTE NAO SEJA VALIDO, AO FINAL DO FOR "return -1"
        {
            //CONTA NUMERO DE CARTAS NO MONTE ATUAL
            int n_cartas = 0;
            int i = 0;
            while(i<13 && mesa[monte].carta[i].valor != '0') //CONTA QUANTAS CARTAS NAO NULAS EXISTEM
            {
                n_cartas++;
                i++;
            }
            if(n_cartas < 3) //SE TEM MENOS DE 3 CARTAS, NAO VALE
            {
                printf("RETORNO 1");
                return -1; //MONTE INVALIDO
            }

            if(n_cartas == 3) //TESTA TRINCA
            {
                if(mesa[monte].carta[0].naipe != '*' && mesa[monte].carta[1].naipe != '*' && mesa[monte].carta[2].naipe != '*') //SEM CORINGAS
                {
                    if(mesa[monte].carta[0].valor == mesa[monte].carta[1].valor && mesa[monte].carta[1].valor == mesa[monte].carta[2].valor) //COMPARA VALORES
                    {
                        if(mesa[monte].carta[0].naipe != mesa[monte].carta[1].naipe && mesa[monte].carta[1].naipe != mesa[monte].carta[2].naipe) //COMPARA NAIPES
                        {
                            continue; //O MONTE EH UMA TRINCA
                        }
                    }
                }
                else   //CASO TENHA ALGUM CORINGA
                {
                    if(mesa[monte].carta[0].naipe == '*' && mesa[monte].carta[1].naipe != '*' && mesa[monte].carta[2].naipe != '*') //SE SO PRIMEIRO FOR CORINGA
                    {
                        if(mesa[monte].carta[1].valor == mesa[monte].carta[2].valor && mesa[monte].carta[1].naipe != mesa[monte].carta[2].naipe) //COMPARA VALORES E NAIPES
                        {
                            continue; //EH TRINCA
                        }
                    }
                    else if(mesa[monte].carta[0].naipe != '*' && mesa[monte].carta[1].naipe == '*' && mesa[monte].carta[2].naipe != '*')  //SE SO SEGUNDO FOR CORINGA
                    {
                        if(mesa[monte].carta[0].valor == mesa[monte].carta[2].valor && mesa[monte].carta[0].naipe != mesa[monte].carta[2].naipe) //COMPARA VALORES E NAIPES
                        {
                            continue; //EH TRINCA
                        }
                    }
                    else if(mesa[monte].carta[0].naipe != '*' && mesa[monte].carta[1].naipe != '*' && mesa[monte].carta[2].naipe == '*')  //SE SO TERCEIRO FOR CORINGA
                    {
                        if(mesa[monte].carta[0].valor == mesa[monte].carta[1].valor && mesa[monte].carta[0].naipe != mesa[monte].carta[1].naipe) //COMPARA VALORES E NAIPES
                        {
                            continue; //EH TRINCA
                        }
                    }
                    else   //SE DOIS SAO CORINGAS
                    {
                        continue; //SE TEM 2 CORINGAS, EH TRINCA
                    }
                }
            }

            if(n_cartas == 4) //TESTA QUADRA
            {
                int flag = 1; //ASSUMA A QUADRA VALIDA INICIALMENTE
                if(mesa[monte].carta[0].naipe != '*') //SE O PRIMEIRO NAO FOR CORINGA
                {
                    for(i=1; i<4; i++)
                    {
                        if(mesa[monte].carta[i].naipe != '*') //SO VERIFICA NAO CORINGAS
                        {
                            if(mesa[monte].carta[0].valor != mesa[monte].carta[i].valor || mesa[monte].carta[0].naipe == mesa[monte].carta[i].naipe)
                            {
                                flag = 0;
                            }
                        }
                    }
                    if(flag)
                    {
                        continue; //EH UMA QUADRA
                    }
                }
                else if(mesa[monte].carta[0].naipe != '*')  //SE O SEGUNDO NAO FOR CORINGA (E O PRIMEIRO FOR)
                {
                    for(i=2; i<4; i++)
                    {
                        if(mesa[monte].carta[i].naipe != '*') //SO VERIFICA NAO CORINGAS
                        {
                            if(mesa[monte].carta[1].valor != mesa[monte].carta[i].valor || mesa[monte].carta[1].naipe == mesa[monte].carta[i].naipe)
                            {
                                flag = 0;
                            }
                        }
                    }
                    if(flag)
                    {
                        continue; //EH UMA QUADRA
                    }
                }
                else
                {
                    if(mesa[monte].carta[2].valor != mesa[monte].carta[3].valor || mesa[monte].carta[2].naipe == mesa[monte].carta[3].naipe)
                    {
                        flag = 0;
                    }
                    if(flag)
                    {
                        continue; //EH UMA QUADRA
                    }
                }
            }

            //TESTA SEQUENCIA
            int flag = 1; //ASSUMA A SEQUENCIA COMO VALIDA INICIALMENTE
            if(mesa[monte].carta[0].naipe != '*') //SE O PRIMEIRO NAO FOR CORINGA
            {
                for(i=1; i<n_cartas; i++)
                {
                    if(mesa[monte].carta[i].naipe != '*') //IGNORE OS CORINGAS
                    {
                        int valor_1; //TRANSFORMA O VALOR DA CARTA EM INT
                        int valor_2; // TRANSFORMA O VALOR DA CARTA EM INT
                        if(mesa[monte].carta[0].valor < 'A') //SE O CHAR FOR NUM
                        {
                          valor_1 = mesa[monte].carta[0].valor - '0';
                        }else{ //SE O CHAR FOR LETRA
                          valor_1 = (mesa[monte].carta[0].valor - 'A') + 10;
                        }
                         if(mesa[monte].carta[i].valor < 'A') //SE O CHAR FOR NUM
                        {
                          valor_2 = mesa[monte].carta[i].valor - '0';
                        }else{ //SE O CHAR FOR LETRA
                          valor_2 = (mesa[monte].carta[i].valor - 'A') + 10;
                        }
                        if(valor_1 != (valor_2 - i) || mesa[monte].carta[0].naipe != mesa[monte].carta[i].naipe)
                        {
                            flag = 0; //SEQUENCIA E INVALIDA
                            break;
                        }
                    }
                }
                if(flag)
                {
                    continue; //EH UMA SEQUENCIA VALIDA
                }
            }
            else if(mesa[monte].carta[1].naipe != '*')  //SE O SEGUNDO NAO FOR CORINGA (E O PRIMEIRO FOR)
            {
                for(i=2; i<n_cartas; i++)
                {
                    if(mesa[monte].carta[i].naipe != '*') //IGNORE OS CORINGAS
                    {
                        int valor_1; //TRANSFORMA O VALOR DA CARTA EM INT
                        int valor_2; // TRANSFORMA O VALOR DA CARTA EM INT
                        if(mesa[monte].carta[1].valor < 'A') //SE O CHAR FOR NUM
                        {
                          valor_1 = mesa[monte].carta[1].valor - '0';
                        }else{ //SE O CHAR FOR LETRA
                          valor_1 = (mesa[monte].carta[1].valor - 'A') + 10;
                        }
                         if(mesa[monte].carta[i].valor < 'A') //SE O CHAR FOR NUM
                        {
                          valor_2 = mesa[monte].carta[i].valor - '0';
                        }else{ //SE O CHAR FOR LETRA
                          valor_2 = (mesa[monte].carta[i].valor - 'A') + 10;
                        }
                        if(valor_1 != (valor_2 - i + 1) || mesa[monte].carta[2].naipe != mesa[monte].carta[i].naipe)
                        {
                            flag = 0; //SEQUENCIA E INVALIDA
                            break;
                        }
                    }
                }
                if(flag)
                {
                    continue; //EH UMA SEQUENCIA VALIDA
                }
            }
            else  //CASO OS DOIS PRIMEIROS SEJAM CORINGAS
            {
                for(i=3; i<n_cartas; i++)
                {
                        int valor_1; //TRANSFORMA O VALOR DA CARTA EM INT
                        int valor_2; // TRANSFORMA O VALOR DA CARTA EM INT
                        if(mesa[monte].carta[2].valor < 'A') //SE O CHAR FOR NUM
                        {
                          valor_1 = mesa[monte].carta[2].valor - '0';
                        }else{ //SE O CHAR FOR LETRA
                          valor_1 = (mesa[monte].carta[2].valor - 'A') + 10;
                        }
                         if(mesa[monte].carta[i].valor < 'A') //SE O CHAR FOR NUM
                        {
                          valor_2 = mesa[monte].carta[i].valor - '0';
                        }else{ //SE O CHAR FOR LETRA
                          valor_2 = (mesa[monte].carta[i].valor - 'A') + 10;
                        }
                    if(valor_1 != (valor_2 - i + 2) || mesa[monte].carta[2].naipe != mesa[monte].carta[i].naipe)
                    {
                        flag = 0; //SEQUENCIA E INVALIDA
                        break;
                    }
                }
                if(flag)
                {
                    continue; //EH UMA SEQUENCIA VALIDA
                }
            }
            printf("RETORNO 2 - monte %d", monte);
            return -1; //SE CHEGOU AQUI (NAO PASSOU EM NENHUM DOS TESTES), MONTE INVALIDO
        } //SE SAIU DESSE LOOP, TODOS OS MONTES SAO VALIDOS!
    }

    //CONFERIR MAO DO JOGADOR (SE NAO PEGOU PECAS DA MESA)
    /*TODAS AS CARTAS ATUAIS NA MAO DO JOGADOR DEVIAM ESTAR ANTERIORMENTE NA MAO DELE*/
    int i= 0;
    int numero_cartas_atual = 0, numero_cartas_antes = 0;
    while(i<106 && jogadores[jogador_atual].carta[i].valor != '0')
    {
        numero_cartas_atual++;
        i++;
    }
    i = 0;
    while(i<106 && jogadores_back[jogador_atual].carta[i].valor != '0')
    {
        numero_cartas_antes++;
        i++;
    }
    if(numero_cartas_atual > numero_cartas_antes)
    {
        printf("RETORNO 3");
        return -1; //JOGADA INVALIDA! JOGADOR NAO PODE RECEBER MAIS CARTAS (ANTES DE COMPRAR)
    }
    //CONTA QUANTAS PECAS DE CADA TIPO A MAO ATUAL POSSUI, E COMPARA SE ELAS EXISTIAM ANTERIORMENTE
    int numero_atual;
    int numero_antes;
    char valor, naipe;
    int j = 0;
    i = 0;
    while(i<106 && jogadores[jogador_atual].carta[i].valor != '0') //TESTA PARA TODAS AS CARTAS
    {
        valor = jogadores[jogador_atual].carta[i].valor;
        naipe = jogadores[jogador_atual].carta[i].naipe;
        while(j<106 && jogadores[jogador_atual].carta[j].valor != '0')
        {
            numero_atual = 0;
            numero_antes = 0;
            if(jogadores[jogador_atual].carta[j].valor == valor && jogadores[jogador_atual].carta[j].naipe == naipe) //CONTA QUANTAS CARTAS IGUAIS A ESSA EXISTEM ATUALMENTE
            {
                numero_atual ++;
            }
            if(jogadores_back[jogador_atual].carta[j].valor == valor && jogadores[jogador_atual].carta[j].naipe == naipe) //CONTA QUANTAS CARTAS IGUAIS A ESSA EXISTIAM ANTES
            {
                numero_antes ++;
            }
            if(numero_atual > numero_antes)
            {
                printf("ANT: %d DEPS: %d\nRETORNO 4\n", numero_antes, numero_atual); //TA DANDO RUIM AQUIII
                return -1; //JOGADA INVALIDA! JOGADOR NAO PODE POSSUIR CARTA QUE NAO POSSUIA ANTES
            }
            j++;
        }
        i++;
    }

    //CONTA NUMERO DE PONTOS JOGADOS (CASO SEJA PRIMEIRA JOGADA, PRECISAM TER 30 PONTOS JOGADOS)
    /* FUNCAO CALCULA NUMERO DE PONTOS QUE EXISTIAM ANTES NA MAO, E SUBTRAI DO NUMERO ATUAL DE PONTOS (A DIFERENCA E O NUMERO DE PONTOS JOGADOS) */
    int numero_pontos_atual = 0;
    int numero_pontos_anterior = 0;
    i=0;
    while(i < 106 && jogadores[jogador_atual].carta[i].valor != '0') //CONTA NUMERO DE PONTOS ATUAIS
    {
        if(jogadores[jogador_atual].carta[i].valor >= 'A' && jogadores[jogador_atual].carta[i].valor <= 'D') //SE FOR LETRA
        {
          numero_pontos_atual += (jogadores[jogador_atual].carta[i].valor - 'A') + 10; //CONVERTE A LETRA PRA INT
        }else if(jogadores[jogador_atual].carta[i].valor != '*'){
          numero_pontos_atual += (jogadores[jogador_atual].carta[i].valor - '0'); //CONVERTE CHAR (NUM) PRA INT
        }else{
          numero_pontos_atual += 20; //CORINGA VALE 20 PONTOS
        }
        i++;
    }
    i=0;
    while(i < 106 && jogadores_back[jogador_atual].carta[i].valor != '0') //CONTA NUMERO DE PONTOS ANTIGO
    {
        if(jogadores_back[jogador_atual].carta[i].valor >= 'A' && jogadores_back[jogador_atual].carta[i].valor <= 'D') //SE FOR LETRA
        {
          numero_pontos_anterior += (jogadores_back[jogador_atual].carta[i].valor - 'A') + 10; //CONVERTE A LETRA PRA INT
        }else if(jogadores_back[jogador_atual].carta[i].valor != '*'){
          numero_pontos_anterior += (jogadores_back[jogador_atual].carta[i].valor - '0'); //CONVERTE CHAR (NUM) PRA INT
        }else{
          numero_pontos_anterior += 20; //CORINGA VALE 20 PONTOS
        }
        i++;
    }
    int numero_pontos = ((numero_pontos_anterior) - (numero_pontos_atual));
    return numero_pontos;
}

int comprar_carta(t_carta monte_baralho[], t_mao jogadores[], int jogador_atual)
{
  jogador_atual --; //COMPENSAR O FATO DOS VETORES COMECAREM A CONTAR NO 0 AO INVES DO 1
  //PEGA PRIMEIRA CARTA DO MONTE (A PRIMEIRA CARTA NAO-NULA)
  int i;
  t_carta carta_comprada;
  for(i = 0; i<106; i++)
  {
    if(monte_baralho[i].valor != '0')
    {
      carta_comprada.valor = monte_baralho[i].valor;
      carta_comprada.naipe = monte_baralho[i].naipe;
      monte_baralho[i].valor = '0';
      monte_baralho[i].naipe = '0';
      break;
    }
  }
  if(i == 106)
  {
    return 1; //MONTE CHEIO
  }
  //DE A CARTA PARA O JOGADOR ATUAL
    //ACHA A PRIMEIRA POSICAO VAZIA NA MAO DO JOGADOR
    i = 0;
    while(jogadores[jogador_atual].carta[i].valor != '0')
    {
      i++;
    }
    jogadores[jogador_atual].carta[i].valor = carta_comprada.valor;
    jogadores[jogador_atual].carta[i].naipe = carta_comprada.naipe;
    return 0;
}

int jogada(t_carta monte_baralho[], t_mao jogadores[], t_mesa mesa[],int numero_jogadores, int jogador_atual) //FUNCAO QUE REALIZA AS JOGADAS
{
    //BACKUP DO ESTADO DO JOGO (PARA PODER RESTAURAR AO PONTO ANTERIOR A JOGADA COMECAR)
    t_carta monte_baralho_back[106];
    t_mao jogadores_back[numero_jogadores];
    t_mesa mesa_back[35];
    backup_jogo(monte_baralho, monte_baralho_back, jogadores, jogadores_back, mesa, mesa_back, numero_jogadores);
    int pontos; //CALCULA QUANTOS PONTOS A JOGADA VALE (PECAS QUE O JOGADOR ABAIXOU)
    int modo_escolhido;
    while(true)
    {
        imprimir_tela(jogadores, numero_jogadores, mesa, jogador_atual);
        printf("1- DESCER UMA PECA\n");
        printf("2- PEGAR PECA DA MESA\n");
        printf("3- MOVIMENTAR PECAS NA MESA\n");
        printf("4- RESETAR JOGADA\n");
        printf("(DIGITE 0 PARA ENCERRAR JOGADA)\n");
        while(true)
        {
            printf("DIGITE O MODO: ");
            fflush(stdin);
            scanf("%d", &modo_escolhido);
            if(modo_escolhido >4 || modo_escolhido <0)
            {
                printf("MODO INVALIDO!\n");
            }
            else
            {
                break;
            }
        }

        if(modo_escolhido == 1) //DESCER UMA PECA
        {
            int flag;
            int posicao_peca; //POSICAO NA MAO DA PECA QUE O JOGADOR QUER DESCER
            int monte; //PARA QUAL MONTE O JOGADOR QUER DESCER A PECA
            printf("QUAL PECA DESEJAS ADICIONAR A MESA? ");
            fflush(stdin);
            scanf("%d",&posicao_peca);
            printf("EM QUAL MONTE DESEJAS ADICIONAR A PECA? ");
            fflush(stdin);
            scanf("%d",&monte);
            flag = descer_peca(mesa, jogadores, jogador_atual, posicao_peca, monte);
            if(flag == 1)
            {
                printf("PECA INEXISTENTE!");
                tres_pontinhos();
            }
            else if(flag == 2)
            {
                printf("MONTE INVALIDO!");
                tres_pontinhos();
            }
            else if(flag == 3)
            {
                printf("MONTE CHEIO!");
                tres_pontinhos();
            }
            continue;
        }

        if(modo_escolhido == 2) //PEGAR PECA DA MESA
        {
            int monte;
            int posicao_monte;
            int flag;
            printf("DE QUAL MONTE DESEJAS PEGAR A PECA? ");
            fflush(stdin);
            scanf("%d",&monte);
            printf("EM QUAL POSICAO DO MONTE? ");
            fflush(stdin);
            scanf("%d",&posicao_monte);
            flag = pegar_peca(mesa, jogadores, jogador_atual, posicao_monte, monte);
            if(flag == 1) //CARTA NAO EXISTE
            {
                printf("CARTA NAO EXISTE");
                tres_pontinhos();
            }
            if(flag == 2) //VALOR DO MONTE OU DA POS INVALIDO
            {
                printf("VALOR DO MONTE OU DA POSICAO INVALIDO");
                tres_pontinhos();
            }
            continue;
        }

        if(modo_escolhido == 3) //MANIPULAR MESA
        {
            int monte_1;
            int posicao_monte_1;
            int monte_2;
            int posicao_monte_2;
            int flag;
            printf("DE QUAL MONTE DESEJAS PEGAR A PECA? ");
            fflush(stdin);
            scanf("%d",&monte_1);
            printf("EM QUAL POSICAO DO MONTE? ");
            fflush(stdin);
            scanf("%d",&posicao_monte_1);
            printf("EM QUAL MONTE DESEJAS COLOCAR A PECA? ");
            fflush(stdin);
            scanf("%d",&monte_2);
            printf("EM QUAL POSICAO DO MONTE? ");
            fflush(stdin);
            scanf("%d",&posicao_monte_2);
            flag = manipular_mesa(mesa, monte_1, monte_2, posicao_monte_1, posicao_monte_2);
            if(flag == 1) //PECA NAO EXISTE
            {
                printf("PECA NAO EXISTENTE");
                tres_pontinhos();
            }
            if(flag == 2) //POSICAO DESTINO INVALIDA
            {
                printf("POSICAO DE DESTINO INVALIDA");
                tres_pontinhos();
            }
            if(flag == 3) //MONTE DE DESTINO CHEIO
            {
                printf("MONTE DE DESTINO CHEIO");
                tres_pontinhos();
            }
            continue;
        }

        if(modo_escolhido == 4) //RESETAR JOGADA
        {
            resetar_jogo(monte_baralho, monte_baralho_back,jogadores,jogadores_back,mesa,mesa_back,numero_jogadores);
            continue;
        }

        //VERIFICAO DA JOGADA
        pontos = conferir_jogada(jogadores, jogadores_back, mesa, jogador_atual); //CONFERE SE A JOGADA FOI VALIDA
        if(pontos == -1) //JOGADA INVALIDA
        {
            printf("JOGADA INVALIDA!\n");
            tres_pontinhos();
        }
        else if(pontos == 0) //JOGADA VALIDA, MAS JOGADOR COMPRA UMA PECA
        {
            printf("JOGOU 0 PONTOS, COMPRANDO CARTA");
            tres_pontinhos();
            int flag = 0;
            flag = comprar_carta(monte_baralho, jogadores, jogador_atual);
            if(flag)
            {
              printf("MONTE VAZIO! TERMINANDO JOGO");
              tres_pontinhos();
              return 1;
            }
            break;
        }
        else if(pontos > 0 && pontos < 30 && jogadores[jogador_atual].primeira_jogada == 0)   //JOGADA INVALIDA, PRIMEIRA JOGADA PRECISA SER DE PELO MENOS 30 PONTOS
        {
            printf("JOGOU MENOS DO QUE 30 PONTOS (%d)!\n", pontos);
            tres_pontinhos();
        }
        else if(pontos >= 30 && jogadores[jogador_atual].primeira_jogada == 0)  //JOGADA VALIDA, E EH A PRIMEIRA DE UM JOGADOR
        {
            jogadores[jogador_atual].primeira_jogada = 1;
            printf("PONTOS JOGADOS: %d", pontos);
            tres_pontinhos();
            break;
        }
        else  //QUALQUER OUTRO CASO, A JOGADA EH VALIDA
        {
            printf("PONTOS JOGADOS: %d", pontos);
            tres_pontinhos();
            break;
        }
    }
    return 0; //JOGADA NORMAL
}

int testa_vitoria(t_mao jogadores[], int jogador_atual)
{
  if(jogadores[jogador_atual-1].carta[0].valor == '0') //SE A PRIMEIRA CARTA JA FOR NULA
  {
    return 1; //GANHOU
  }
  return 0; //NAO GANHOU
}

void contagem_pontos(t_mao jogadores[], int numero_jogadores, int pontos_jogadores[], int* p_jogador_ganhador, int* p_pontos_obtidos)
{
  int carta = 0;
  int jogador;
  for(jogador = 0; jogador < numero_jogadores; jogador++)
  {
    pontos_jogadores[jogador] = 0; //ZERA O NUMERO DE PONTOS ANTES DE CONTAR
    while(carta < 106 && jogadores[jogador].carta[carta].valor != '0')
    {
      if(jogadores[jogador].carta[carta].valor != '*')
      {
        if(jogadores[jogador].carta[carta].valor >= 'A' && jogadores[jogador].carta[carta].valor <= 'D') //SE FOR LETRA
        {
          pontos_jogadores[jogador] -= (jogadores[jogador].carta[carta].valor - 'A') + 10;
        }else{
          pontos_jogadores[jogador] -= (jogadores[jogador].carta[carta].valor - '0');
        }
        carta ++;
      }else{
        pontos_jogadores[jogador] -= 20;
        carta ++;
      }
    }
  }
  int jogador_ganhador = 0; //ASSUME O PRIMEIRO COMO O GANHADOR
  for(jogador = 1; jogador < numero_jogadores; jogador++)
  {
    if(pontos_jogadores[jogador] < pontos_jogadores[jogador_ganhador])
    {
      jogador_ganhador = jogador;
    }
  }
  //CALCULA PONTOS OBTIDOS PELO GANHADOR
  int pontos_obtidos = 0; //PONTOS OBTIDOS EH A SOMA (DO MODULO) DOS PONTOS DOS OUTROS JOGADORES
  for(jogador = 0; jogador <numero_jogadores; jogador++)
   {
    if(jogador != jogador_ganhador)
    {
      pontos_obtidos += abs(pontos_jogadores[jogador]);
    }
   }
   *p_pontos_obtidos = pontos_obtidos;
   jogador_ganhador ++; //COMPENSA O FATO DOS VETORES COMECAREM NO 0, JOGADOR DO INDICE 0 NA VDD EH JOGADOR 1
   *p_jogador_ganhador = jogador_ganhador;
}
