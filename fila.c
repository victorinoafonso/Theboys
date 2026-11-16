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

    return 0;
}

// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere (struct fila_t *f, void *item)
{

    if(!f || !item)
        return ERRO;

    struct fila_nodo_t *novo_elemento = malloc(sizeof(struct fila_nodo_t));

    if(!novo_elemento)
        return ERRO;

    novo_elemento->item = item;
    novo_elemento->prox = NULL;

    if(!f->prim)
    {
        f->prim = novo_elemento;
        f->fim = novo_elemento;

    }

    else
    {
        f->fim->prox = novo_elemento;
        f->fim = novo_elemento;
    }

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