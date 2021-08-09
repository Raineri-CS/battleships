#include "battleship.h"

int gameLoop(char *domain, unsigned short int port, unsigned char gameMode) {
  int clientSockfd, didRead;
  struct sockaddr_in serverAddr;
  char recvBuffer[1024], sendBuffer[8];

  /* Inicializacoes */
  clientSockfd = 0;
  didRead = 0;
  recvBuffer[0] = '\0';
  sendBuffer[0] = '\0';

  /* Protocolo TCP */
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);

  /* Abre a socket do cliente no modo tcp do tipo STREAM */
  if ((clientSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "\n Errro ao criar a socket! \n");
    return -1;
  }

  if (inet_pton(AF_INET, domain, &serverAddr.sin_addr) <= 0) {
    fprintf(stderr, "\n Endereco invalido ou nao suportado pelo AF_INET \n");
    return -1;
  }

  if (connect(clientSockfd, (struct sockaddr *)&serverAddr,
              sizeof(serverAddr)) < 0) {
    fprintf(stderr, "\n Conexao falhou! \n");
    return -1;
  }

  /*
    TODO
    Preprocessamento (jogo)
      -> Tentar abrir arquivo de predefinicao de tabuleiro
      ->
      se DESCRITOR_TABULEIRO entao
        se VERIFICAR_INTEGRIDADE(DESCRITOR_TABULEIRO) entao
          CONSTRUIR_MATRIZ(DESCRITOR_TABULEIRO)
        senao
          RETORNE -1
      senao
        RANDOMIZA_ESTRUTURAS
      -> VERIFICAR_INTEGRIDADE devera retornar 0 ou -1, e devera verificar se os
    tipos procedem no arquivo
      -> O input sera dado no arquivo como uma chamada de funcao em c
      -> S(PONTO_INICIAL, MODO, ORIENTACAO)
         T(PONTO_INICIAL, MODO, ORIENTACAO)
         N(PONTO_INICIAL, MODO, ORIENTACAO)
         P(PONTO_INICIAL, MODO, ORIENTACAO)

      -> Sendo PONTO_INICIAL um ponto no tabuleiro, por exemplo a1
      -> MODO sendo 0 para VERTICAL, e 1 para HORIZONTAL
      -> ORIENTACAO sendo 0 ESQUERDA/CIMA e 1 DIREITA/BAIXO dependendo do MODO
  */

  /* Finalmente, o loop do jogo */
  for (;;) {
    /*
      TODO
      O loop consiste em:
        -> inicioJogo:
        -> Receber o movimento do server
        -> Enviar o acerto/erro
        -> Representar o tabuleiro atual graficamente
        -> Esperar o comando do cliente
        -> Esperar resposta do servidor
        -> se TODDAS_AS_ESTRUTURAS == 0 entao Enviar Venceu!
        -> se Venceu! entao feche a conexao e retorne 0
        -> goto inicioJogo (inicio do for)
    */
  }
  /* Se por algum motivo ele chegar aqui, deu ruim em algum lugar */
  return -1;
}

void init(tabuleiro *tab) {
  for (unsigned int i = 0; i < FIELD_SIZE; i++)
    for (int j = 0; j < FIELD_SIZE; j++) {
      tab->field[i][j].isOccupied = 0;
      tab->field[i][j].type = WAT;
    }
}

