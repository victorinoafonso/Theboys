// TAD Eventos
// Implenta os eventos da simulação
// Definição da implementação

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eventos.h"
#include "defines.h"
#include "fila.h"
#include "fprio.h"
#include "entidades.h"
#include "simulacao.h"
#include "conjunto.h"

// Evento que reprenta um heroi chegando em uma base num instante de tempo
void evento_chega(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza od dados necessarios para executar a função
    heroi_t *heroi = item->dados1;
    base_t *base = item->dados2;

    if (!heroi || !base)
        return;

    // Actualiza a base actual do heroi para base em que chegou
    heroi->base_atual = base->id;

    // Calcula quantos herois já estão na base
    int espera, presentes = cjto_card(base->presentes);

    // Analisa se tem espaço na base e se a fila de espera esta vazia
    if (presentes < base->lotacao && !fila_tamanho(base->espera))
    {
        espera = TRUE;
    }

    else
    {
        espera = (heroi->paciencia) > (10 * fila_tamanho(base->espera));
    }

    // Cria novo evento a ser agendado
    dados_t *novo_evento = malloc(sizeof(dados_t));

    if (!novo_evento)
        return;

    novo_evento->dados1 = heroi;
    novo_evento->dados2 = base;

    // Decide se espera ou desiste
    if (espera)
    {
        // Se o agendamento falhar sai da funcao
        if (fprio_insere(lef, novo_evento, ESPERA, mundo->relogio) == ERRO)
        {
            free(novo_evento);
            return;
        }

        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", mundo->relogio, heroi->id, base->id, presentes, base->lotacao);
    }

    else
    {
        // Se o agendamento falhar sai da funcao
        if (fprio_insere(lef, novo_evento, DESISTE, mundo->relogio) == ERRO)
        {
            free(novo_evento);
            return;
        }

        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", mundo->relogio, heroi->id, base->id, presentes, base->lotacao);
    }
}

// Evento que adiciona o heroi na fila de espera da uma base
void evento_espera(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza od dados necessarios para executar a função
    heroi_t *heroi = item->dados1;
    base_t *base = item->dados2;

    if (!heroi || !base)
        return;

    // Cria um ponteiro
    int *id = malloc(sizeof(int));
    if (!id)
        return;

    *id = heroi->id; // Faz ele apontar para o id do heroi

    // Se não conseguir inserir la fila, sai da funcao
    if (fila_insere(base->espera, id) == ERRO)
    {
        free(id);
        return;
    }

    // Cria novo evento a ser agendado
    dados_t *novo_evento = malloc(sizeof(dados_t));

    if (!novo_evento)
        return;

    novo_evento->dados1 = base;
    novo_evento->dados2 = NULL; // Evento avisa só precisa da base

    int presentes = fila_tamanho(base->espera);

    // Atualiza o tamanho máximo da fila
    if (presentes > base->fila_max)
        base->fila_max = presentes;

    // Se o agendamento falhar, sai da funcao
    if (fprio_insere(lef, novo_evento, AVISA, mundo->relogio) == ERRO)
    {
        free(novo_evento);
        return;
    }

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", mundo->relogio, heroi->id, base->id, presentes);
}

// Evento que representa o heroi desistindo de uma base
void evento_desiste(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza os dados necessarios para execução da função
    heroi_t *heroi = item->dados1;
    base_t *base = item->dados2;

    if (!heroi || !base)
        return;

    // Sorteia uma base de destino para a viajem
    base_t *destino = &mundo->bases[aleat(0, mundo->nBases - 1)];

    // Cria novo evento a ser agendado
    dados_t *novo_evento = malloc(sizeof(dados_t));

    if (!novo_evento)
        return;

    novo_evento->dados1 = heroi;
    novo_evento->dados2 = destino;

    // Se o agendamento falhar, sai da funcao
    if (fprio_insere(lef, novo_evento, VIAJA, mundo->relogio) == ERRO)
    {
        free(novo_evento);
        return;
    }

    printf("%6d: DESISTE HEROI %2d BASE %d\n", mundo->relogio, heroi->id, base->id);
}

