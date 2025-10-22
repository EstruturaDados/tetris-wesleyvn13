#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// Struct Peca: Tipo de dado basico (componente de ambas as estruturas)
typedef struct {
    char nome; // Tipo da peca ('I', 'O', 'T', etc.)
    int id;    // Identificador unico
} Peca;

// Definicoes de Capacidade
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3 // Capacidade da Pilha de Reserva

//Variaveis Globais de Controle

// FILA CIRCULAR
Peca fila_pecas[CAPACIDADE_FILA];
int frente = 0;
int tras = 0;
int contador_pecas_fila = 0; // Quantidade de pecas na fila

// PILHA LINEAR
Peca pilha_reserva[CAPACIDADE_PILHA];
int topo_pilha = -1; // Indice do topo (inicia em -1 para pilha vazia)

// GERAÇÃO DE ID
int id_global = 0; 

// Protótipos das Funções
void limpar_buffer();
Peca gerarPeca();
// Fila
int isFilaCheia();
void enqueue(Peca nova_peca);
Peca dequeue(); // Retorna a peca jogada
// Pilha
int isPilhaCheia();
int isPilhaVazia();
void push(Peca peca);
Peca pop();
// Acoes do Jogo
void jogarPeca();
void reservarPeca();
void usarPecaReservada();
// Exibicao
void exibirEstadoAtual();
void exibirMenu();

// FUNÇÕES AUXILIARES E GERAÇÃO DE PEÇAS


void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funcao para gerar uma nova peca com ID unico e tipo aleatorio
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    nova.nome = tipos[rand() % num_tipos];
    nova.id = id_global++;
    
    return nova;
}

// FUNÇÕES DE MANIPULAÇÃO DA FILA CIRCULAR


int isFilaCheia() {
    return contador_pecas_fila == CAPACIDADE_FILA;
}

// Insere um novo elemento no final da fila (TRAS)
void enqueue(Peca nova_peca) {
    if (isFilaCheia()) {
        // Esta condicao nunca deve ocorrer neste jogo, pois a fila e sempre mantida cheia,
        // mas e boa pratica mante-la.
        return; 
    }
    
    fila_pecas[tras] = nova_peca;
    tras = (tras + 1) % CAPACIDADE_FILA; // Avanca de forma circular
    contador_pecas_fila++;
}

// Remove e retorna o elemento da frente da fila (FRENTE)
Peca dequeue() {
 
    
    Peca peca_jogada = fila_pecas[frente];
    
    frente = (frente + 1) % CAPACIDADE_FILA; // Avanca de forma circular
    contador_pecas_fila--;
    
    return peca_jogada;
}

int isPilhaCheia() {
    return topo_pilha == CAPACIDADE_PILHA - 1;
}

int isPilhaVazia() {
    return topo_pilha == -1;
}

// Insere um elemento no topo da pilha (PUSH)
void push(Peca peca) {
    if (isPilhaCheia()) {
        printf("ERRO - Pilha de reserva cheia! Nao foi possivel reservar a peca.\n");
        return;
    }
    
    topo_pilha++; // Avanca o topo
    pilha_reserva[topo_pilha] = peca; // Insere a peca
    
    printf("SUCESSO - Peça '%c' (ID %d) movida para a reserva.\n", peca.nome, peca.id);
}

// Remove e retorna o elemento do topo da pilha (POP)
Peca pop() {
    // Esta funcao e chamada SOMENTE se isPilhaVazia() for falsa
    Peca peca_usada = pilha_reserva[topo_pilha];
    topo_pilha--; // Retrai o topo (LIFO)
    return peca_usada;
}

// =================================================================
// FUNÇÕES DE AÇÃO DO JOGADOR
// =================================================================

