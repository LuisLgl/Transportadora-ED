#include <stdio.h>
#include <stdlib.h>
#include "transportadora.h"

void menuClientes(ListaClientes *listaClientes, FilaFilas *filaFilas);
void menuPedidos(ListaClientes *listaClientes, FilaFilas *filaFilas);
void menuEntregas(ListaClientes *clientes,FilaFilas *filaFilas, PilhaNaoEfetuada *pilhaNaoEfetuada, FilaDevolucao *filadevolucao, int *pontos);

int main()
{
    ListaClientes listaClientes;
    FilaFilas filaFilas;
    PilhaNaoEfetuada *pilhaNaoEfetuada;
    FilaDevolucao *filadevolucao;
    int opcao;
    int pontos = 0;

    // Inicializa as estruturas de dados
    inicializaListaClientes(&listaClientes);
    inicializaFilaFilas(&filaFilas);
    inicializaPilhaNaoEfetuada(&pilhaNaoEfetuada);
    inicializaFilaDevolucao(&filadevolucao);

    do
    {
        printf("\n===== Sistema de Gestao de Transportadora =====\n");
        printf("1. Gerenciar Clientes\n");
        printf("2. Gerenciar Pedidos\n");
        printf("3. Gerenciar Entregas\n");
        printf("4. Mostrar Pontos\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            menuClientes(&listaClientes, &filaFilas);
            break;
        case 2:
            menuPedidos(&listaClientes, &filaFilas);
            break;
        case 3:
            menuEntregas(&listaClientes,&filaFilas, pilhaNaoEfetuada, filadevolucao, &pontos);
            break;
        case 4:
            printf("Pontos atuais: %d\n", pontos);
            break;
        case 5:
            printf("Saindo...\n");
            limparMemoria(&listaClientes, &filaFilas, &pilhaNaoEfetuada, &filadevolucao);
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }
    } while (opcao != 5);

    limparMemoria(&listaClientes, &filaFilas, &pilhaNaoEfetuada, &filadevolucao);
    return 0;
}

void menuClientes(ListaClientes *listaClientes, FilaFilas *filaFilas)
{
    int opcao;

    do
    {
        printf("\n===== Gestao de Clientes =====\n");
        printf("1. Adicionar Cliente\n");
        printf("2. Remover Cliente\n");
        printf("3. Editar Cliente\n");
        printf("4. Imprimir Clientes\n");
        printf("5. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
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
            printf("Opção invalida! Tente novamente.\n");
            break;
        }
    } while (opcao != 5);
}

void menuPedidos(ListaClientes *listaClientes, FilaFilas *filaFilas)
{
    int opcao;

    do
    {
        printf("\n===== Gestao de Pedidos =====\n");
        printf("1. Adicionar Pedido\n");
        printf("2. Remover Pedido\n");
        printf("3. Editar Pedido\n");
        printf("4. Despachar Pedido\n");
        printf("5. Despachar todos os pedidos\n");
        printf("6. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            adicionaPedido(listaClientes);
            break;
        case 2:
        {
            int id_cliente;
            printf("Digite o ID do cliente do pedido a ser removido: ");
            scanf("%d", &id_cliente);
            Cliente *cliente = encontrarClientePorID(listaClientes, id_cliente);
            if (cliente != NULL)
            {
                removePedido(cliente, filaFilas);
            }
            else
            {
                printf("Cliente nao encontrado.\n");
            }
            break;
        }
        case 3:
        {
            int id_cliente;
            printf("Digite o ID do cliente do pedido a ser editado: ");
            scanf("%d", &id_cliente);
            Cliente *cliente = encontrarClientePorID(listaClientes, id_cliente);
            if (cliente != NULL)
            {
                editaPedido(cliente);
            }
            else
            {
                printf("Cliente nao encontrado.\n");
            }
            break;
        }
        case 4:
            despacharPedido(listaClientes, filaFilas);
            break;
        case 5:
            despacharTudo(listaClientes, filaFilas);

            break;
        case 6:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }
    } while (opcao != 6);
}

void menuEntregas(ListaClientes *clientes,FilaFilas *filaFilas, PilhaNaoEfetuada *pilhaNaoEfetuada, FilaDevolucao *filadevolucao, int *pontos)
{   
    int opcao;

    do
    {
        printf("\n===== Gestao de Entregas =====\n");
        printf("1. Imprimir Fila de Entregas\n");
        printf("2. Imprimir Pedidos Devolvidos\n");
        printf("3. Realizar Entrega\n");
        printf("4. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            imprimeFilaFilas(filaFilas);
            break;
        case 2:
            imprimeFilaDevolucao(filadevolucao);
            break;
        case 3:
            concluirEntrega(clientes, filaFilas, &pilhaNaoEfetuada, &filadevolucao, pontos);
            break;
        case 4:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }
    } while (opcao != 4);
}