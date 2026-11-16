// TAD Entidades
// Implenta as entidades do programa e suas inicializações
// Definição da interface

#ifndef ENTIDADES
#define ENTIDADES


//Tipo de dados heroi
//Estrutura que implementa a entidade Heroi
typedef struct heroi
{
    int id;
    struct cjto_t *habilidades;
    int estado;
    int paciencia;
    int velocidade;
    int experiencia;
    int base_atual;

} heroi_t;

// Tipo de dados local
// Representa as coordenadas internas de localização
typedef struct local
{
    int x;
    int y;

} local_t;

//Tipo de dados base
//Estrutura que implementa a entidades base
typedef struct base
{
    int id;
    int lotacao;
    struct cjto_t *presentes;
    int estado;
    struct fila_t *espera;
    local_t local;
    int fila_max;
    int missoes_cumpridas;
    
} base_t;

//Tipo de dados missao
//Estrutura que implementa a entidade Missão
typedef struct missao
{
    int id;
    struct cjto_t *habilidades;
    int perigo;
    local_t local;
    int tentativas;
    int cumprida;

} missao_t;

//Tipo de dados invasao
//Estrutura que implementa a entidade Invasão
typedef struct invasao
{
    int id;
    int local;
    int pen_base;
    int pen_heroi;
    
} invasao_t;

// Estrutura que implementa a entidade Mundo
//Tipo de dados mundo
typedef struct mundo
{
    int nHerois;
    heroi_t *herois;
    int nBases;
    base_t *bases;
    int nMissoes;
    missao_t *missoes;
    int nInvasoes;
    invasao_t *invasoes;
    int nHabilidades;
    int tamanho_mundo;
    int relogio;
    int eventos_tratados;
    int invasoes_realizadas;

} mundo_t;

//Destroi o mundo virtual e libera  a memória
//Retorna NULL
void destroi_mundo(mundo_t **mundo);

// Inicialização do mundo virtual
//Retorna um ponteiro para o mundo virtual iniciado ou NULL se erro
mundo_t *inicia_mundo();













#endif