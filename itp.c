#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHAS 26
#define MAX_COLUNAS 99
#define ANSI_COLOR_RED "\x1b[31m" // cores em ANSI utilizadas
#define ANSI_COLOR_GRAY "\e[0;37m"
#define ANSI_COLOR_BLUE "\e[0;34m"
#define ANSI_COLOR_RESET "\x1b[0m"


// Definição da struct Assento
typedef struct Assento {
  int reservado;
  int fileira;
  int numero;
  char nome[100];
} Assento;

// Definição da struct Teatro
typedef struct Teatro {
  struct Assento **assentos;
  int numLinhas;
  int numColunas;
} Teatro;

void menu() {

  printf("\n-------------------------------------------------------\n");
  printf("Opção | Nome da Operação\n\n");
  printf("0     | Sair do programa\n");
  printf("1     | Visualizar\n");
  printf("2     | Reservar assento específico\n");
  printf("3     | Reservar assentos consecutivos específicos\n");
  printf("4     | Reservar assento pelo sistema\n");
  printf("5     | Reservar assentos consecutivos pelo sistema\n");
  printf("6     | Cancelar reserva de um assento\n");
  printf("7     | Cancelar reservas de uma pessoa\n");
  printf("8     | Cancelar todas as reservas\n");
  printf("9     | Verificar disponibilidade\n");
  printf("10    | Consultar assentos consecutivos\n");
  printf("11    | Exibir informações\n");
  printf("-------------------------------------------------------\n");
}

// Função para inicializar a matriz de assentos
struct Assento **inicializarMatriz(int numLinhas, int numColunas) {
  if (numLinhas > MAX_LINHAS || numColunas > MAX_COLUNAS) {
    printf("Número de linhas ou colunas inválido! reinicie o programa para usa-lo novamente.\n");
    return NULL;
  }

  Assento **matriz = (Assento **)malloc(numLinhas * sizeof(Assento *));
  if (matriz == NULL) {
    perror("Erro ao alocar matriz");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < numLinhas; i++) {
    matriz[i] = (Assento *)malloc(numColunas * sizeof(Assento));
    if (matriz[i] == NULL) {
      perror("Erro ao alocar matriz");
      exit(EXIT_FAILURE);
    }

    for (int j = 0; j < numColunas; j++) {
      matriz[i][j].reservado = 0;
      matriz[i][j].nome[0] = '\0';
    }
  }

  return matriz;
}

// Função para criar o Teatro
Teatro criarTeatro(int numLinhas, int numColunas) {
  Teatro teatro;
  teatro.assentos = inicializarMatriz(numLinhas, numColunas);
  teatro.numLinhas = numLinhas;
  teatro.numColunas = numColunas;
  return teatro;
}

// Função para exibir o estado atual do Teatro
void exibirTeatro(Teatro teatro) {
  char *fileiras[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I",
                      "J", "K", "L", "M", "N", "O", "P", "Q", "R",
                      "S", "T", "U", "V", "W", "X", "Y", "Z"};
  printf("Estado atual do Teatro (azul está livre e vermelho reservado):\n");
  for (int i = 0; i < teatro.numLinhas; i++) {
    for (int j = 0; j < teatro.numColunas; j++) {
      if (teatro.assentos[i][j].reservado) {
        printf(ANSI_COLOR_RED "%s%d " ANSI_COLOR_RESET, fileiras[i], j + 1);
      } else {
        printf(ANSI_COLOR_BLUE "%s%d " ANSI_COLOR_RESET, fileiras[i], j + 1);
      }
    }
    printf("\n");
  }
}

// Função para liberar a memória da matriz de assentos
void liberarMatriz(Assento **matriz, int linhas) {
  for (int i = 0; i < linhas; i++) {
    free(matriz[i]);
  }
  free(matriz);
}

bool isReservado(Teatro t, int linha, int coluna) {
  if (t.assentos[linha][coluna].reservado) {
    return true;
  }
  return false;
}

bool isLocalizacaoFalsa(int linha, int coluna, int numLinhas, int numColunas) {
  if (linha < 0 || linha > numLinhas || coluna < 0 || coluna > numColunas) {
    return true;
  }
  return false;
}

int reservarAssento(Teatro t, int linha, int coluna, char *nome) {
  if (isLocalizacaoFalsa(linha, coluna, t.numLinhas, t.numColunas)) {
    printf("Forneça uma localização válida!\n");
    return 0;
  }
  if (isReservado(t, linha - 1, coluna - 1)) {
    printf("Assento já reservado, tente outra posição!\n");
    return 0;
  } else {
    t.assentos[linha - 1][coluna - 1].reservado = 1;
    strcpy(t.assentos[linha - 1][coluna - 1].nome, nome);
    printf("Assento reservado com sucesso!\n");
  }
  return 1;
}