int addToField(unsigned int x1, unsigned int y1, unsigned int x2,
               unsigned int y2, unsigned char type, tabuleiro *tab) {
  // Tratando erros de preprocessamento
  if ((x1 > 14 || y1 > 14) || (x2 > 14 || y2 > 14)) {
    fprintf(stderr, "Erro ao inserir a peca, fora dos indices do tabuleiro!\n");
    return -1;
  }

  switch (type) {
  case SUB:
    // Verifica qual dos eixos se diferenciou
    if (x1 == x2) {
      // Verifica se eh do tamanho correto
      // abs te retorna o valor sem o bit de sinal da subtracao
      // esta sendo castado pra int dentro da funcao pra sumir com um warning de
      // que os parametros sao unsigned int, logo nao tem efeito a chamada
      if (abs((int)(y2 - y1)) + 1 != 2) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = y1; i <= y2; i++) {
        // Verifica se o espaco ja esta ocupado antes de tentar inserir
        if (tab->field[x1][i].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!",
                  x1, i);
          return -1;
        }
        tab->field[x1][i].isOccupied = 1;
        tab->field[x1][i].type = SUB;
      }
    } else if (y1 == y2) {
      // Tem o caso de estar erradas as coordenadas, nao se pode ter pecas na
      // diagonal
      if (abs((int)(x2 - x1)) + 1 != 2) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = x1; i <= x2; i++) {
        if (tab->field[i][y1].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!", i,
                  y1);
          return -1;
        }
        tab->field[i][y1].isOccupied = 1;
        tab->field[i][y1].type = SUB;
      }
    }
    break;
  case TOR:
    if (x1 == x2) {
      if (abs((int)(y2 - y1)) + 1 != 3) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = y1; i <= y2; i++) {
        if (tab->field[x1][i].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!",
                  x1, i);
          return -1;
        }
        tab->field[x1][i].isOccupied = 1;
        tab->field[x1][i].type = TOR;
      }
    } else if (y1 == y2) {
      if (abs((int)(x2 - x1)) + 1 != 3) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = x1; i <= x2; i++) {
        if (tab->field[i][y1].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!", i,
                  y1);
          return -1;
        }
        tab->field[i][y1].isOccupied = 1;
        tab->field[i][y1].type = TOR;
      }
    }
    break;
  case TAS:
    if (x1 == x2) {
      if (abs((int)(y2 - y1)) + 1 != 4) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = y1; i <= y2; i++) {
        if (tab->field[x1][i].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!",
                  x1, i);
          return -1;
        }
        tab->field[x1][i].isOccupied = 1;
        tab->field[x1][i].type = TAS;
      }
    } else if (y1 == y2) {
      if (abs((int)(x2 - x1)) + 1 != 4) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = x1; i <= x2; i++) {
        if (tab->field[i][y1].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!", i,
                  y1);
          return -1;
        }
        tab->field[i][y1].isOccupied = 1;
        tab->field[i][y1].type = TAS;
      }
    }
    break;
  case AIP:
    if (x1 == x2) {
      if (abs((int)(y2 - y1)) + 1 != 5) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = y1; i <= y2; i++) {
        if (tab->field[x1][i].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!",
                  x1, i);
          return -1;
        }
        tab->field[x1][i].isOccupied = 1;
        tab->field[x1][i].type = AIP;
      }
    } else if (y1 == y2) {
      if (abs((int)(x2 - x1)) + 1 != 5) {
        fprintf(
            stderr,
            "Erro ao inserir a peca, o tamanho nao condiz com a categoria\n");
        return -1;
      }
      for (unsigned int i = x1; i <= x2; i++) {
        if (tab->field[i][y1].isOccupied) {
          fprintf(stderr, "Erro ao inserir peca, espaco (%d,%d) ja ocupado!", i,
                  y1);
          return -1;
        }
        tab->field[i][y1].isOccupied = 1;
        tab->field[i][y1].type = AIP;
      }
    }
    break;
  default:
    break;
  }
  return 0;
}

int verifyFileIntegrity(FILE *fd, tabuleiro *tab) {
  int tempX1, tempY1, tempX2, tempY2, subQty, torQty, tasQty, aipQty;
  char pieceBuffer[32];

  // Temporary coordinates
  tempX1 = 0;
  tempY1 = 0;
  tempX2 = 0;
  tempY2 = 0;

  // Used to check if the correct number of units is inputtted by the end of the
  // file
  subQty = 0;
  torQty = 0;
  tasQty = 0;
  aipQty = 0;

  while (fscanf(fd, "%s %d %d %d %d", pieceBuffer, &tempX1, &tempY1, &tempX2,
                &tempY2) != EOF) {
    /* Tem que ser nested ifs porque o switch-case nao gosta de multiplos
     * caracteres */
    if (strncmp(pieceBuffer, "SUB", 3) == 0) {
      if (addToField(tempX1, tempY1, tempX2, tempY2, SUB, tab) == -1)
        return -1;
      subQty++;
      if (subQty > 5) {
        fprintf(stderr, "A quantidade de SUBMARINOS excedeu 5!\n");
        return -1;
      }
    } else if (strncmp(pieceBuffer, "TOR", 3) == 0) {
      if (addToField(tempX1, tempY1, tempX2, tempY2, TOR, tab))
        return -1;
      torQty++;
      if (torQty > 3) {
        fprintf(stderr, "A quantidade de TORPEDEIROS excedeu 3!\n");
        return -1;
      }
    } else if (strncmp(pieceBuffer, "TAS", 3) == 0) {
      if (addToField(tempX1, tempY1, tempX2, tempY2, TAS, tab))
        return -1;
      tasQty++;
      if (tasQty > 2) {
        fprintf(stderr, "A quantidade de NAVIO-TANQUE excedeu 2!\n");
        return -1;
      }
    } else if (strncmp(pieceBuffer, "AIP", 3) == 0) {
      if (addToField(tempX1, tempY1, tempX2, tempY2, AIP, tab))
        return -1;
      aipQty++;
      if (aipQty > 1) {
        fprintf(stderr, "A quantidade de PORTA-AVIOES excedeu 1!\n");
        return -1;
      }
    }
  }

  // Verify if all the pieces are present
  if (subQty != 5 || torQty != 3 || tasQty != 2 || aipQty != 1) {
    fprintf(stderr, "Pecas insuficientes para completar um tabuleiro!");
    return -1;
  }

  return 0;
}

void printField(tabuleiro *tab) {
  printf("\t1  2  3  4  5  6  7  8  9  10 11 12 13 14 "
         "15\n\t┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐\n");
  for (unsigned int i = 0; i < 15; i++) {
    // Letras do lado do tabuleiro
    printf("%c \t│", 97 + i);
    for (unsigned int j = 0; j < 15; j++) {
      switch (tab->field[j][i].type) {
      case SUB:
        printf("S │");
        break;
      case TOR:
        printf("T │");
        break;
      case TAS:
        printf("N │");
        break;
      case AIP:
        printf("P │");
        break;
      default:
        printf("  │");
        break;
      }
    }
    if(i+1 == 15){
      printf("\n");
      break;
    }
    printf("\n\t├──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┤\n");
  }
  printf("\t└──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘\n\n");
}
