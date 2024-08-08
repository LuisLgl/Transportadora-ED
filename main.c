#include "transportadora.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para o menu de cadastro de clientes e pedidos
void menuCadastro(ListaClientes *lista, FilaFilas *filas) {
    int opcao;
    do {
        printf("\nMENU DE CADASTRO:\n");
        printf("1. Adicionar Cliente\n");
        printf("2. Remover Cliente\n");
        printf("3. Editar Cliente\n");
        printf("4. Imprimir Clientes\n");
        printf("5. Adicionar Pedido\n");
        printf("6. Remover Pedido\n");
        printf("7. Editar Pedido\n");
        printf("8. Imprimir Pedidos de um Cliente\n");
        printf("9. Imprimir Filas de Entregas\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionaCliente(lista);
                break;
            case 2:
                removeCliente(lista, filas);
                break;
            case 3:
                editaCliente(lista);
                break;
            case 4:
                imprimeClientes(lista);
                break;
            case 5:
                adicionaPedido(lista, filas);
                break;
            case 6: {
                int id_cliente;
                printf("Digite o ID do cliente: ");
                scanf("%d", &id_cliente);
                Cliente *cliente = encontrarClientePorID(lista, id_cliente);
                if (cliente != NULL) {
                    removePedido(cliente, filas);
                } else {
                    printf("Cliente não encontrado!\n");
                }
                break;
            }
            case 7: {
                int id_cliente;
                printf("Digite o ID do cliente: ");
                scanf("%d", &id_cliente);
                Cliente *cliente = encontrarClientePorID(lista, id_cliente);
                if (cliente != NULL) {
                    editaPedido(cliente);
                } else {
                    printf("Cliente não encontrado!\n");
                }
                break;
            }
            case 8: {
                int id_cliente;
                printf("Digite o ID do cliente: ");
                scanf("%d", &id_cliente);
                Cliente *cliente = encontrarClientePorID(lista, id_cliente);
                if (cliente != NULL) {
                    imprimePedidos(cliente);
                } else {
                    printf("Cliente não encontrado!\n");
                }
                break;
            }
            case 9:
                imprimeFilas(filas);
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}

// Função para o menu de transportes (placeholder)
void menuTransportes(ListaClientes *lista) {
    int opcao;
    do {
        printf("\nMENU DE TRANSPORTES:\n");
        printf("1. Alterar Status para Entregue\n");
        printf("2. Despachar Pedido\n");
        printf("3. Editar Entrega\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                alterarStatusParaEntregue(lista);
                break;
            case 2:
                // Implementar a função para despachar pedidos
                // (depende de como você deseja implementar o despacho de pedidos)
                break;
            case 3:
                // Implementar a função para editar entregas
                // (depende de como você deseja implementar a edição de entregas)
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}

int main() {
    ListaClientes lista;
    inicializaListaClientes(&lista);

    FilaFilas filas;
    inicializaListaFilas(&filas);

    char escolha;

    do {
        // Display the main menu
        printf("Menu Principal:\n");
        printf("1. Cadastro de Clientes e Pedidos\n");
        printf("2. Transportes\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &escolha);

        switch (escolha) {
            case '1':
                menuCadastro(&lista, &filas);
                break;
            case '2':
                menuTransportes(&lista);
                break;
            case '0':
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (escolha != '0');

    // Libere a memória aqui
    limparMemoria(&lista, &filas);

    return 0;
}