int reservarAssentosConsecutivos(Teatro t, int linha, int coluna, int n,
                                 char *nome) {
  if (isLocalizacaoFalsa(linha, coluna, t.numLinhas, t.numColunas)) {
    printf("Forneça uma localização válida!\n");
    return 0;
  }
  int contador = 0;
  for (int i = linha - 1; i < t.numLinhas; i++) {
    if (contador == n) {
      break;
    }
    int newColuna = coluna;
    if (i > linha - 1) {
      newColuna = 1;
    }
    for (int j = newColuna - 1; j < t.numColunas; j++) {
      if (contador == n) {
        break;
      }
      if (isReservado(t, i, j)) {
        printf("Não foi possível realizar as reservas\n");
        return 0;
      } else {
        t.assentos[i][j].reservado = 1;
        strcpy(t.assentos[i][j].nome, nome);
        contador += 1;
      }
    }
  }
  printf("Assentos reservados com sucesso");

  return 1;
}

Assento *reservarAssentoPeloSistema(Teatro t, char *nome) {
  for (int i = 0; i < t.numLinhas; i++) {
    for (int j = 0; j < t.numColunas; j++) {
      if (!isReservado(t, i, j)) {
        t.assentos[i][j].reservado = 1;
        strcpy(t.assentos[i][j].nome, nome);
        printf("Assento reservado com sucesso!\n");
        printf("seu lugar está na linha %d, coluna %d", i + 1, j + 1);
        return &t.assentos[i][j];
      }
    }
  }

  return NULL;
}

Assento **reservarAssentosConsecutivosPeloSistema(Teatro t, int n, char *nome) {

  int contador = 0;

  for (int i = 0; i < t.numLinhas; i++) {
    for (int j = 0; j < t.numColunas; j++) {
      for (int k = 0; k < n; k++) {

        if (isReservado(t, i, j + k) || (j+k) > t.numColunas) {
          contador = 0;
        }  

        if (!isReservado(t, i, j + k)) {
          contador++;
          if (contador == n){
            for (int l = 0; l < n; l++) {

              t.assentos[i - 1 ][j+l].reservado = 1;
              strcpy(t.assentos[i - 1][j].nome, nome);
              printf("Assento reservado com sucesso!\n");

            }
            break;
          }
          break;
        }
        break;
      }
      break;
    }
  }
  return 0;
}

int cancelarReserva(Teatro t, int linha, int coluna) {
  if (isLocalizacaoFalsa(linha, coluna, t.numLinhas, t.numColunas)) {
    printf("Forneça uma localização válida!\n");
    return 0;
  }
  if (!isReservado(t, linha - 1, coluna - 1)) {
    printf("Não foi possível cancelar a reserva, pois o assento não estava "
           "reservado");
    return 0;
  }

  t.assentos[linha - 1][coluna - 1].reservado = 0;
  printf("Assento cancelado com sucesso!\n");

  return 1;
}

int cancelarReservas(Teatro t, char *nome) {
  int contador = 0;
  for (int i = 0; i < t.numLinhas; i++) {
    for (int j = 0; j < t.numColunas; j++) {
      if (strcmp(t.assentos[i][j].nome, nome) == 0) {
        t.assentos[i][j].reservado = 0;
        contador += 1;
      }
    }
  }
  if (contador == 0) {
    printf("Não foi encontrada nenhuma reserva com para o nome digitado\n");
  } else {
    printf("%d reservas canceladas com sucesso!\n", contador);
  }
  return contador;
}

int cancelarTodasAsReservas(Teatro t) {
  int contador = 0;
  for (int i = 0; i < t.numLinhas; i++) {
    for (int j = 0; j < t.numColunas; j++) {
      if (isReservado(t, i, j)) {
        t.assentos[i][j].reservado = 0;
        contador += 1;
      }
    }
  }
  if (contador == 0) {
    printf("Não foi encontrada nenhuma reserva\n");
  } else {
    printf("%d reservas canceladas com sucesso!\n", contador);
  }
  return contador;
}

