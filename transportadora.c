#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definições para pedidos
typedef struct Pedido {
    int id;
    char descricao[100];
    struct Pedido *prox;
} Pedido;

// Definições para clientes
typedef struct Cliente {
    int id;
    char nome[50];
    Pedido *pedidos;  // Lista de pedidos do cliente
    struct Cliente *prox;
} Cliente;

typedef struct {
    Cliente *inicio;
} ListaClientes;

// Funções para a lista de clientes
void inicializaListaClientes(ListaClientes *lista) {
    lista->inicio = NULL;
}

void adicionaCliente(ListaClientes *lista) {
    Cliente *novo = (Cliente*) malloc(sizeof(Cliente));
    if (novo == NULL) {
        printf("Erro ao alocar memória para novo cliente.\n");
        return;
    }

    printf("Digite o ID do cliente: ");
    scanf("%d", &novo->id);
    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", novo->nome);  // Lê a linha inteira até a nova linha

    novo->pedidos = NULL;  // Inicializa a lista de pedidos como vazia
    novo->prox = lista->inicio;
    lista->inicio = novo;
}

void removeCliente(ListaClientes *lista) {
    int id;
    printf("Digite o ID do cliente a ser removido: ");
    scanf("%d", &id);

    Cliente *atual = lista->inicio;
    Cliente *anterior = NULL;

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Cliente não encontrado.\n");
        return;
    }

    if (anterior == NULL) {
        lista->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    Pedido *pedidoAtual = atual->pedidos;
    while (pedidoAtual != NULL) {
        Pedido *pedidoProx = pedidoAtual->prox;
        free(pedidoAtual);
        pedidoAtual = pedidoProx;
    }

    free(atual); // Libera a memória do cliente removido
}

void editaCliente(ListaClientes *lista) {
    int id;
    printf("Digite o ID do cliente a ser editado: ");
    scanf("%d", &id);

    Cliente *atual = lista->inicio;

    while (atual != NULL && atual->id != id) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Cliente não encontrado.\n");
        return;
    }

    printf("Digite o novo nome do cliente: ");
    scanf(" %[^\n]", atual->nome);  // Lê a linha inteira até a nova linha
}

void imprimeClientes(ListaClientes *lista) {
    Cliente *atual = lista->inicio;

    while (atual != NULL) {
        printf("ID: %d, Nome: %s\n", atual->id, atual->nome);
        atual = atual->prox;
    }
}

// Funções para a lista de pedidos
void adicionaPedido(Cliente *cliente) {
    Pedido *novo = (Pedido*) malloc(sizeof(Pedido));
    if (novo == NULL) {
        printf("Erro ao alocar memória para novo pedido.\n");
        return;
    }

    printf("Digite o ID do pedido: ");
    scanf("%d", &novo->id);
    printf("Digite a descrição do pedido: ");
    scanf(" %[^\n]", novo->descricao);  // Lê a linha inteira até a nova linha

    novo->prox = cliente->pedidos;
    cliente->pedidos = novo;
}

void removePedido(Cliente *cliente) {
    int id;
    printf("Digite o ID do pedido a ser removido: ");
    scanf("%d", &id);

    Pedido *atual = cliente->pedidos;
    Pedido *anterior = NULL;

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Pedido não encontrado.\n");
        return;
    }

    if (anterior == NULL) {
        cliente->pedidos = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual); // Libera a memória do pedido removido
}

void editaPedido(Cliente *cliente) {
    int id;
    printf("Digite o ID do pedido a ser editado: ");
    scanf("%d", &id);

    Pedido *atual = cliente->pedidos;

    while (atual != NULL && atual->id != id) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Pedido não encontrado.\n");
        return;
    }

    printf("Digite a nova descrição do pedido: ");
    scanf(" %[^\n]", atual->descricao);  // Lê a linha inteira até a nova linha
}

void imprimePedidos(Cliente *cliente) {
    Pedido *atual = cliente->pedidos;

    while (atual != NULL) {
        printf("ID: %d, Descrição: %s\n", atual->id, atual->descricao);
        atual = atual->prox;
    }
}
