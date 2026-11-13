// TAD Entidades
// Implenta as entidades do programa e suas inicializações
// Definição da interface

#ifndef ENTIDADES
#define ENTIDADES


//Tipo de dados heroi
typedef struct heroi heroi_t;

//Tipo de dados base
typedef struct base base_t;

//Tipo de dados missao
typedef struct missao missao_t;

//Tipo de dados invasao
typedef struct invasao invasao_t;

//Tipo de dados mundo
typedef struct mundo mundo_t;

//Destroi o mundo virtual e libera  a memória
//Retorna NULL
void destroi_mundo(mundo_t **mundo);

// Inicialização do mundo virtual
//Retorna um ponteiro para o mundo virtual iniciado ou NULL se erro
mundo_t *inicia_mundo();













#endif