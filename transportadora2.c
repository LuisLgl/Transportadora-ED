#include "transportadora.h"
#include "transportadora1.c"

// Funções para a lista de clientes
void inicializaListaClientes(ListaClientes *lista) {
    lista->inicio = NULL;
}

void adicionaCliente(ListaClientes *lista) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    printf("Digite o ID do cliente: ");
    scanf("%d", &novo->id);
    printf("Digite o nome do cliente: ");
    scanf("%s", novo->nome);
    printf("Digite o endereço do cliente: ");
    scanf("%s", novo->endereco);
    novo->pedidos = NULL;
    novo->prox = lista->inicio;
    lista->inicio = novo;
}

void removeCliente(ListaClientes *lista) {
    int id;
    printf("Digite o ID do cliente a ser removido: ");
    scanf("%d", &id);
    Cliente *anterior = NULL;
    Cliente *atual = lista->inicio;
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        if (anterior == NULL) {
            lista->inicio = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        free(atual);
    } else {
        printf("Cliente não encontrado!\n");
    }
}

void editaCliente(ListaClientes *lista) {
    int id;
    printf("Digite o ID do cliente a ser editado: ");
    scanf("%d", &id);
    Cliente *atual = lista->inicio;
    while (atual != NULL && atual->id != id) {
        atual = atual->prox;
    }
    if (atual != NULL) {
        printf("Digite o novo nome do cliente: ");
        scanf("%s", atual->nome);
        printf("Digite o novo endereço do cliente: ");
        scanf("%s", atual->endereco);
    } else {
        printf("Cliente não encontrado!\n");
    }
}

void imprimeClientes(ListaClientes *lista) {
    Cliente *atual = lista->inicio;
    while (atual != NULL) {
        printf("ID: %d, Nome: %s, Endereço: %s\n", atual->id, atual->nome, atual->endereco);
        imprimePedidos(atual);
        atual = atual->prox;
    }
}

// Funções para a lista de pedidos
void adicionaPedido(Cliente *cliente, ListaFilas *filas) {
    Pedido *novo = (Pedido *)malloc(sizeof(Pedido));
    printf("Digite o ID do pedido: ");
    scanf("%d", &novo->id);
    printf("Digite a descrição do pedido: ");
    scanf("%s", novo->descricao);
    novo->prox = cliente->pedidos;
    cliente->pedidos = novo;
    adicionaEntrega(filas, novo->id, cliente->endereco);
}

void removePedido(Cliente *cliente, ListaFilas *filas) {
    int id;
    printf("Digite o ID do pedido a ser removido: ");
    scanf("%d", &id);
    Pedido *anterior = NULL;
    Pedido *atual = cliente->pedidos;
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        if (anterior == NULL) {
            cliente->pedidos = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        removeEntrega(filas, cliente->endereco);
        free(atual);
    } else {
        printf("Pedido não encontrado!\n");
    }
}

void editaPedido(Cliente *cliente) {
    int id;
    printf("Digite o ID do pedido a ser editado: ");
    scanf("%d", &id);
    Pedido *atual = cliente->pedidos;
    while (atual != NULL && atual->id != id) {
        atual = atual->prox;
    }
    if (atual != NULL) {
        printf("Digite a nova descrição do pedido: ");
        scanf("%s", atual->descricao);
    } else {
        printf("Pedido não encontrado!\n");
    }
}

void imprimePedidos(Cliente *cliente) {
    Pedido *atual = cliente->pedidos;
    while (atual != NULL) {
        printf("    ID do Pedido: %d, Descrição: %s\n", atual->id, atual->descricao);
        atual = atual->prox;
    }
}

// Funções para a lista de filas de entregas
void inicializaListaFilas(ListaFilas *filas) {
    filas->inicio = NULL;
}

void adicionaEntrega(ListaFilas *filas, int id_pedido, char *endereco) {
    FilaPorEndereco *atual = filas->inicio;
    while (atual != NULL && strcmp(atual->endereco, endereco) != 0) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        atual = (FilaPorEndereco *)malloc(sizeof(FilaPorEndereco));
        strcpy(atual->endereco, endereco);
        atual->fila.inicio = NULL;
        atual->fila.fim = NULL;
        atual->prox = filas->inicio;
        filas->inicio = atual;
    }

    Entrega *nova = (Entrega *)malloc(sizeof(Entrega));
    nova->id_pedido = id_pedido;
    strcpy(nova->endereco, endereco);
    nova->prox = NULL;

    if (atual->fila.fim == NULL) {
        atual->fila.inicio = nova;
        atual->fila.fim = nova;
    } else {
        atual->fila.fim->prox = nova;
        atual->fila.fim = nova;
    }
}

Entrega* removeEntrega(ListaFilas *filas, char *endereco) {
    FilaPorEndereco *atual = filas->inicio;
    while (atual != NULL && strcmp(atual->endereco, endereco) != 0) {
        atual = atual->prox;
    }

    if (atual != NULL && atual->fila.inicio != NULL) {
        Entrega *entrega = atual->fila.inicio;
        atual->fila.inicio = atual->fila.inicio->prox;
        if (atual->fila.inicio == NULL) {
            atual->fila.fim = NULL;
        }
        return entrega;
    }
    return NULL;
}

void imprimeFilas(ListaFilas *filas) {
    FilaPorEndereco *atual = filas->inicio;
    while (atual != NULL) {
        printf("Endereço: %s\n", atual->endereco);
        Entrega *entregaAtual = atual->fila.inicio;
        while (entregaAtual != NULL) {
            printf("    ID do Pedido: %d\n", entregaAtual->id_pedido);
            entregaAtual = entregaAtual->prox;
        }
        atual = atual->prox;
    }
}
