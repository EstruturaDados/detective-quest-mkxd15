#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define TAMANHO_TABELA 4

//Criação das Salas.
typedef struct No {
    char sala[50];
    char pista[50];
    struct No* esquerda;
    struct No* direita;
} No; // Nó da árvore binária

const char* pista_global[TAMANHO_TABELA] = {}; // Array para armazenar pistas
const char* suspeitos[TAMANHO_TABELA] = {"Suspeito1", "Suspeito2", "Suspeito3", "Suspeito4"}; // Lista de suspeitos

No* criarNo(char* sala, char* pista) { // Função para criar uma nova arvore binária para as salas e pistas
    No* novo = (No*)malloc(sizeof(No)); 
    if(novo == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }

    strcpy(novo->sala, sala);
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

No* inserirSala(No* mansao, char* sala, char* pista) { // Função para inserir salas na árvore binária
    if(mansao == NULL) {
        return criarNo(sala, pista);
    }
    if (strcmp(sala, mansao->sala) < 0) {
        mansao->esquerda = inserirSala(mansao->esquerda, sala, pista);
    } else {
        mansao->direita = inserirSala(mansao->direita, sala, pista);
    }

    return mansao;
}

void liberarArvore(No* mansao) { // Função para liberar a memória alocada para a árvore binária
    if (mansao != NULL) {
        liberarArvore(mansao->esquerda);
        liberarArvore(mansao->direita);
        free(mansao);
    }
}

void criarSala(No **mansao){ // Função para criar as salas iniciais da mansão
    *mansao = inserirSala(*mansao, "Hall de Entrada", "p1");
    *mansao = inserirSala(*mansao, "Sala de Estar", "p2");
    *mansao = inserirSala(*mansao, "Biblioteca", "p3");
    *mansao = inserirSala(*mansao, "Quarto", "p4");
}

// Criação de Pistas.
typedef struct NoBST {
    char valor[50];
    struct NoBST* esquerda;
    struct NoBST* direita;
} NoBST; // Nó da árvore binária de busca para pistas

NoBST* criarNoBST(const char* valor) { // Função para criar um novo nó BST para pistas
    NoBST* novo = (NoBST*)malloc(sizeof(NoBST));
    strcpy(novo->valor, valor);
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

NoBST* inserirPista(NoBST* raiz, const char* valor) { // Função para inserir pistas no nó BST
    if (raiz == NULL)
        return criarNoBST(valor);
    if (strcmp(valor, raiz->valor) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, valor);
    else
        raiz->direita = inserirPista(raiz->direita, valor);
    return raiz;
    }

void emOrdemBST(NoBST* raiz) { // Função para imprimir as pistas em ordem BST
    if(raiz!= NULL) {
        emOrdemBST(raiz->esquerda);
        printf("%s ", raiz->valor);
        emOrdemBST(raiz->direita);
    }
}


// Criação de suspeitos.
typedef struct { // Estrutura para tabela hash de suspeitos
    char chave[50];   // string usada como chave
    char valor[50];   // pode ser um suspeito, pista, etc.
    int ocupado;
} Entrada;

Entrada tabela[TAMANHO_TABELA];

int hash(const char* chave) { // Função hash simples
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_TABELA;
}

void inserirSuspeito(const char* chave) { // Função para inserir suspeitos na tabela hash
    int indice = hash(chave);

    while (tabela[indice].ocupado) {
        indice = (indice + 1) % TAMANHO_TABELA;
    }

    if(strcmp(chave, "p1") == 0){
        strcpy(tabela[indice].valor, suspeitos[0]);
    }
    else if(strcmp(chave, "p2") == 0){
        strcpy(tabela[indice].valor, suspeitos[1]);
    }
    else if(strcmp(chave, "p3") == 0){
        strcpy(tabela[indice].valor, suspeitos[2]);
    }
    else if(strcmp(chave, "p4") == 0){
        strcpy(tabela[indice].valor, suspeitos[3]);
    }

    strcpy(tabela[indice].chave, chave);
    tabela[indice].ocupado = 1;
}

int buscarSuspeito(const char* chave) { // Função para buscar suspeitos na tabela hash
    int indice = hash(chave);
    int inicio = indice;

    while (tabela[indice].ocupado) {
        if (strcmp(tabela[indice].chave, chave) == 0) {
            return indice;
        }
        indice = (indice + 1) % TAMANHO_TABELA;

        if (indice == inicio)
            break;  // deu a volta
    }

    return -1; // não encontrado
}

int verificarSuspeitoFinal(No *mansao) { // Função para acusar um suspeito baseado na pista atual
    char* pistaAtual = mansao->pista;

    printf("\nAcuse um suspeito baseado em suas pistas!\n");
    printf("Pista atual: %s\n\n", pistaAtual);
    printf("Suspeitos:\n");

    for (int i = 0; i < TAMANHO_TABELA; i++) {
        printf("%d. %s\n", i + 1, suspeitos[i] );
    }

    getchar();

    printf("Escolha suspeito: ");
    char sus[50];
    fgets(sus, 50, stdin);
    sus[strcspn(sus, "\n")] = '\0';

    for (int i = 0; i < TAMANHO_TABELA; i++) { // Verifica se a pista atual corresponde ao suspeito escolhido, se for o certo o programa termina
        if(strcmp(pistaAtual, tabela[i].chave) == 0){
            if (strcmp(sus, tabela[i].valor) == 0) {
                printf("Parabéns! Você acertou o culpado: %s.\n", tabela[i].valor);
                getchar();
                exit(0); 
            }
        }
    }

    printf("Suspeito inválido. Tente novamente.\n");
    getchar();
    return -1;
}


// Movimentação pela mansão.
void moverEsquerda(No **mansao, NoBST **pista) { // Função para mover para a sala à esquerda com base na arvore binária
    if ((*mansao)->esquerda != NULL) {
        *mansao = (*mansao)->esquerda;
        *pista = inserirPista(*pista, (*mansao)->pista);
        printf("\nVocê está em: %s.\n", (*mansao)->sala);
        printf("Você achou uma pista: %s.\n\n", (*mansao)->pista);

        for(int i = 0; i < TAMANHO_TABELA; i++){
            if(pista_global[i] == NULL){
                pista_global[i] = (*mansao)->pista;
                break;
            }
        }

        getchar();
    } else {
        printf("\nNão há sala à esquerda!\n");
    }
}

void moverDireita(No **mansao, NoBST **pista) { // Função para mover para a sala à direita com base na arvore binária
    if ((*mansao)->direita != NULL) {
        *mansao = (*mansao)->direita;
        *pista = inserirPista(*pista, (*mansao)->pista);
        printf("\nVocê está em: %s.\n", (*mansao)->sala);
        printf("Você achou uma pista: %s.\n\n", (*mansao)->pista);

        for(int i = 0; i < sizeof(pista_global)/sizeof(pista_global[0]); i++){
            if(pista_global[i] == NULL){
                pista_global[i] = (*mansao)->pista;
                break;
            }
        }

        getchar();
    } else {
        printf("\nNão há sala à direita!\n");
    }
}


void explorarSalas(){ // Função principal para explorar as salas da mansão
    int opcao;   
    No* mansao = NULL;
    NoBST* pista = NULL;
    criarSala(&mansao);

    printf("\nVocê está em: %s.\n\n", mansao->sala);

    do{
        printf("Qual direção você deseja ir?\n");
        printf("1. Esquerda (e).\n");
        printf("2. Direita (d).\n");
        printf("3. Sala atual.\n");
        printf("4. Acusar Suspeito.\n");
        printf("0. Sair\n\n");
        printf("Escolha uma opção: ");
    
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                moverEsquerda(&mansao, &pista);
                inserirSuspeito(mansao->pista);
                break;
            case 2:
                moverDireita(&mansao, &pista);
                inserirSuspeito(mansao->pista);
                break;
            case 3:
                printf("\n%s. \n", mansao->sala);
                getchar();
                break;
            case 4:
                verificarSuspeitoFinal(mansao);
                break;
            case 0:
                printf("\nVocê achou estas pistas: ");
                emOrdemBST(pista);
                printf(".\n");
                liberarArvore(mansao);
                break;
        }
    } while (opcao != 0);
}

int main() {
    explorarSalas();
}
