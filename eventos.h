// TAD Eventos
// Implenta os eventos da simulação
// Definição da interface

#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"
#include "fprio.h"
#include "simulacao.h"

//Evento que reprenta um heroi chegando em uma base num instante de tempo 
void evento_chega(struct fprio_t *lef, mundo_t *mundo, dados_t *item );

//Evento que adiciona o heroi na fila de espera da uma base
void evento_espera(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento que representa o heroi desistindo de uma base
void evento_desiste(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento que representa o Aviso do porteiro
//Insere os gerois da fila para o conjunto de presentes da base
void evento_avisa(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento que representa o heroi entrando em uma base
void evento_entra(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

// Evento que representa um heroi saindo de uma base
void evento_sai(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento que representa o heroi viajando
void evento_viaja(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento que representa um heroi morrendo
void evento_morre(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento que representa a missao de cada heroi
void evento_missao(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento querepresenta a invansao em uma base
void evento_invade(struct fprio_t *lef, mundo_t *mundo, dados_t *item);

//Evento que representa recuperacao do heroi
void evento_recupera_heroi(dados_t *item);

//Evento que representa recuperacao a base
void evento_reconstroi_base(dados_t *item);

// Representa o final do mundo da simulação
void evento_fim(mundo_t *mundo);








#endif