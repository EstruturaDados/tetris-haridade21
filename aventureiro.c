#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5   // Tamanho fixo da fila circular
#define TAM_PILHA 3  // Capacidade máxima da pilha de reserva

// ---------- ESTRUTURA DA PEÇA ----------
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// ---------- ESTRUTURA DA FILA CIRCULAR ----------
typedef struct {
    Peca pecas[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ---------- ESTRUTURA DA PILHA ----------
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ---------- PROTÓTIPOS ----------
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void enfileirar(Fila *f, Peca nova);
Peca desenfileirar(Fila *f);
void empilhar(Pilha *p, Peca nova);
Peca desempilhar(Pilha *p);
void exibirEstado(Fila *f, Pilha *p);
Peca gerarPeca();
void limparBuffer();

// ---------- FUNÇÃO PRINCIPAL ----------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int jogadas = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    // ---------- LOOP PRINCIPAL ----------
    do {
        printf("\n=====================================\n");
        printf("        GERENCIAMENTO DE PEÇAS\n");
        printf("            TETRIS STACK\n");
        printf("=====================================\n");

        exibirEstado(&fila, &pilha);

        printf("\nAções disponíveis:\n");
        printf("1 - Jogar próxima peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Esvaziar reserva\n");
        printf("0 - Sair do jogo\n");
        printf("=====================================\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada inválida! Digite um número.\n");
            limparBuffer();
            continue; // volta para o menu
        }

        limparBuffer(); // limpa o buffer após a leitura

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca());
                    jogadas++;
                } else {
                    printf("\nFila vazia! Nenhuma peça disponível.\n");
                }
                break;

            case 2: // Reservar peça
                if (filaVazia(&fila)) {
                    printf("\nFila vazia! Nenhuma peça para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Libere espaço antes de reservar mais.\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\nPeça [%c %d] reservada com sucesso!\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca());
                }
                break;

            case 3: // Usar peça da reserva
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\nPeça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\nPilha de reserva está vazia!\n");
                }
                break;

            case 4: // Esvaziar reserva
                inicializarPilha(&pilha);
                printf("\nPilha de reserva esvaziada.\n");
                break;

            case 0:
                printf("\nEncerrando o jogo... Total de jogadas: %d\n", jogadas);
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ---------- IMPLEMENTAÇÕES ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) {
    return (f->quantidade == 0);
}

int filaCheia(Fila *f) {
    return (f->quantidade == TAM_FILA);
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

int pilhaCheia(Pilha *p) {
    return (p->topo == TAM_PILHA - 1);
}

void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->pecas[f->fim] = nova;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->pecas[p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca removida = p->pecas[p->topo];
    p->topo--;
    return removida;
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n-------------------------------------\n");
    printf("Fila de peças (Próxima → Última): ");
    if (filaVazia(f)) {
        printf("[Vazia]");
    } else {
        for (int i = 0; i < f->quantidade; i++) {
            int index = (f->inicio + i) % TAM_FILA;
            printf("[%c %d] ", f->pecas[index].nome, f->pecas[index].id);
        }
    }

    printf("\nPilha de reserva (Topo → Base): ");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }
    printf("\n-------------------------------------\n");
}

Peca gerarPeca() {
    static int contadorID = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;
    Peca nova;
    nova.nome = tipos[indice];
    nova.id = contadorID++;
    return nova;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