// Evento que representa o Aviso do porteiro
// Insere os gerois da fila para o conjunto de presentes da base
void evento_avisa(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza os dados necessarios para execução da função
    base_t *base = item->dados1;

    if (!base)
        return;

    int presentes = cjto_card(base->presentes);
    int tamanho_fila = fila_tamanho(base->espera);

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d)", mundo->relogio, base->id, presentes, base->lotacao);

    if (tamanho_fila > 0)
        fila_imprime(base->espera);
    else
        printf("\n");

    while (presentes < base->lotacao && tamanho_fila > 0)
    {
        dados_t *novo_evento = malloc(sizeof(dados_t));

        if (!novo_evento)
            return;

        int *id = fila_retira(base->espera);

        if (!id)
        {
            free(novo_evento);
            return;
        }

        heroi_t *heroi = &mundo->herois[*id];

        novo_evento->dados1 = heroi;
        novo_evento->dados2 = base;

        cjto_insere(base->presentes, heroi->id);

        if (fprio_insere(lef, novo_evento, ENTRA, mundo->relogio) == ERRO)
        {
            cjto_retira(base->presentes, heroi->id);
            fila_insere(base->espera, id);
            free(novo_evento);
            return;
        }

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", mundo->relogio, base->id, heroi->id);

        free(id);

        presentes = cjto_card(base->presentes);
        tamanho_fila = fila_tamanho(base->espera);
    }
}

// Evento que representa o heroi entrando em uma base
void evento_entra(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza os dados necessarios para execução da função
    heroi_t *heroi = item->dados1;
    base_t *base = item->dados2;

    if (!heroi || !base)
        return;

    int presentes = cjto_card(base->presentes);
    int tempo_permanencia = 15 + heroi->paciencia * aleat(1, 20);
    int saida = mundo->relogio + tempo_permanencia;

    dados_t *novo_evento = malloc(sizeof(dados_t));

    if (!novo_evento)
        return;

    novo_evento->dados1 = heroi;
    novo_evento->dados2 = base;

    // Se o agendamento falhar, sai da funcao
    if (fprio_insere(lef, novo_evento, SAI, saida) == ERRO)
    {
        free(novo_evento);
        return;
    }

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%d) SAI %d\n", mundo->relogio, heroi->id, base->id, presentes, base->lotacao, saida);
}

// Evento que representa um heroi saindo de uma base
void evento_sai(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza os dados necessarios para execução da função
    heroi_t *heroi = item->dados1;
    base_t *base = item->dados2;

    if (!heroi || !base)
        return;

    dados_t *novo_viaja = malloc(sizeof(dados_t));
    dados_t *novo_avisa = malloc(sizeof(dados_t));

    // Se pelo menos um falhar sai da função
    if (!novo_viaja || !novo_avisa)
    {
        free(novo_viaja);
        free(novo_avisa);
        return;
    }

    // Dados do evento VIAJA
    novo_viaja->dados1 = heroi;
    novo_viaja->dados2 = &mundo->bases[aleat(0, mundo->nBases - 1)];

    // Dados do evento AVISA
    novo_avisa->dados1 = base;
    novo_avisa->dados2 = NULL; // Avisa só precisa da base

    // Retira o herói da base
    cjto_retira(base->presentes, heroi->id);

    // Se o agendamento falhar, insere o heroi de volta na base
    if (fprio_insere(lef, novo_viaja, VIAJA, mundo->relogio) == ERRO)
    {
        cjto_insere(base->presentes, heroi->id);
        free(novo_viaja);
        free(novo_avisa);
        return;
    }

    // Se o agendamento falhar, sai da funcao
    if (fprio_insere(lef, novo_avisa, AVISA, mundo->relogio) == ERRO)
    {
        free(novo_avisa);
        return;
    }

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", mundo->relogio, heroi->id, base->id, cjto_card(base->presentes), base->lotacao);
}

