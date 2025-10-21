#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para gerar pecas aleatoriamente

// Struct Peca: Define o tipo de dado para cada bloco (componente da fila)
typedef struct {
    char nome; // Caractere que representa o tipo da peca ('I', 'O', 'T', 'L', etc.)
    int id;    // Identificador unico para a ordem de criacao
} Peca;

// Definicao da capacidade da fila
#define CAPACIDADE_FILA 5

// --- Variaveis Globais da Fila Circular ---

Peca fila_pecas[CAPACIDADE_FILA]; // O array que implementa a fila
int frente = 0;                   // Indice do primeiro elemento (Dequeue)
int tras = 0;                     // Indice da proxima posicao livre (Enqueue)
int contador_pecas = 0;           // Rastreia o numero atual de pecas na fila
int id_global = 0;                // Gera IDs unicos para as pecas

// Protótipos das Funções
void limpar_buffer();
Peca gerarPeca();
int isFilaVazia();
int isFilaCheia();
void enqueue(Peca nova_peca);
void dequeue();
void exibirFila();
void exibirMenu();

// FUNÇÕES AUXILIARES DE E/S E GERAÇÃO DE PEÇAS


// Limpa o buffer de entrada (stdin)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funcao para gerar uma nova peca com ID unico e tipo aleatorio
Peca gerarPeca() {
    Peca nova;
    
    // Tipos de pecas (I, O, T, L)
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Escolhe um tipo aleatorio
    nova.nome = tipos[rand() % num_tipos];
    
    // Atribui um ID unico
    nova.id = id_global++;
    
    return nova;
}

// Retorna 1 (verdadeiro) se a fila nao contem elementos
int isFilaVazia() {
    return contador_pecas == 0;
}

// Retorna 1 (verdadeiro) se a fila atingiu a capacidade maxima
int isFilaCheia() {
    return contador_pecas == CAPACIDADE_FILA;
}

// Insere um novo elemento no final da fila (TRAS)
void enqueue(Peca nova_peca) {
    if (isFilaCheia()) {
        printf("\n[ATENCAO] A fila de pecas futuras esta cheia! Nao e possivel inserir a peca '%c'.\n", nova_peca.nome);
        return;
    }
    
    // Adiciona a nova peca na posicao 'tras'
    fila_pecas[tras] = nova_peca;
    
    // Avanca o ponteiro 'tras' de forma circular
    tras = (tras + 1) % CAPACIDADE_FILA;
    
    contador_pecas++;
    printf("\n[SUCESSO] Peca '%c' (ID %d) inserida no final da fila.\n", nova_peca.nome, nova_peca.id);
}

// Remove o elemento da frente da fila (FRENTE)
void dequeue() {
    if (isFilaVazia()) {
        printf("\n[ATENCAO] A fila de pecas esta vazia! Nenhuma peca para jogar.\n");
        return;
    }
    
    Peca peca_jogada = fila_pecas[frente];
    
    // Avanca o ponteiro 'frente' de forma circular
    frente = (frente + 1) % CAPACIDADE_FILA;
    
    contador_pecas--;
    printf("\n[JOGADA] Peca '%c' (ID %d) removida da frente da fila.\n", peca_jogada.nome, peca_jogada.id);
}

// Exibe o estado atual da fila
void exibirFila() {
    printf("\n--- FILA DE PECAS FUTURAS (Capacidade: %d/%d) ---\n", contador_pecas, CAPACIDADE_FILA);
    
    if (isFilaVazia()) {
        printf("Fila de pecas: (VAZIA)\n");
        printf("----------------------------------------------------\n");
        return;
    }
    
    // A logica da fila circular exige um loop que inicie na 'frente'
    // e avance por 'contador_pecas' passos
    printf("Fila de pecas: ");
    
    int i = frente;
    for (int count = 0; count < contador_pecas; count++) {
        Peca p = fila_pecas[i];
        
        // Exibe o formato [TIPO ID]
        printf("[%c %d] ", p.nome, p.id);
        
        // Avanca o indice de forma circular
        i = (i + 1) % CAPACIDADE_FILA;
    }
    
    printf("\n----------------------------------------------------\n");
    printf("Proxima a jogar: [%c %d]\n", fila_pecas[frente].nome, fila_pecas[frente].id);
}

// Exibe as opcoes de acao
void exibirMenu() {
    printf("\n========== ACOES DO JOGADOR ==========\n");
    printf("| [1] Jogar proxima peca (Dequeue)     |\n");
    printf("| [2] Inserir nova peca (Enqueue)      |\n");
    printf("| [0] Sair do Jogo                     |\n");
    printf("======================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    
    // Inicializa o gerador de numeros aleatorios (para gerarPeca)
    srand(time(NULL)); 
    
    printf(">>> Iniciando Tetris Stack: Controle de Fila Circular <<<\n");
    
    // 1. Inicializa a fila com 5 elementos (requisito funcional)
    printf("\nINICIALIZACAO - Preenchendo a fila com %d pecas iniciais.\n", CAPACIDADE_FILA);
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(gerarPeca());
    }
    
    // Exibe o estado inicial
    exibirFila();
    
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
                dequeue();
                break;
            case 2:
                enqueue(gerarPeca()); // Gera e insere a nova peca
                break;
            case 0:
                printf("\nFIM - Simulacao de fila encerrada. Ate a proxima rodada!\n");
                break;
            default:
                printf("\nALERTA - Opcao desconhecida.\n");
        }
        
        if (opcao != 0) {
            exibirFila(); // Exibe o estado atual da fila apos cada operacao
        }
        
    } while (opcao != 0);

    return 0;
}
