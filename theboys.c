// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui

#include <stdlib.h>
#include <stdio.h>
#include "entidades.h"
#include "simulacao.h"
#include "fprio.h"

// minimize o uso de variáveis globais

// programa principal

int main(void)
{
  srand(0);

  mundo_t *mundo = inicia_mundo();

  if (!mundo)
  {
    fprintf(stderr, "Erro ao criar o mundo\n");

    return 1;
  }

  struct fprio_t *lef = fprio_cria();

  if (!lef)
  {
    fprintf(stderr, "Erro ao criar a LEF\n");

    destroi_mundo(&mundo);
    
    return 1;
  }

  eventos_iniciais(lef, mundo);

  simulacao(lef, mundo);

  destroi_simulacao(lef);

  destroi_mundo(&mundo);

  return 0;
}
