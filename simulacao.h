// TAD Simulaçaõ
// Implenta o estado inicial da SED (simulação de eventos discretos)
//Implementa o loop principal da simulalação
// Definição da interface

#ifndef SIMULACAO
#define SIMULACAO


//Tipo de dado dados dos eventos 
// Guarda as informações para execução dos eventos da LEF
typedef struct dados
{
    void *dados1; //Tipo de dados generico
    void *dados2; //Tipo de dados generico

} dados_t;


//Insere na LEF os eventos iniciais da simulação 
//Retorna -1 se erro ou 1 se sucesso
int eventos_iniciais(struct fprio_t *lef, mundo_t *mundo);

// Laço principal da Simulação
void simulacao(struct fprio_t *lef, mundo_t *mundo);

//Ao terminar a simulação
//Destroi a LEF e os items nela
void destroi_simulacao(struct fprio_t *lef);


#endif