#include "transportadora.h"

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
    scanf(" %[^\n]", novo->descricao);  // Lê a linha inteira até a nova linha
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

void menu(ListaClientes *lista, ListaFilas *filas) {
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
                adicionaCliente(lista);
                break;
            case 2:
                removeCliente(lista);
                break;
            case 3:
                editaCliente(lista);
                break;
            case 4:
                imprimeClientes(lista);
                break;
            case 5:
                printf("Digite o ID do cliente para adicionar um pedido: ");
                scanf("%d", &clienteID);
                cliente = lista->inicio;
                while (cliente != NULL && cliente->id != clienteID) {
                    cliente = cliente->prox;
                }
                if (cliente != NULL) {
                    adicionaPedido(cliente, filas);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            case 6:
                printf("Digite o ID do cliente para remover um pedido: ");
                scanf("%d", &clienteID);
                cliente = lista->inicio;
                while (cliente != NULL && cliente->id != clienteID) {
                    cliente = cliente->prox;
                }
                if (cliente != NULL) {
                    removePedido(cliente, filas);
                } else {
                    printf("Cliente não encontrado.\n");
                }
                break;
            case 7:
                printf("Digite o ID do cliente para editar um pedido: ");
                scanf("%d", &clienteID);
                cliente = lista->inicio;
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
                cliente = lista->inicio;
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
}
