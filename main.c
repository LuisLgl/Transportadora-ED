#include "Transportadora.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    ListaClientes lista;
    inicializaListaClientes(&lista);

    int opcao;
    int clienteID;
    Cliente *cliente;

    do {
        printf("\nMenu:\n");
        printf("1. Adicionar Cliente\n");
        printf("2. Remover Cliente\n");
        printf("3. Editar Cliente\n");
        printf("4. Imprimir Clientes\n");
        printf("5. Adicionar Pedido\n");
        printf("6. Remover Pedido\n");
        printf("7. Editar Pedido\n");
        printf("8. Imprimir Pedidos de um Cliente\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionaCliente(&lista);
                break;
            case 2:
                removeCliente(&lista);
                break;
            case 3:
                editaCliente(&lista);
                break;
            case 4:
                imprimeClientes(&lista);
                break;
            case 5:
                printf("Digite o ID do cliente para adicionar um pedido: ");
                scanf("%d", &clienteID);
                cliente = lista.inicio;
                while (cliente != NULL && cliente->id != clienteID) {
                    cliente = cliente->prox;
                }
                if (cliente != NULL) {
                    adicionaPedido(cliente);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            case 6:
                printf("Digite o ID do cliente para remover um pedido: ");
                scanf("%d", &clienteID);
                cliente = lista.inicio;
                while (cliente != NULL && cliente->id != clienteID) {
                    cliente = cliente->prox;
                }
                if (cliente != NULL) {
                    removePedido(cliente);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            case 7:
                printf("Digite o ID do cliente para editar um pedido: ");
                scanf("%d", &clienteID);
                cliente = lista.inicio;
                while (cliente != NULL && cliente->id != clienteID) {
                    cliente = cliente->prox;
                }
                if (cliente != NULL) {
                    editaPedido(cliente);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            case 8:
                printf("Digite o ID do cliente para imprimir os pedidos: ");
                scanf("%d", &clienteID);
                cliente = lista.inicio;
                while (cliente != NULL && cliente->id != clienteID) {
                    cliente = cliente->prox;
                }
                if (cliente != NULL) {
                    imprimePedidos(cliente);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}