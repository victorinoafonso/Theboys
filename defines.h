//Arquivo que contém todos as constantes 
//E funções uteis para o funcionamento do programa

#ifndef DEFINES
#define DEFINES




//Defines do mundo
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 // 60 * 24 * 365 dias (1 ano)
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS 50 // N_HABILIDADES * 5
#define N_BASES 10 //N_HEROIS / 5
#define N_MISSOES 5256 //T_FIM_DO_MUNDO / 100
#define N_INVASOES 52 //T_FIM_DO_MUNDO / 10000

//Defines de estados
#define ATIVO 1
#define INATIVO 0
#define MORTO -1

//Defines de retorno
#define ERRO -1
#define SUCESSO 1

//Defines booleanos
#define TRUE 1
#define FALSE 0

//Defines de eventos
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define INVADE 10
#define REC_HEROI 11
#define REC_BASE 12
#define FIM 13


// Gerador de números aleatórios entre min e max
// Retorna um número inteiro gerado aleatoriamente num intervalo(min, max)
static inline int aleat(int min, int max)
{
    return rand() % (max - min + 1) + min;
}





#endif