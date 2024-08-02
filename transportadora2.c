#include "transportadora.h"

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