int verificarDisponibilidade(Teatro t, int linha, int coluna) {
  if (isLocalizacaoFalsa(linha, coluna, t.numLinhas, t.numColunas)) {
    printf("Forneça uma localização válida!\n");
    return 0;
  }
  if (isReservado(t, linha - 1, coluna - 1)) {
    printf("O assento escolhido já está reservado\n");
  } else {
    printf("O assento escolhido está disponível\n");
  }
  return 0;
}

 void consultarAssentosConsecutivos(Teatro t, int n) {
     int contador = 0;
    char *fileiras[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I",
    "J", "K", "L", "M", "N", "O", "P", "Q", "R",
    "S", "T", "U", "V", "W", "X", "Y", "Z"};

     for (int i = 0; i < t.numLinhas; i++) {
         for (int j = 0; j < t.numColunas; j++) {
            if (isReservado(t, i, j)){
              contador = 0;
            }

            if (!isReservado(t, i, j)) {
                contador++;
              if(j == t.numColunas - 1 && contador < n){
                contador = 0;
              }
              if ( contador == n){
                 printf("Os assentos %s%d até %s%d estão disponíveis\n", fileiras[i], (j - n) + 2, fileiras[i], j + 1);
                contador = 0;
               }
            }

            if ( i == t.numLinhas - 1 && j == t.numColunas - 1 && contador < n){
              break;
            }  
         }
     }
 }
 void exibirInformacoes(Teatro t){
   int contador_vagos = 0;
   int contador_ocupado = 0;
   for (int i = 0; i < t.numLinhas; i++) {
     for (int j = 0; j < t.numColunas; j++) {
       if (isReservado(t, i, j)) {
         t.assentos[i][j].reservado = 0;
         contador_ocupado++;
       }

       else{
         contador_vagos++;
       } 
     }
   }
   printf("lugares vagos = %d | lugares ocupados = %d", contador_vagos, contador_ocupado);
 }

int main() {
  int numLinhas, numColunas, opcao = 14;
  int linha, coluna, n;
  char nome[99];

  // Leitura do número de linhas e colunas
  printf("Digite o número de linhas e colunas (separados por espaço): ");
  scanf("%d %d", &numLinhas, &numColunas);

  if (inicializarMatriz( numLinhas, numColunas) == NULL) {
    return 0;
  }

  // Criação do Teatro
  struct Teatro teatro = criarTeatro(numLinhas, numColunas);


  while (opcao != 0) {
    menu();
    printf("selecione uma opção:");
    scanf("%d", &opcao);

    if (opcao == 0) {
      printf("obrigado por usar o programa!\n");
    }

    if (opcao == 1) {
      exibirTeatro(teatro);
    }

    if (opcao == 2) {
      printf("Digite a linha, a coluna do assento que deseja reservar e seu "
             "nome (separados por espaço): ");
      scanf("%d %d %s", &linha, &coluna, nome);
      reservarAssento(teatro, linha, coluna, nome);
    }

    if (opcao == 3) {
      printf(
          "Digite a linha e a coluna do assento inicial que deseja reservar, a "
          "quantidade de assentos e seu nome (separados por espaço): ");
      scanf("%d %d %d %s", &linha, &coluna, &n, nome);
      reservarAssentosConsecutivos(teatro, linha, coluna, n, nome);
    }

    if (opcao == 4) {
      printf("Digite o seu nome: ");
      scanf("%s", nome);
      reservarAssentoPeloSistema(teatro, nome);
    }

    if (opcao == 5) {
      printf("digite o número de lugares que você deseja ocupar e seu nome(separados por espaço): ");
      scanf("%d", &n);
      scanf("%s", nome);
      reservarAssentosConsecutivosPeloSistema(teatro, n, nome);
    }

    if (opcao == 6) {
      printf("Digite a linha e a coluna do assento que deseja cancelar a "
             "reserva: ");
      scanf("%d %d", &linha, &coluna);
      cancelarReserva(teatro, linha, coluna);
    }

    if (opcao == 7) {
      printf(
          "Digite o nome do cliente que deseja cancelar todas as reservas: ");
      scanf("%s", nome);
      cancelarReservas(teatro, nome);
    }

    if (opcao == 8) {
      cancelarTodasAsReservas(teatro);
      printf("Todas as reservas foram canceladas\n");
    }

    if (opcao == 9) {
      printf("Digite a linha e coluna do assento que deseja consultar: ");
      scanf("%d %d", &linha, &coluna);
      verificarDisponibilidade(teatro, linha, coluna);
    }

    if (opcao == 10) {

      printf("Digite o número de assentos consecutivos que deseja verificar: ");
      scanf("%d", &n);
      if (n > numColunas) {
        printf("operação inválida, pois o número selecionado é maior que a quantidade de colunas");
      }

      else{
        consultarAssentosConsecutivos(teatro, n);
      }

    }

    if (opcao == 11) {
      exibirInformacoes(teatro);
    }
  }

  // Liberação de memória
  liberarMatriz(teatro.assentos, teatro.numLinhas);

  return 0;
}