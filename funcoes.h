#ifndef FUNCOES_H
#define FUNCOES_H
#include "struct.h"

void tres_pontinhos();
void char_colorido(char texto, int cor); //IMPRIME UM TEXTO COLORIDO
void inicializar_monte(t_carta monte_baralho[]); //INICIALIZA O MONTE COM AS CARTAS ORDENADS (LE DO ARQUIVO PADRÃO)
void inicializar_cartas(t_mao jogadores[], int numero_jogadores); //INICIALZIA A MAO DOS JOGADORES COM "NADA"
void inicializar_maos(t_carta monte_baralho[], t_mao jogadores[], int numero_jogadores); //DISTRIBUI AS CARTAS
void inicializar_mesa(t_mesa mesa[]);
int embaralhar_monte(t_carta monte_baralho[]); //CRIA UM MONTE EM ORDEM ALEATORIA OU LE DE UM ARQUIVO A ORDEM
void imprimir_tela(t_mao jogadores[], int numero_jogadores, t_mesa mesa[], int jogador_atual);
void backup_jogo(t_carta monte_baralho[], t_carta monte_baralho_back[], t_mao jogadores[], t_mao jogadores_back[], t_mesa mesa[], t_mesa mesa_back[], int numero_jogadores); //BACKUP DO ESTADO ATUAL DO JOGO
void resetar_jogo(t_carta monte_baralho[], t_carta monte_baralho_back[], t_mao jogadores[], t_mao jogadores_back[], t_mesa mesa[], t_mesa mesa_back[], int numero_jogadores); //RESETA O JOGO ATUAL USANDO O BACKUP
int descer_peca(t_mesa mesa[],t_mao jogadores[],int jogador_atual,int posicao_peca, int monte); //ADICIONA A PECA DO JOGADOR A ALGUM MONTE
int pegar_peca(t_mesa mesa[],t_mao jogadores[],int jogador_atual,int posicao_monte,int monte); //ADICIONA PECA DA MESA A MAO DO JOGADOR (PODE SER USADO PARA DESFAZER PECAS DESCIDAS ERRONEAMENTE)
int manipular_mesa(t_mesa mesa[],int monte_1,int monte_2,int posicao_monte_1,int posicao_monte_2); //MUDA A POSICAO DE UMA PECA
int conferir_jogada(t_mao jogadores[], t_mao jogadores_back[], t_mesa mesa[], int jogador_atual); //CONFERE SE A JOGADA FOI VALIDA
int comprar_carta(t_carta monte_baralho[], t_mao jogadores[], int jogador_atual);
int jogada(t_carta monte_baralho[], t_mao jogadores[], t_mesa mesa[],int numero_jogadores, int jogador_atual); //FUNCAO QUE REALIZA AS JOGADAS
int testa_vitoria(t_mao jogadores[], int jogador_atual);
void contagem_pontos(t_mao jogadores[], int numero_jogadores, int pontos_jogadores[], int* p_jogador_ganhador, int* p_pontos_obtidos);

#endif

