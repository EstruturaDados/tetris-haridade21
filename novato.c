#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5 // tamanho fixo da fila

// ---------- ESTRUTURA DA PEÇA ----------
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// ---------- ESTRUTURA DA FILA CIRCULAR ----------
typedef struct {
    Peca pecas[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ---------- PROTÓTIPOS DAS FUNÇÕES ----------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void enfileirar(Fila *f, Peca nova);
Peca desenfileirar(Fila *f);
void exibirFila(Fila *f);
Peca gerarPeca();

// ---------- FUNÇÃO PRINCIPAL ----------
int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n=====================================\n");
        printf("        FILA DE PEÇAS FUTURAS\n");
        printf("=====================================\n");
        exibirFila(&fila);
        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Inserir nova peça  \n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
                } else {
                    printf("\nFila vazia! Nenhuma peça para jogar.\n");
                }
                break;

            case 2:
                if (!filaCheia(&fila)) {
                    enfileirar(&fila, gerarPeca());
                    printf("\nNova peça adicionada!\n");
                } else {
                    printf("\nFila cheia! Espere jogar uma peça antes.\n");
                }
                break;

            case 0:
                printf("\nEncerrando o jogo... Até logo!\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// ---------- IMPLEMENTAÇÃO DAS FUNÇÕES ----------

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return (f->quantidade == 0);
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return (f->quantidade == TAM_FILA);
}

// Adiciona uma nova peça ao final da fila (enqueue)
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->pecas[f->fim] = nova;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

// Remove a peça da frente da fila (dequeue)
Peca desenfileirar(Fila *f) {
    Peca removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("\n[Fila vazia]\n");
        return;
    }

    printf("\nFila de peças: ");
    int i, index;
    for (i = 0; i < f->quantidade; i++) {
        index = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->pecas[index].nome, f->pecas[index].id);
    }
    printf("\n");
}

// Gera uma nova peça aleatória
Peca gerarPeca() {
    static int contadorID = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;

    Peca nova;
    nova.nome = tipos[indice];
    nova.id = contadorID++;

    return nova;
}
