#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

int main()
{
  int tipo, prio;

  struct fprio_t *fila = fprio_cria();

  if (!fila)
    return 1;

  for (int i = 0; i < 4; i++)
  {
    char *item_t = malloc(sizeof(char));

    if (!item_t)
      break;

    if (scanf(" %c %d %d", item_t, &tipo, &prio) != 3)
    {
      free(item_t);
      break;
    }

    if (fprio_insere(fila, item_t, tipo, prio) == -1)
    {
      free(item_t);
      break;
    }
  }

  fprio_imprime(fila);
  printf("\n");

  fprio_destroi(fila);

  return 0;
}