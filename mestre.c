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
    int topo; // -1 quando vazia, 0..TAM_PILHA-1 quando tiver itens
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
void trocarFrenteComTopo(Fila *f, Pilha *p);
void trocaMultiplaTres(Fila *f, Pilha *p);

// ---------- FUNÇÃO PRINCIPAL ----------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int jogadas = 0;

    srand((unsigned int)time(NULL));
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

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da frente da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva (desempilhar)\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("=====================================\n");
        printf("Opção escolhida: ");
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
                    printf("\nAção: Jogar peça -> [%c %d]\n", jogada.nome, jogada.id);
                    // Gera nova peça para manter fila sempre cheia quando possível
                    enfileirar(&fila, gerarPeca());
                    jogadas++;
                } else {
                    printf("\nFila vazia! Nenhuma peça disponível.\n");
                }
                break;

            case 2: // Reservar peça (mover da frente da fila para topo da pilha)
                if (filaVazia(&fila)) {
                    printf("\nFila vazia! Nenhuma peça para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Libere espaço antes de reservar mais.\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\nAção: Reservar peça -> [%c %d]\n", reservada.nome, reservada.id);
                    // Repor a fila
                    enfileirar(&fila, gerarPeca());
                }
                break;

            case 3: // Usar peça da reserva (desempilhar)
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\nAção: Usar peça da reserva -> [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\nPilha de reserva está vazia!\n");
                }
                break;

            case 4: // Trocar peça da frente da fila com o topo da pilha
                trocarFrenteComTopo(&fila, &pilha);
                break;

            case 5: // Troca múltipla: trocar as 3 primeiras da fila com 3 peças da pilha
                trocaMultiplaTres(&fila, &pilha);
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

/* Inicializa a fila como vazia */
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

/* Inicializa a pilha como vazia */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

/* Retorna 1 se fila vazia, 0 caso contrário */
int filaVazia(Fila *f) {
    return (f->quantidade == 0);
}

/* Retorna 1 se fila cheia, 0 caso contrário */
int filaCheia(Fila *f) {
    return (f->quantidade == TAM_FILA);
}

/* Retorna 1 se pilha vazia, 0 caso contrário */
int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

/* Retorna 1 se pilha cheia, 0 caso contrário */
int pilhaCheia(Pilha *p) {
    return (p->topo == TAM_PILHA - 1);
}

/* Enfileira se houver espaço; caso contrário ignora (proteção) */
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->pecas[f->fim] = nova;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

/* Desenfileira -- pressupõe que não está vazia quando chamado */
Peca desenfileirar(Fila *f) {
    Peca removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

/* Empilha se houver espaço; caso contrário ignora (proteção) */
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->pecas[p->topo] = nova;
}

/* Desempilha -- pressupõe que não está vazia quando chamado */
Peca desempilhar(Pilha *p) {
    Peca removida = p->pecas[p->topo];
    p->topo--;
    return removida;
}

/* Exibe o estado atual da fila e da pilha com formatação clara */
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n-------------------------------------\n");
    printf("Fila de peças\t(Próxima -> Última): ");
    if (filaVazia(f)) {
        printf("[Vazia]");
    } else {
        for (int i = 0; i < f->quantidade; i++) {
            int index = (f->inicio + i) % TAM_FILA;
            printf("[%c %d] ", f->pecas[index].nome, f->pecas[index].id);
        }
    }

    printf("\nPilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }
    printf("\n-------------------------------------\n");
}

/* Gera nova peça com tipo aleatório e id incremental */
Peca gerarPeca() {
    static int contadorID = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;
    Peca nova;
    nova.nome = tipos[indice];
    nova.id = contadorID++;
    return nova;
}

/* Limpa buffer de stdin */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Troca a peça da frente da fila com o topo da pilha */
void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("\nFila vazia! Não há peça para trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Não há peça no topo para trocar.\n");
        return;
    }

    int idxFrente = f->inicio;
    Peca temp = f->pecas[idxFrente];
    f->pecas[idxFrente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;

    printf("\nAção: Troca realizada entre frente da fila e topo da pilha.\n");
}

/* Troca os 3 primeiros da fila com as 3 peças da pilha (se ambas tiverem >= 3) */
void trocaMultiplaTres(Fila *f, Pilha *p) {
    if (f->quantidade < 3) {
        printf("\nA fila não tem pelo menos 3 peças para realizar a troca múltipla.\n");
        return;
    }
    if ((p->topo + 1) < 3) {
        printf("\nA pilha não tem pelo menos 3 peças para realizar a troca múltipla.\n");
        return;
    }

    // Armazena os 3 primeiros da fila (f0,f1,f2)
    Peca ftemp[3];
    for (int i = 0; i < 3; i++) {
        int idx = (f->inicio + i) % TAM_FILA;
        ftemp[i] = f->pecas[idx];
    }

    // Armazena as 3 do topo da pilha em ordem Top -> Top-1 -> Top-2
    Peca stemp[3];
    for (int i = 0; i < 3; i++) {
        stemp[i] = p->pecas[p->topo - i]; // stemp[0] = top, stemp[1] = top-1 ...
    }

    // Atribui para a fila os valores vindos da pilha (mantendo ordem Top -> ...)
    for (int i = 0; i < 3; i++) {
        int idx = (f->inicio + i) % TAM_FILA;
        f->pecas[idx] = stemp[i];
    }

    // Atribui para a pilha os valores vindos da fila, mas em ordem Top -> Base = f2, f1, f0
    for (int i = 0; i < 3; i++) {
        p->pecas[p->topo - i] = ftemp[2 - i]; // para que novo topo seja f2
    }

    printf("\nAção: Troca múltipla realizada entre os 3 primeiros da fila e as 3 da pilha.\n");
}