// Evento que representa o heroi viajando
void evento_viaja(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza os dados necessarios para execução da função
    heroi_t *heroi = item->dados1;
    base_t *destino = item->dados2;

    if (!heroi || !destino)
        return;

    if (heroi->base_atual < 0 ||
        heroi->base_atual >= mundo->nBases)
    {
        fprintf(stderr,
                "ERRO: HEROI %d, BASE_ATUAL %d, TEMPO %d\n",
                heroi->id, heroi->base_atual, mundo->relogio);
        abort();
    }
    // Recebe a base de origem do heroi
    base_t *origem = &mundo->bases[heroi->base_atual];

    // Diferença entre as coordenadas das bases
    int dx = destino->local.x - origem->local.x;
    int dy = destino->local.y - origem->local.y;

    // Distância cartesiana
    int distancia = sqrt(dx * dx + dy * dy);

    // Duração da viagem em minutos
    int duracao = distancia / heroi->velocidade;

    int chegada = mundo->relogio + duracao;

    dados_t *novo_evento = malloc(sizeof(dados_t));

    if (!novo_evento)
        return;

    novo_evento->dados1 = heroi;
    novo_evento->dados2 = destino;

    if (fprio_insere(lef, novo_evento, CHEGA, chegada) == ERRO)
    {
        free(novo_evento);
        return;
    }

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", mundo->relogio, heroi->id, origem->id, destino->id, distancia, heroi->velocidade, chegada);
}

// Evento que representa um heroi morrendo
void evento_morre(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    // Actualiza os dados necessarios para execução da função
    heroi_t *heroi = item->dados1;
    missao_t *missao = item->dados2;

    if (!heroi || !missao)
        return;

    // Aloca o evento AVISA
    dados_t *novo_evento = malloc(sizeof(dados_t));
    if (!novo_evento)
        return;

    // Cria uma lef auxiliar
    struct fprio_t *aux = fprio_cria();
    if (!aux)
    {
        free(novo_evento);
        return;
    }

    base_t *base = &mundo->bases[heroi->base_atual];

    // Retira o herói da base
    cjto_retira(base->presentes, heroi->id);

    // Marca o herói como morto
    heroi->estado = MORTO;

    // Remove todos os eventos futuros do herói da lef
    int tipo, tempo;
    dados_t *evento;

    while ((evento = fprio_retira(lef, &tipo, &tempo)))
    {
        // Compara os eventos relacionados ao heroi morto
        if ((tipo == CHEGA || tipo == ESPERA || tipo == DESISTE || tipo == VIAJA || tipo == ENTRA || tipo == SAI || tipo == MORRE) && evento->dados1 == heroi)
        {
            free(evento);
        }
        else
        {
            fprio_insere(aux, evento, tipo, tempo);
        }
    }

    // Faz a Lef receber os eventos futuros retirando
    // as do heroi morto
    fprio_troca(lef, aux);
    fprio_destroi(aux);

    novo_evento->dados1 = base;
    novo_evento->dados2 = NULL; // Avisa só precisa da base

    if (fprio_insere(lef, novo_evento, AVISA, mundo->relogio) == ERRO)
    {
        free(novo_evento);
        return;
    }

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", mundo->relogio, heroi->id, missao->id);
}

// Verifica se uma base possui as habilidades necessárias
// Retorna 1 se apta e 0 se não apta
int base_apta(mundo_t *mundo, base_t *base, missao_t *missao)
{
    if (!mundo || !base || !missao || base->estado != ATIVO)
        return 0;

    struct cjto_t *habilidades = cjto_cria(missao->habilidades->cap);

    if (!habilidades)
        return 0;

    // Percorre todos os heróis do mundo
    for (int i = 0; i < mundo->nHerois; i++)
    {
        heroi_t *heroi = &mundo->herois[i];

        // Só considera os heróis ativos e presentes na base
        if (heroi->estado == ATIVO && cjto_pertence(base->presentes, heroi->id) == 1)
        {
            struct cjto_t *uniao = cjto_uniao(habilidades, heroi->habilidades);

            if (!uniao)
            {
                cjto_destroi(habilidades);
                return 0;
            }

            cjto_destroi(habilidades);
            habilidades = uniao;
        }
    }

    // Verifica se a equipe possui as habilidades da missão
    int resultado = cjto_contem(habilidades, missao->habilidades);

    cjto_destroi(habilidades);

    return resultado == 1;
}

