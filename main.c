#include <stdio.h>
#include <stdlib.h>
#include "transportadora.h"

void menuClientes(ListaClientes *listaClientes, FilaFilas *filaFilas);
void menuPedidos(ListaClientes *listaClientes, FilaFilas *filaFilas);
void menuEntregas(FilaFilas *filaFilas);

int main() {
    ListaClientes listaClientes;
    FilaFilas filaFilas;
    PilhaNaoEfetuada *pilhaNaoEfetuada;
    PilhaDevolucao *pilhaDevolucao;
    int opcao;

    // Inicializa as estruturas de dados
    inicializaListaClientes(&listaClientes);
    inicializaFilaFilas(&filaFilas);
    inicializaPilhaNaoEfetuada(&pilhaNaoEfetuada);
    inicializaPilhaDevolucao(&pilhaDevolucao);

    do {
        printf("\n===== Sistema de Gestão de Transportadora =====\n");
        printf("1. Gerenciar Clientes\n");
        printf("2. Gerenciar Pedidos\n");
        printf("3. Gerenciar Entregas\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                menuClientes(&listaClientes, &filaFilas);
                break;
            case 2:
                menuPedidos(&listaClientes, &filaFilas);
                break;
            case 3:
                menuEntregas(&filaFilas);
                break;
            case 4:
                printf("Saindo...\n");
                // Liberação de memória
                //limparMemoria(&listaClientes, &filaFilas);
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 4);

    return 0;
}

void menuClientes(ListaClientes *listaClientes, FilaFilas *filaFilas) {
    int opcao;

    do {
        printf("\n===== Gestão de Clientes =====\n");
        printf("1. Adicionar Cliente\n");
        printf("2. Remover Cliente\n");
        printf("3. Editar Cliente\n");
        printf("4. Imprimir Clientes\n");
        printf("5. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionaCliente(listaClientes);
                break;
            case 2:
                removeCliente(listaClientes, filaFilas);
                break;
            case 3:
                editaCliente(listaClientes, filaFilas);
                break;
            case 4:
                imprimeClientes(listaClientes);
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 5);
}

void menuPedidos(ListaClientes *listaClientes, FilaFilas *filaFilas) {
    int opcao;

    do {
        printf("\n===== Gestão de Pedidos =====\n");
        printf("1. Adicionar Pedido\n");
        printf("2. Remover Pedido\n");
        printf("3. Editar Pedido\n");
        printf("4. Despachar Pedido\n"); // Nova opção
        printf("5. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionaPedido(listaClientes, filaFilas);
                break;
            case 2: {
                int id_cliente;
                printf("Digite o ID do cliente do pedido a ser removido: ");
                scanf("%d", &id_cliente);
                Cliente *cliente = encontrarClientePorID(listaClientes, id_cliente);
                if (cliente != NULL) {
                    removePedido(cliente, filaFilas);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            }
            case 3: {
                int id_cliente;
                printf("Digite o ID do cliente do pedido a ser editado: ");
                scanf("%d", &id_cliente);
                Cliente *cliente = encontrarClientePorID(listaClientes, id_cliente);
                if (cliente != NULL) {
                    editaPedido(cliente);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            }
            case 4:
                despacharPedido(listaClientes, filaFilas);
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 5);
}

void menuEntregas(FilaFilas *filaFilas) {
    int opcao;

    do {
        printf("\n===== Gestão de Entregas =====\n");
        printf("1. Imprimir Fila de Entregas\n");
        printf("2. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                imprimeFilaFilas(filaFilas);
                break;
            case 2:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 2);
}
