// TAD Simulaçaõ
// Implenta o estado inicial da SED (simulação de eventos discretos)
// Implementa o loop principal da simulalação
// Definição da implemeentaçaõ

#include <stdlib.h>
#include "simulacao.h"
#include "fprio.h"
#include "entidades.h"
#include "defines.h"
#include "eventos.h"

// Insere na LEF os eventos iniciais da simulação
// Retorna -1 se erro ou 1 se sucesso
int eventos_iniciais(struct fprio_t *lef, mundo_t *mundo)
{
    if (!lef || !mundo)
        return ERRO;

    // Cada heroi chegará em uma base aleatória
    // A chegada é agenda em momento dentro dos 3 primeiros dias
    for (int i = 0; i < mundo->nHerois; i++)
    {

        dados_t *item = malloc(sizeof(dados_t));

        if (!item)
            return ERRO;

        item->dados1 = &mundo->herois[i];                    // Recebe o endereço do heroi
        item->dados2 = &mundo->bases[aleat(0, N_BASES - 1)]; // Recebe o endereço da base sorteiada

        int tempo = aleat(0, 4320); // 60 * 24 * 3 (3 dias)

        // Se o agendamento falhar informa erro
        if (fprio_insere(lef, item, CHEGA, tempo) == ERRO)
        {
            free(item);
            return ERRO;
        }
    }

    // Cada missão será agendada para alguem momento da simulação
    for (int i = 0; i < mundo->nMissoes; i++)
    {
        dados_t *item = malloc(sizeof(dados_t));

        if (!item)
            return ERRO;

        item->dados1 = &mundo->missoes[i]; // Recebe  o endereço da missão
        item->dados2 = NULL;               // Não será usado por isso recebe NULL

        int tempo = aleat(0, T_FIM_DO_MUNDO);

        // Se o agendamento falhar informa erro
        if (fprio_insere(lef, item, MISSAO, tempo) == ERRO)
        {
            free(item);
            return ERRO;
        }
    }

    // Cada invasão é agendada
    // Acontece a partir dos tres primeiros dias da simulação
    for (int i = 0; i < mundo->nInvasoes; i++)
    {
        dados_t *item = malloc(sizeof(dados_t));

        if (!item)
            return ERRO;

        item->dados1 = &mundo->invasoes[i]; // Recebe o endereço da invasão
        item->dados2 = NULL;                // não será usado por isso recebe NULL

        int tempo = aleat(4321, T_FIM_DO_MUNDO); // 4321 = (60 * 24 * 3) + 1

        // Se o agendamento falhar informa erro
        if (fprio_insere(lef, item, INVADE, tempo) == ERRO)
        {
            free(item);
            return ERRO;
        }
    }

    // O evento FIM vai ser agendado no instante final da simulação
    dados_t *item = malloc(sizeof(dados_t));

    if (!item)
        return ERRO;

    item->dados1 = NULL; // Não será usado por isso recebe NULL
    item->dados2 = NULL; // Não será usado por isso recebe NULL

    int tempo = T_FIM_DO_MUNDO;

    // Se o agendamento falhar informa erro
    if (fprio_insere(lef, item, FIM, tempo) == ERRO)
    {
        free(item);
        return ERRO;
    }

    return SUCESSO;
}

// Laço principal da Simulação
void simulacao(struct fprio_t *lef, mundo_t *mundo)
{

    if (!lef || !mundo)
        return;

    int evento, tempo;
    dados_t *item;

    do
    {
        item = (dados_t *)fprio_retira(lef, &evento, &tempo);

        if(!item)
            return;

        mundo->relogio = tempo;

        mundo->eventos_tratados++;
        

        switch (evento)
        {
            case CHEGA:

                evento_chega(lef, mundo, item);
                break;

            case ESPERA:
            
                evento_espera(lef, mundo, item);
                break;
            
            case DESISTE:

                evento_desiste(lef, mundo, item);
                break;

            case AVISA:

                evento_avisa(lef, mundo, item);        
                break;

            case ENTRA:

                evento_entra(lef, mundo, item);
                break;
            
            case SAI:

                evento_sai(lef, mundo, item);
                break;

            case VIAJA:

                evento_viaja(lef, mundo, item);       
                break;

            case MORRE:

                evento_morre(lef, mundo, item);
                break;
            
            case MISSAO:

                evento_missao(lef, mundo, item);
                break;
            
            case INVADE:

                evento_invade(lef, mundo, item);          
                break;

            case REC_HEROI:

                evento_recupera_heroi( item);
                break;
            
            case REC_BASE:

                evento_reconstroi_base(item);
                break;

            case FIM:

                evento_fim(mundo);
                break;

            default:
                free(item);
                return;

        }

        
        free(item);

    } while (evento != FIM);
}

//Ao terminar a simulação
//Destroi a LEF e os items nela
void destroi_simulacao(struct fprio_t *lef)
{
    if(!lef)
        return;

    fprio_destroi(lef);
}