// Procura a base apta mais próxima da missão
base_t *escolhe_base(mundo_t *mundo, missao_t *missao)
{
    if (!mundo || !missao)
        return NULL;

    base_t *melhor_base = NULL;
    int menor_distancia = 0;

    // Percorre todas a bases vendo qual a melhor de todas aptas
    for (int i = 0; i < mundo->nBases; i++)
    {
        base_t *base = &mundo->bases[i];

        // Verifica se a base pode cumprir a missão
        if (base_apta(mundo, base, missao))
        {
            int dx = base->local.x - missao->local.x;
            int dy = base->local.y - missao->local.y;

            int distancia = dx * dx + dy * dy;

            // Guarda a base apta mais próxima
            if (melhor_base == NULL || distancia < menor_distancia)
            {
                melhor_base = base;
                menor_distancia = distancia;
            }
        }
    }

    return melhor_base;
}

// Processa os heróis de uma base que cumpre a missão
void cumpre_missao(struct fprio_t *lef, mundo_t *mundo, base_t *base, missao_t *missao)
{
    if (!lef || !mundo || !base || !missao)
        return;

    // Percorre todos os heróis do mundo
    for (int i = 0; i < mundo->nHerois; i++)
    {
        heroi_t *heroi = &mundo->herois[i];

        // Considera apenas heróis ativos e presentes na base
        if (heroi->estado == ATIVO && cjto_pertence(base->presentes, heroi->id) == 1)
        {
            // Calcula o risco de morte do herói
            double risco = missao->perigo / (heroi->paciencia + heroi->experiencia + 1.0);

            if (risco > aleat(0, 30))
            {
                // Cria o evento MORRE
                dados_t *novo_evento = malloc(sizeof(dados_t));

                if (!novo_evento)
                    return;

                novo_evento->dados1 = heroi;
                novo_evento->dados2 = missao;

                if (fprio_insere(lef, novo_evento, MORRE, mundo->relogio) == ERRO)
                {
                    free(novo_evento);
                    return;
                }
            }
            else
            {
                heroi->experiencia++;
            }
        }
    }
}

// Evento que representa a missao de cada heroi
void evento_missao(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    missao_t *missao = item->dados1;

    if (!missao)
        return;

    // Incrementa o número de tentativas
    missao->tentativas++;

    // Imprime as habilidades exigidas pela missão
    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", mundo->relogio, missao->id, missao->tentativas);

    cjto_imprime(missao->habilidades);
    printf(" ]\n");

    // Procura a base apta mais próxima
    base_t *base = escolhe_base(mundo, missao);

    if (base)
    {
        // Cria um conjunto para reunir as habilidades da equipe
        struct cjto_t *habilidades = cjto_cria(missao->habilidades->cap);

        if (!habilidades)
            return;

        // Percorre os heróis presentes na base
        for (int i = 0; i < mundo->nHerois; i++)
        {
            heroi_t *heroi = &mundo->herois[i];

            if (heroi->estado == ATIVO && cjto_pertence(base->presentes, heroi->id) == 1)
            {
                struct cjto_t *uniao = cjto_uniao(habilidades, heroi->habilidades);

                if (!uniao)
                {
                    cjto_destroi(habilidades);
                    return;
                }

                cjto_destroi(habilidades);
                habilidades = uniao;
            }
        }

        // Marca a missão como cumprida
        missao->cumprida = 1;

        // Incrementa o número de missões cumpridas pela base
        base->missoes_cumpridas++;

        // Imprime a base escolhida e as habilidades da equipe
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", mundo->relogio, missao->id, base->id);

        cjto_imprime(habilidades);
        printf(" ]\n");

        // Libera o conjunto auxiliar
        cjto_destroi(habilidades);

        // Processa os heróis da missão
        cumpre_missao(lef, mundo, base, missao);
    }

    else
    {

        printf("%6d: MISSAO %d IMPOSSIVEL\n", mundo->relogio, missao->id);

        // Cria um novo evento MISSAO
        dados_t *novo_evento = malloc(sizeof(dados_t));

        if (!novo_evento)
            return;

        novo_evento->dados1 = missao;
        novo_evento->dados2 = NULL;

        // Reagenda a missão para 24 horas depois
        int tempo = mundo->relogio + 24 * 60; // dia seguinte

        if (fprio_insere(lef, novo_evento, MISSAO, tempo) == ERRO)
        {
            free(novo_evento);
            return;
        }
    }
}

