# The Boys — Simulador de Eventos Discretos

Projeto final da disciplina **Programação 1 (CI1001)**, do curso de Ciência da Computação da Universidade Federal do Paraná (UFPR).

## Descrição

O projeto consiste na implementação, em linguagem C, de um simulador de eventos discretos inspirado na série *The Boys*.

A simulação representa um mundo composto por heróis, bases, missões e invasões. Os heróis possuem características próprias, como habilidades, velocidade, paciência e experiência, e deslocam-se entre bases para participar de missões.

O objetivo é simular o funcionamento desse mundo durante **525.600 minutos (365 dias)**, processando os acontecimentos em ordem cronológica.

## Funcionamento

O simulador utiliza uma **Lista de Eventos Futuros (LEF)**, implementada através de uma fila de prioridades.

Cada evento possui um instante de execução e é processado conforme a ordem temporal estabelecida pela LEF.

Os principais eventos são:

- **CHEGA:** chegada de um herói a uma base.
- **ESPERA:** entrada do herói na fila de espera.
- **DESISTE:** desistência de entrar numa base.
- **AVISA:** gerenciamento da fila e da lotação da base.
- **ENTRA:** entrada de um herói na base.
- **SAI:** saída de um herói da base.
- **VIAJA:** deslocamento entre bases.
- **MORRE:** morte de um herói durante uma missão.
- **MISSAO:** tentativa de realização de uma missão.
- **INVADE:** invasão de uma base.
- **RECUPERA_HEROI:** recuperação de um herói debilitado.
- **RECONSTROI_BASE:** reativação de uma base.
- **FIM:** encerramento da simulação e apresentação das estatísticas.

As missões são realizadas por equipes cujas habilidades, em conjunto, atendam aos requisitos exigidos. Missões impossíveis são reagendadas para o dia seguinte.

## Organização do projeto

O código está dividido em módulos para facilitar a implementação e a manutenção:

| Arquivo | Responsabilidade |
|---|---|
| `theboys.c` | Função principal e inicialização do programa |
| `simulacao.c` | Controle da simulação e processamento dos eventos |
| `eventos.c` | Implementação dos eventos |
| `entidades.c` | Criação e destruição das entidades do mundo |
| `fprio.c` | Implementação da fila de prioridades |
| `fila.c` | Implementação da fila de espera |
| `conjunto.h` e `conjunto.o` | Interface e implementação fornecida do TAD Conjunto |
| `Makefile` | Compilação do projeto |

Os respectivos arquivos de cabeçalho (`.h`) definem as estruturas e interfaces utilizadas pelos módulos.

## Compilação

O projeto foi desenvolvido em C, utilizando o padrão **C99**.

### Requisitos

- Compilador GCC
- Make
- Ambiente Linux

Para compilar, abra um terminal no diretório do projeto e execute:

`make`

A compilação utiliza as opções:

`-Wall -Wextra -Werror -std=c99 -g`

O executável gerado é denominado `theboys`.

## Execução

Para executar o simulador:

`./theboys`

Como a simulação produz uma grande quantidade de mensagens, recomenda-se redirecionar a saída para um arquivo:

`./theboys > saida.txt`

Para consultar apenas as últimas linhas, que contêm as estatísticas finais:

`tail -n 20 saida.txt`

Para remover os arquivos gerados pela compilação:

`make clean`

## Resultados

Durante a execução, o programa apresenta informações sobre os eventos processados.

Ao final, são exibidas estatísticas como:

- Estado, paciência, velocidade, experiência e habilidades dos heróis.
- Lotação, tamanho máximo da fila e missões cumpridas por cada base.
- Quantidade total de eventos processados.
- Número e percentual de missões cumpridas.
- Quantidade de invasões realizadas.
- Número mínimo, máximo e médio de tentativas por missão.
- Taxa de mortalidade dos heróis.

## Observações

- A simulação utiliza `srand(0)` para permitir a reprodução da sequência de números pseudoaleatórios.
- O tempo de simulação é diferente do tempo real de execução.
- O grande volume de eventos pode resultar em um tempo de processamento elevado.
- O gerenciamento das estruturas de dados e da memória é realizado manualmente, conforme as características da linguagem C.

## Referência

Projeto Final — *The Boys*, disciplina CI1001 — Programação 1, Universidade Federal do Paraná (UFPR).
