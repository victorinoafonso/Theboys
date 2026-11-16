// TAD Fila de prioridades (FPRIO) genérica
// Implementa a logica usada para o LEF (lista de eventos futuros)
// Definição da implementação
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fprio.h"
#include "defines.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria()
{
    struct fprio_t *fila = malloc(sizeof(struct fprio_t));

    if (!fila)
        return NULL;

    memset(fila, 0, sizeof(struct fprio_t));

    return fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi(struct fprio_t *f)
{
    if (!f)
        return NULL;

    struct fpnodo_t *aux;

    for (int i = 0; i < f->num; i++)
    {
        aux = f->prim;
        f->prim = aux->prox;
        free(aux->item);
        free(aux);
    }

    free(f);
    return NULL;
}

// Cria e inicializa um novo nodo
//Retorna o ponteiro para o nodo criado
static inline struct fpnodo_t *cria_nodo(void *item, int tipo, int prio)
{
    struct fpnodo_t *novo = malloc(sizeof(struct fpnodo_t));
    if (!novo)
        return NULL;

    novo->item = item;
    novo->tipo = tipo;
    novo->prio = prio;
    novo->prox = NULL;

    return novo;
}

// Percorre a fila verificando se o item já existe
// Retorna -1 se o item já existe na fila, ou 1 se pode inserir.
// Devolve em *pos_ant o nodo anterior à inserção (NULL se for no início).
static inline int busca_posicao(struct fprio_t *f, void *item, int prio, struct fpnodo_t **pos_ant)
{
    struct fpnodo_t *ant = NULL;
    struct fpnodo_t *act = f->prim;
    int achou_posicao = 0;

    *pos_ant = NULL;

    while (act)
    {
        //Verifica
        if (act->item == item)
            return ERRO; 

        //Se não achou a posição e a prioridade é maior
        if (!achou_posicao && act->prio > prio)
        {
            *pos_ant = ant;
            achou_posicao = 1;
        }

        ant = act;
        act = act->prox;
    }

    // Se não achou prioridade maior, entra depois do último elemento ('ant')
    if (!achou_posicao)
        *pos_ant = ant;

    return SUCESSO; 
}

// Encaixa o novo nodo na fila logo após 'pos_ant' (ou no início se pos_ant == NULL)
static inline void insere_apos(struct fprio_t *f, struct fpnodo_t *pos_ant, struct fpnodo_t *novo)
{
    if (!pos_ant) 
    {
        // Fila vazia ou inserção no início
        novo->prox = f->prim;

        f->prim = novo;

        if (!f->fim)
            f->fim = novo;
    }

    // Inserção no meio ou no final
    else 
    {
        novo->prox = pos_ant->prox;
        pos_ant->prox = novo;

        //Olha se vai inserir no final
        if (!novo->prox)
            f->fim = novo;
    }
}

// Função principal limpa e fácil de ler
int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio)
{
    if (!f || !item)
        return ERRO;

    struct fpnodo_t *pos_ant = NULL;

    //Verifica se o item já existe enquanto busca a posição
    if (busca_posicao(f, item, prio, &pos_ant) == ERRO)
        return ERRO;

    //Aloca o novo nodo
    struct fpnodo_t *novo = cria_nodo(item, tipo, prio);
    if (!novo)
        return ERRO;

    //Insere na fila
    insere_apos(f, pos_ant, novo);

    return ++(f->num);
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira(struct fprio_t *f, int *tipo, int *prio)
{
    // Verifica se os ponteiros são nulos ou a fila esta vazia
    // Retorna null
    if (!f || !prio || !tipo || !f->prim)
        return NULL;

    struct fpnodo_t *aux = f->prim;

    f->prim = aux->prox;

    if (!f->prim)
        f->fim = NULL;

    aux->prox = NULL;

    void *item = aux->item;
    aux->item = NULL;

    *tipo = aux->tipo;
    *prio = aux->prio;

    free(aux);

    f->num--;

    return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho(struct fprio_t *f)
{
    if (!f)
        return ERRO;

    return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime(struct fprio_t *f)
{
    if (!f)
        return;

    struct fpnodo_t *aux = f->prim;

    while (aux)
    {
        printf("(%d %d)", aux->tipo, aux->prio);

        if (aux->prox)
            printf(" ");

        aux = aux->prox;
    }
}