// Remove da LEF todos os eventos SAI pendentes dos heróis presentes na base.
// Retorna 1 em caso de sucesso ou ERRO (-1) se falhar a alocação/inserção.
static int remove_saidas_base(struct fprio_t *lef, base_t *base)
{
    struct fprio_t *aux = fprio_cria();
    if (!aux)
        return ERRO;

    int tipo, tempo;
    dados_t *evento;

    while ((evento = fprio_retira(lef, &tipo, &tempo)))
    {
        int remover = 0;

        // Remove qualquer SAI pendente (inclusive no mesmo instante) dos heróis na base
        if (tipo == SAI)
        {
            heroi_t *heroi = evento->dados1;

            if (heroi && cjto_pertence(base->presentes, heroi->id) == 1)
                remover = 1;
        }

        if (remover)
        {
            free(evento);
        }
        else
        {
            if (fprio_insere(aux, evento, tipo, tempo) == ERRO)
            {
                free(evento);
                fprio_troca(lef, aux);
                fprio_destroi(aux);
                return ERRO;
            }
        }
    }

    // Devolve os eventos restantes à LEF em O(1)
    fprio_troca(lef, aux);
    fprio_destroi(aux);

    return 1;
}

// Processa os heróis presentes na base
static int processa_herois_base(struct fprio_t *lef, mundo_t *mundo, base_t *base, invasao_t *invasao)
{
    int primeiro = 1;

    for (int i = 0; i < mundo->nHerois; i++)
    {
        heroi_t *heroi = &mundo->herois[i];

        if (cjto_pertence(base->presentes, heroi->id) == 1)
        {
            if (!primeiro)
                printf(", ");

            printf("%d", heroi->id);
            primeiro = 0;

            // Torna o herói inativo
            heroi->estado = INATIVO;

            // Agenda a recuperação do herói
            dados_t *recupera = malloc(sizeof(dados_t));
            if (!recupera)
                return ERRO;

            recupera->dados1 = heroi;
            recupera->dados2 = NULL;

            int tempo = mundo->relogio + invasao->pen_heroi;
            if (fprio_insere(lef, recupera, REC_HEROI, tempo) == ERRO)
            {
                free(recupera);
                return ERRO;
            }

            // Agenda a saída imediata do herói
            dados_t *saida = malloc(sizeof(dados_t));
            if (!saida)
                return ERRO;

            saida->dados1 = heroi;
            saida->dados2 = base;

            if (fprio_insere(lef, saida, SAI, mundo->relogio) == ERRO)
            {
                free(saida);
                return ERRO;
            }
        }
    }
    return 1;
}

