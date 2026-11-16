// TAD Fila genérica
// Implementa a logica usada para a fila de espera
// Definição da implementação
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include "fila.h"
#include "defines.h"


// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria ()
{
    struct fila_t *fila = malloc(sizeof(struct fila_t));

    if(!fila)
        return NULL;

    memset(fila, 0, sizeof(struct fila_t));

    return fila;

}


// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f)
{
    if(!f)
        return NULL;

    struct fila_nodo_t *aux = f->prim;

    while (aux)
    {
        f->prim = aux->prox;

        free(aux->item);
        free(aux);
        aux = f->prim;
    }

    free(f);

    return NULL;
}

// Busca na fila se o item ja existe
// Retorna 0 se não encontar e -1 se encontar
static inline int existe(struct fila_t *f, void *item)
{
    if(!f)
        return ERRO;

    struct fila_nodo_t *aux = f->prim;

    while (aux)
    {
        if (aux->item == item)
            return ERRO;

        aux = aux->prox;
    }

    return SUCESSO;
}

// Cria e inicializa um novo nodo da fila
static inline struct fila_nodo_t *cria_nodo(void *item)
{
    struct fila_nodo_t *novo = malloc(sizeof(struct fila_nodo_t));
    if (!novo)
        return NULL;

    novo->item = item;
    novo->prox = NULL;

    return novo;
}

// Encaixa o novo nodo sempre no final da fila (FIFO)
static void insere_final(struct fila_t *f, struct fila_nodo_t *novo)
{
    // Fila vazia
    if (!f->prim) 
    {
        f->prim = novo;
        f->fim = novo;
    }

    else 
    {
        f->fim->prox = novo;
        f->fim = novo;
    }
}

// Insere o item na fila.
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere(struct fila_t *f, void *item)
{
    if (!f || !item)
        return ERRO;

    // Verifica se já existe antes de alocar memória
    if (existe(f, item) == ERRO)
        return ERRO;

    //Aloca o novo nodo
    struct fila_nodo_t *novo_elemento = cria_nodo(item);
    if (!novo_elemento)
        return ERRO;

    // Conecta no final da fila
    insere_final(f, novo_elemento);

    return ++(f->num);
}


// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira (struct fila_t *f)
{
    if(!f || !f->prim)
        return NULL;

    struct fila_nodo_t *aux = f->prim;

    f->prim = aux->prox;
    
    if(!f->prim)
        f->fim = NULL;

    void *item = aux->item;

    free(aux);
    f->num--;

    return item; 
}


// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f)
{
    if(!f)
        return ERRO;

    return f->num;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f)
{
    if(!f || !f->prim)
        return;

    struct fila_nodo_t *aux = f->prim;

    printf(" FILA [ ");

    while(aux)
    {
        printf("%2d ", *(int *)aux->item);

        aux = aux->prox;

    }

    printf("]\n");

}