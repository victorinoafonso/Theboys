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

// Busca na fila se o item ja existe
// Retorna 0 se não encontar e -1 se encontar
int existe(struct fprio_t *f, void *item)
{
    struct fpnodo_t *aux = f->prim;

    while (aux)
    {
        if (aux->item == item)
            return ERRO;

        aux = aux->prox;
    }

    return 0;
}

// Insere se a fila estiver vazia
int insere_vazio(struct fprio_t *f, struct fpnodo_t *elemento)
{
    f->prim = elemento;
    f->fim = elemento;
    elemento->prox = NULL;

    return 0;
}

// Insere no inicio da fila
int insere_inicio(struct fprio_t *f, struct fpnodo_t *elemento)
{
    elemento->prox = f->prim;
    f->prim = elemento;

    return 0;
}

// Insere no final da fila
int insere_final(struct fprio_t *f, struct fpnodo_t *elemento)
{
    f->fim->prox = elemento;
    f->fim = elemento;
    elemento->prox = NULL;

    return 0;
}

// Insere o item no meio da fila
int insere_meio(struct fprio_t *f, struct fpnodo_t *novo_elemento)
{
    struct fpnodo_t *ant, *act = f->prim;

    while (act->prio <= novo_elemento->prio && act->prox)
    {
        ant = act;
        act = ant->prox;
    }

    ant->prox = novo_elemento;
    novo_elemento->prox = act;

    return 0;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio)
{
    // Verifica se os ponteiros são nulos ou se ja existe o item na fila
    // Retorna um erro
    if (!f || !item || existe(f, item))
        return ERRO;

    struct fpnodo_t *novo_elemento = malloc(sizeof(struct fpnodo_t));

    if (!novo_elemento)
        return ERRO;

    novo_elemento->item = item;
    novo_elemento->tipo = tipo;
    novo_elemento->prio = prio;

    // Caso a fila esteje vazia
    if (!f->prim)
    {
        insere_vazio(f, novo_elemento);
    }

    // Caso a prioridade for menor que a do primeiro elemento
    else if (novo_elemento->prio < f->prim->prio)
    {
        insere_inicio(f, novo_elemento);
    }

    // Caso a prioridade se maior ou igual do que o ultimo elemento
    else if (novo_elemento->prio >= f->fim->prio)
    {
        insere_final(f, novo_elemento);
    }

    // Caso geral
    else
    {
        insere_meio(f, novo_elemento);
    }

    return ++(f->num);
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira(struct fprio_t *f, int *tipo, int *prio)
{
    //Verifica se os ponteiros são nulos ou a fila esta vazia
    //Retorna null
    if (!f || !prio || !tipo || !f->prim )
        return NULL;

    struct fpnodo_t *aux = f->prim;

    f->prim = aux->prox;
    
    if(!f->prim)
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
int fprio_tamanho (struct fprio_t *f)
{
  if(!f)
    return ERRO;
  
  return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f)
{
  if(!f)
    return;
  
  struct fpnodo_t *aux = f->prim;

  while(aux)
  {
    printf("(%d %d)", aux->tipo, aux->prio);

    if(aux->prox)
      printf(" ");
    
    aux = aux->prox;
  }
}