// Evento que representa a invasao em uma base
void evento_invade(struct fprio_t *lef, mundo_t *mundo, dados_t *item)
{
    if (!lef || !mundo || !item)
        return;

    invasao_t *invasao = item->dados1;
    if (!invasao)
        return;

    base_t *base = &mundo->bases[invasao->local];

    printf("%6d: INVADE   BASE %d   INVADE %d\n", mundo->relogio, base->id, invasao->id);

    // Se a base já está inativa, ignora a invasão
    if (base->estado != ATIVO)
        return;

    // Remove os eventos SAI pendentes dos heróis presentes na base
    if (remove_saidas_base(lef, base) == ERRO)
        return;

    printf("%6d: INVADE %d BASE %d HEROIS INATIVOS [",
           mundo->relogio, invasao->id, base->id);

    // Processa os heróis presentes na base
    if (processa_herois_base(lef, mundo, base, invasao) == ERRO)
        return;

    printf("]\n");

    // Torna a base inativa
    base->estado = INATIVO;

    // Agenda a reconstrução da base
    dados_t *reconstroi = malloc(sizeof(dados_t));
    if (!reconstroi)
        return;

    reconstroi->dados1 = base;
    reconstroi->dados2 = NULL;

    int tempo_base = mundo->relogio + invasao->pen_base;
    if (fprio_insere(lef, reconstroi, REC_BASE, tempo_base) == ERRO)
    {
        free(reconstroi);
        return;
    }

    // Incrementa o número de invasões realizadas
    mundo->invasoes_realizadas++;

    printf("%6d: INVADE %d BASE %d INATIVA\n", mundo->relogio, invasao->id, base->id);
}

// Evento que representa recuperacao do heroi
void evento_recupera_heroi(dados_t *item)
{
    if (!item)
        return;

    heroi_t *heroi = item->dados1;

    if (!heroi)
        return;

    heroi->estado = ATIVO;
}

// Evento que representa recuperacao a base
void evento_reconstroi_base(dados_t *item)
{
    if (!item)
        return;

    base_t *base = item->dados1;

    if (!base)
        return;

    base->estado = ATIVO;
}

// Representa o final do mundo da simulação
void evento_fim(mundo_t *mundo)
{
    if (!mundo)
        return;

    printf("%6d: FIM\n\n", mundo->relogio);

    int mortos = 0;

    // Estatísticas dos heróis
    for (int i = 0; i < mundo->nHerois; i++)
    {
        heroi_t *heroi = &mundo->herois[i];

        if (heroi->estado == MORTO)
            mortos++;

        printf("HEROI %2d %s PAC %3d VEL %4d EXP %4d HABS [ ",

               heroi->id,
               heroi->estado == MORTO ? "MORTO" : "VIVO",
               heroi->paciencia,
               heroi->velocidade,
               heroi->experiencia);

        cjto_imprime(heroi->habilidades);
        printf(" ]\n");
    }

    printf("\n");

    // Estatísticas das bases
    for (int i = 0; i < mundo->nBases; i++)
    {
        base_t *base = &mundo->bases[i];

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", base->id, base->lotacao, base->fila_max, base->missoes_cumpridas);
    }

    printf("\n");

    // Estatísticas das missões
    int cumpridas = 0;
    int min_tentativas = 0;
    int max_tentativas = 0;
    int total_tentativas = 0;

    for (int i = 0; i < mundo->nMissoes; i++)
    {
        missao_t *missao = &mundo->missoes[i];

        if (missao->cumprida)
            cumpridas++;

        if (i == 0 || missao->tentativas < min_tentativas)
            min_tentativas = missao->tentativas;

        if (missao->tentativas > max_tentativas)
            max_tentativas = missao->tentativas;

        total_tentativas += missao->tentativas;
    }

    double taxa_missoes = 0;
    double media_tentativas = 0;
    double taxa_invasoes = 0;
    double taxa_mortalidade = 0;

    if (mundo->nMissoes > 0)
    {
        taxa_missoes = 100.0 * cumpridas / mundo->nMissoes;
        media_tentativas = (double)total_tentativas / mundo->nMissoes;
    }

    if (mundo->nInvasoes > 0)
        taxa_invasoes =
            100.0 * mundo->invasoes_realizadas / mundo->nInvasoes;

    if (mundo->nHerois > 0)
        taxa_mortalidade = 100.0 * mortos / mundo->nHerois;

    printf("EVENTOS TRATADOS: %d\n", mundo->eventos_tratados);

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n",
           cumpridas, mundo->nMissoes, taxa_missoes);

    printf("INVASÕES REALIZADAS: %d/%d (%.1f%%)\n",
           mundo->invasoes_realizadas,
           mundo->nInvasoes,
           taxa_invasoes);

    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n",
           min_tentativas, max_tentativas, media_tentativas);

    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mortalidade);
}
