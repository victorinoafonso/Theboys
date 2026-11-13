// TAD Entidades
// Implenta as entidades do programa e suas inicializações
// Definição da implementação

#include <stdlib.h>
#include <string.h>
#include "entidades.h"
#include "conjunto.h"
#include "fila.h"
#include "defines.h"

// Estrutura que implementa a entidade Heroi
struct heroi
{
    int id;
    struct cjto_t *habilidades;
    int estado;
    int paciencia;
    int velocidade;
    int experiencia;
    int base_atual;
};

// Tipo de dados local
// Representa as coordenadas internas de localização
typedef struct local
{
    int x;
    int y;

} local_t;

// Estrutura que implementa a entidades base
struct base
{
    int id;
    int lotacao;
    struct cjto_t *presentes;
    int estado;
    struct fila_t *espera;
    local_t local;
};

// Estrutura que implementa a entidade Missão
struct missao
{
    int id;
    struct cjto_t *habilidades;
    int perigo;
    local_t local;
};

// Estrutura que implementa a entidade Invasão
struct invasao
{
    int id;
    int local;
    int pen_base;
    int pen_heroi;
};

// Estrutura que implementa a entidade Mundo
struct mundo
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
};

// Gerador de números aleatórios entre min e max
// Retorna um número inteiro gerado aleatoriamente num intervalo(min, max)
int aleat(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

//Libera a memória alocada para o vetor de herois apontado por heroi
//Libera a memória alocada para conjunto de habilidades
//Atribui NULL ao ponteiro
void destroi_herois(heroi_t **heroi, int nHerois)
{
    if(!heroi || !*heroi)
        return;

    for (int i = 0; i < nHerois; i++)
        cjto_destroi((*heroi)[i].habilidades);

    free(*heroi);

    *heroi = NULL;  
}

// Inicialização de cada Heroi
// Retorna um ponteiro para o vetor de Herois ou NULL se erro
heroi_t *inicia_heroi(int nHerois)
{
    heroi_t *heroi = malloc(nHerois * sizeof(heroi_t));

    if (!heroi)
        return NULL;

    for (int i = 0; i < nHerois; i++)
    {
        heroi[i].id = i;

        // Conjunto com tamanho aleatório(1,3)
        // Escolhidas entre as habilidades possiveis
        heroi[i].habilidades = cjto_aleat(
            aleat(1, 3),
            N_HABILIDADES
        );

        //Caso a criação do conjunto de errado
        //Libera as memorias que ja foram alocadas e  destroi o vetor
        //Retorna NULL
        if(!heroi[i].habilidades)
        {
            destroi_herois(&heroi,i);
            return NULL;
        }

        heroi[i].estado = ATIVO; // 1 ativo e 0 inativo
        heroi[i].paciencia = aleat(0, 100);
        heroi[i].velocidade = aleat(50, 5000);
        heroi[i].experiencia = 0;
        heroi[i].base_atual = -1; // -1 quando não esta em nenhuma base 
    }

    return heroi;
}

//Libera a memória alocada para o vetor de bases apontado por base
//Libera a memória alocada para a fila de espera e conjunto de habilidades
//Atribui NULL ao ponteiro
void destroi_bases(base_t **base, int nBases)
{
    if(!base || !*base)
        return;

    for (int i = 0; i < nBases; i++)
    {
        fila_destroi((*base)[i].espera);
        cjto_destroi((*base)[i].presentes);
    }

    free(*base);
    *base = NULL;
    
}

// Inicialização de cada base
// Retorna um ponteiro para o vetor de Bases ou NULL se erro
base_t *inicia_base(int nBases)
{
    base_t *base = malloc(nBases * sizeof(base_t));

    if (!base)
        return NULL;

    for (int i = 0; i < nBases; i++)
    {
        base[i].id = i;
        base[i].lotacao = aleat(3, 10);
        base[i].presentes = cjto_cria(N_HEROIS);
        base[i].estado = ATIVO; // 1 ativo e 0 inativo
        base[i].espera = fila_cria();

        //Se alguma alocação der errado em qualquer momento
        //Destroi todas que já foram alocadas ate ao momento
        //Returna NULL
        if(!base[i].presentes || !base[i].espera)
        {
            cjto_destroi(base[i].presentes);
            fila_destroi(base[i].espera);

            destroi_bases(&base, i);
            return NULL;
            
        }

        base[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        base[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
    }

    return base;
}

//Libera a memória alocada para o vetor de missoes apontado por missao
//Libera a memória alocada para conjunto de habilidades
//Atribui NULL ao ponteiro
void destroi_missoes(missao_t **missao, int nMissoes)
{
    if(!missao || !*missao)
        return;

    for (int i = 0; i < nMissoes; i++)
        cjto_destroi((*missao)[i].habilidades);

    free(*missao);
    *missao = NULL;     

}


// Inicializaçaõ de cada missão
// retorna um ponteiro para o vetor de missões ou NULL se erro
missao_t *inicia_missao(int nMissoes)
{
    missao_t *missao = malloc(nMissoes * sizeof(missao_t));

    if (!missao)
        return NULL;

    for (int i = 0; i < nMissoes; i++)
    {
        missao[i].id = i;

        // Conjunto com tamanho aleatório(6,10)
        // Escolhidas entre as habilidades possiveis
        missao[i].habilidades = cjto_aleat(
            aleat(6, 10),
            N_HABILIDADES
        );

        //Caso a criação do conjunto de errado
        //Libera as memorias que ja foram alocadas e destroi o vetor
        //Retorna NULL
        if(!missao[i].habilidades)
        {
            destroi_missoes(&missao,i);
            return NULL;
        }

        missao[i].perigo = aleat(0, 100);
        missao[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        missao[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
    }

    return missao;
}


//Libera a memória alocada para o vetor de invasoes apontado por invasao
//Atribui NULL ao ponteiro
void destroi_invasaos(invasao_t **invasao)
{
    if(!invasao || !*invasao)
        return;

    free(*invasao);
    *invasao = NULL;
}

// Inicialização de cada invasão
// Retorna um ponteiro para o vetor de invasões ou NULL se erro
invasao_t *inicia_invasao(int nInvasoes)
{
    invasao_t *invasao = malloc(nInvasoes * sizeof(invasao_t));

    if (!invasao)
        return NULL;

    for (int i = 0; i < nInvasoes; i++)
    {
        invasao[i].id = i;
        invasao[i].local = aleat(0, N_BASES - 1);
        invasao[i].pen_base = aleat(0, 7200); // 60 * 24 * 5 (5 dias)
        invasao[i].pen_heroi = aleat(0, 4320); // 60 * 24 * 3 (3 dias)  
    }

    return invasao;    
}



//Destroi o mundo virtual e libera  a memória
//Retorna NULL
void destroi_mundo(mundo_t **mundo)
{
    if(!mundo || !*mundo)
        return ;

    destroi_herois(&(*mundo)->herois, (*mundo)->nHerois);
    destroi_bases(&(*mundo)->bases, (*mundo)->nBases);
    destroi_missoes(&(*mundo)->missoes, (*mundo)->nMissoes);
    destroi_invasaos(&(*mundo)->invasoes);
    free(*mundo);

    *mundo = NULL;

} 


// Inicialização do mundo virtual
// Retorna um ponteiro para o mundo virtual iniciado ou NULL se erro
mundo_t *inicia_mundo()
{
    mundo_t *mundo = malloc(sizeof(mundo_t));

    if (!mundo)
        return NULL;

    memset(mundo, 0, sizeof(mundo_t));

    mundo->nHerois = N_HEROIS;
    mundo->herois = inicia_heroi(mundo->nHerois);

    mundo->nBases = N_BASES;
    mundo->bases = inicia_base(mundo->nBases);

    mundo->nMissoes = N_MISSOES;
    mundo->missoes = inicia_missao(mundo->nMissoes);

    mundo->nInvasoes = N_INVASOES;
    mundo->invasoes = inicia_invasao(mundo->nInvasoes);

    mundo->nHabilidades = N_HABILIDADES;
    mundo->tamanho_mundo = N_TAMANHO_MUNDO;
    mundo->relogio = T_INICIO;

    //Verificando se um dos ponteiros continua nulo
    //Se alguma alocaçaõ der errado, detroi o mundo e retorna NULL
    if(!mundo->herois || !mundo->bases || !mundo->missoes || !mundo->invasoes)
    {
        destroi_mundo(&mundo);
        return NULL;
    }

    return mundo;
}





