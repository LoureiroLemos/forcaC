#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PALAVRA 100
#define MAX_PALAVRAS 100
#define MAX_TENTATIVAS 26 // Número máximo de letras no alfabeto

// Estrutura para armazenar informações do jogo
typedef struct {
    int tentativas;
    char palavra[MAX_PALAVRA];
    char controle[MAX_PALAVRA];
    char letrasTentadas[MAX_TENTATIVAS];
    int chancesRestantes;
    int acertos;
} Jogo;

// Estrutura para armazenar informações do jogador
typedef struct {
    char nome[50];
    int jogos;
    int acertos;
    int erros;
} Jogador;

// Função para carregar palavras de um arquivo texto
int carregarPalavras(char palavras[MAX_PALAVRAS][MAX_PALAVRA]) {
    FILE *arquivo = fopen("palavras.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo 'palavras.txt'. Certifique-se de que ele está no mesmo diretório do programa.\n");
        return 0;
    }

    int i = 0;
    while (fscanf(arquivo, "%s", palavras[i]) != EOF && i < MAX_PALAVRAS) {
        i++;
    }
    fclose(arquivo);
    return i; // Retorna o número de palavras carregadas
}

// Função para criar um novo jogo
Jogo *novoJogo(char palavras[MAX_PALAVRAS][MAX_PALAVRA], int totalPalavras, char dificuldade) {
    Jogo *jogo = (Jogo *)malloc(sizeof(Jogo));
    if (!jogo) {
        printf("Erro ao alocar memória para o jogo.\n");
        return NULL;
    }

    memset(jogo->controle, 0, sizeof(jogo->controle));
    memset(jogo->letrasTentadas, 0, sizeof(jogo->letrasTentadas));
    srand(time(NULL));
    int nlinha = rand() % totalPalavras;
    strcpy(jogo->palavra, palavras[nlinha]);
    int tamanho = strlen(jogo->palavra);

    switch (dificuldade) {
        case '1':
            jogo->chancesRestantes = tamanho * 0.7;
            break;
        case '2':
            jogo->chancesRestantes = tamanho * 0.5;
            break;
        case '3':
            jogo->chancesRestantes = tamanho * 0.3;
            break;
    }

    jogo->tentativas = 0;
    jogo->acertos = 0;
    return jogo;
}

// Função para exibir as letras já tentadas
void mostrarLetrasTentadas(Jogo *jogo) {
    printf("\nLetras já tentadas: ");
    for (int i = 0; i < jogo->tentativas; i++) {
        printf("%c ", jogo->letrasTentadas[i]);
    }
    printf("\n");
}

// Função para o jogador adivinhar letras
void adivinhar(Jogo *jogo) {
    char letra;
    printf("\nAdivinhe uma letra: ");
    scanf(" %c", &letra);
    letra = tolower(letra);

    // Registrar a letra tentada
    jogo->letrasTentadas[jogo->tentativas++] = letra;

    int tamanho = strlen(jogo->palavra);
    int acertou = 0;

    for (int i = 0; i < tamanho; i++) {
        if (letra == jogo->palavra[i] && jogo->controle[i] == 0) {
            jogo->controle[i] = letra;
            jogo->acertos++;
            acertou = 1;
        }
    }

    if (!acertou) {
        jogo->chancesRestantes--;
        printf("Letra incorreta! Chances restantes: %d\n", jogo->chancesRestantes);
    }
}

// Função principal
int main() {
    char palavras[MAX_PALAVRAS][MAX_PALAVRA];
    int totalPalavras = carregarPalavras(palavras);
    if (totalPalavras == 0) {
        return 1; // Encerrar se não houver palavras carregadas
    }

    printf("DESENVOLVIDO POR:\n Helena Ferrreira de Moraes GRR20240454\n Melissa de Medeiros Soares Galindo Cseh GRR20244829\n Pedro Tonial Loureiro Guerreiro de Lemos GRR20244483\n Raí Boaventura GRR20241361\n Victor Hugo Betoni GRR20244740\n\n");

    Jogador jogador;
    printf("Digite seu nome: ");
    scanf("%s", jogador.nome);
    jogador.jogos = 0;
    jogador.acertos = 0;
    jogador.erros = 0;

    char jogarNovamente = 's';

    while (jogarNovamente == 's') {
        jogador.jogos++;

        // Seleção de dificuldade com validação
        char dificuldade;
        do {
            printf("\nBem-vindo ao Jogo da Forca!\n");
            printf("Escolha a dificuldade (1 - Fácil / 2 - Médio / 3 - Difícil): ");
            scanf(" %c", &dificuldade);

            if (dificuldade != '1' && dificuldade != '2' && dificuldade != '3') {
                printf("Opção inválida. Tente novamente.\n");
            }
        } while (dificuldade != '1' && dificuldade != '2' && dificuldade != '3');

        Jogo *jogo = novoJogo(palavras, totalPalavras, dificuldade);
        if (!jogo) {
            return 1;
        }

        int tamanho = strlen(jogo->palavra);
        printf("A palavra tem %d letras.\n", tamanho);

        while (jogo->chancesRestantes > 0) {
            mostrarLetrasTentadas(jogo); // Mostrar letras tentadas antes de cada tentativa

            printf("\nPalavra: ");
            for (int i = 0; i < tamanho; i++) {
                if (jogo->controle[i] != 0) {
                    printf("%c", jogo->controle[i]);
                } else {
                    printf("_ ");
                }
            }

            if (jogo->acertos == tamanho) {
                printf("\n\nParabéns! Você descobriu a palavra: %s\n", jogo->palavra);
                jogador.acertos++;
                break;
            }

            adivinhar(jogo);
        }

        if (jogo->chancesRestantes == 0) {
            printf("\nVocê perdeu! A palavra era: %s\n", jogo->palavra);
            jogador.erros++;
        }

        free(jogo);

        printf("\nDeseja jogar novamente? (s/n): ");
        scanf(" %c", &jogarNovamente);
    }

    printf("\nResumo do jogador:\n");
    printf("Nome: %s\n", jogador.nome);
    printf("Jogos jogados: %d\n", jogador.jogos);
    printf("Acertos: %d\n", jogador.acertos);
    printf("Erros: %d\n", jogador.erros);

    return 0;
}