// 1. Joga uma peca da fila (Dequeue + Reabastecer)
void jogarPeca() {
    Peca peca_jogada = dequeue(); // Peca removida da frente
    printf("JOGADA - Peca '%c' (ID %d) usada no tabuleiro.\n", peca_jogada.nome, peca_jogada.id);
    
    // Requisito: A cada acao, uma nova peca é gerada e adicionada ao final da fila.
    enqueue(gerarPeca()); 
}

// 2. Reserva uma peca (Dequeue + Push)
void reservarPeca() {
    if (isPilhaCheia()) {
        printf("FALHA - Pilha de reserva cheia. Jogue ou use uma peca reservada primeiro.\n");
        return;
    }
    
    Peca peca_reservada = dequeue(); // Peca removida da frente da fila
    push(peca_reservada);           // Peca adicionada ao topo da pilha
    
    // Requisito: A cada acao, uma nova peca é gerada e adicionada ao final da fila.
    enqueue(gerarPeca()); 
}

// 3. Usa uma peca reservada (Pop + Reabastecer)
void usarPecaReservada() {
    if (isPilhaVazia()) {
        printf("FALHA - A pilha de reserva esta vazia. Nenhuma peca para usar.\n");
        return;
    }
    
    Peca peca_usada = pop(); // Peca removida do topo da pilha (LIFO)
    printf("USADA - Peca reservada '%c' (ID %d) usada no tabuleiro.\n", peca_usada.nome, peca_usada.id);
    
    // Requisito: A cada acao, uma nova peca é gerada e adicionada ao final da fila.
    enqueue(gerarPeca()); 
}

// =================================================================
// FUNÇÕES DE EXIBIÇÃO
// =================================================================

void exibirFila() {
    printf("Fila de pecas: ");
    
    if (contador_pecas_fila == 0) {
        printf("(VAZIA)");
        return;
    }
    
    // Percorre a fila a partir da 'frente'
    int i = frente;
    for (int count = 0; count < contador_pecas_fila; count++) {
        Peca p = fila_pecas[i];
        printf("[%c %d] ", p.nome, p.id);
        i = (i + 1) % CAPACIDADE_FILA;
    }
}

void exibirPilha() {
    printf("Pilha de reserva (Topo -> Base): ");
    
    if (isPilhaVazia()) {
        printf("(VAZIA)");
        return;
    }

    // Percorre a pilha do topo para a base (LIFO)
    for (int i = topo_pilha; i >= 0; i--) {
        Peca p = pilha_reserva[i];
        printf("[%c %d] ", p.nome, p.id);
    }
}

void exibirEstadoAtual() {
    printf("\n--- ESTADO ATUAL ---\n");
    exibirFila();
    printf("\n");
    exibirPilha();
    printf("\n--------------------\n");
}

void exibirMenu() {
    printf("\n========== ACOES DO JOGADOR ==========\n");
    printf("| 1 - Jogar proxima peca (Fila -> Tabuleiro)   |\n");
    printf("| 2 - Reservar peca (Fila -> Pilha)            |\n");
    printf("| 3 - Usar peca reservada (Pilha -> Tabuleiro) |\n");
    printf("| 0 - Sair do Jogo                             |\n");
    printf("======================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    
    // Inicializa o gerador de numeros aleatorios
    srand(time(NULL)); 
    
    printf(">>> Iniciando Tetris Stack: Fila e Pilha <<<\n");
    
    // 1. Inicializa a fila com a capacidade maxima
    printf("\nSETUP - Preenchendo a fila com %d pecas iniciais.\n", CAPACIDADE_FILA);
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(gerarPeca());
    }
    
    exibirEstadoAtual();
    
    do {
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nERRO - Opcao invalida. Tente novamente.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 0:
                printf("\nFIM - Simulacao encerrada. Ate a proxima rodada!\n");
                break;
            default:
                printf("\nALERTA - Opcao desconhecida.\n");
        }
        
        if (opcao != 0) {
            exibirEstadoAtual(); // Exibe o estado atual apos cada operacao
        }
        
    } while (opcao != 0);

    return 0;
}
