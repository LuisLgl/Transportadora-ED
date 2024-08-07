#include "transportadora.h"

// Funções para a lista de clientes
void inicializaListaClientes(ListaClientes *lista) {
    lista->inicio = NULL;
}

void adicionaCliente(ListaClientes *lista) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para novo cliente.\n");
        return;
    }

    printf("Digite o CPF do cliente (apenas números, sem pontos ou traços): ");
    scanf("%s", novo->cpf);
    
    // Extrair os últimos 4 dígitos do CPF para serem usados como ID
    int cpf_length = strlen(novo->cpf);
    if (cpf_length < 4) {
        printf("CPF inválido! Deve conter pelo menos 4 dígitos.\n");
        free(novo);
        return;
    }
    novo->id = atoi(&novo->cpf[cpf_length - 4]);
    printf("Digite o nome do cliente: ");
    scanf("%s", novo->nome);
    printf("Digite o endereço do cliente: ");
    scanf("%s", novo->endereco);

    novo->pedidos = NULL;
    novo->prox = lista->inicio;
    lista->inicio = novo;

    printf("Cliente adicionado com sucesso!\n");
    printf("ID: %d, CPF: %s, Nome: %s, Endereço: %s\n", novo->id, novo->cpf, novo->nome, novo->endereco);
}


void removeCliente(ListaClientes *lista, ListaFilas *filas) {
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
        // Remover todos os pedidos do cliente
        Pedido *pedidoAtual = atual->pedidos;
        while (pedidoAtual != NULL) {
            Pedido *pedidoRemover = pedidoAtual;
            pedidoAtual = pedidoAtual->prox;
            removeEntrega(filas, atual->id); // Remover entrega associada ao pedido
            free(pedidoRemover);
        }
        
        // Remover o cliente da lista de clientes
        if (anterior == NULL) {
            lista->inicio = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        free(atual);
        printf("Cliente removido com sucesso!\n");
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
        int opcao;
        printf("O que deseja editar?\n");
        printf("1. Nome\n");
        printf("2. Endereço\n");
        printf("3. CPF\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                printf("Digite o novo nome do cliente: ");
                scanf("%s", atual->nome);
                printf("Nome atualizado com sucesso!\n");
                break;
            case 2:
                printf("Digite o novo endereço do cliente: ");
                scanf("%s", atual->endereco);
                printf("Endereço atualizado com sucesso!\n");
                break;
            case 3: {
                char novoCPF[12];
                printf("Digite o novo CPF do cliente (apenas números, sem pontos ou traços): ");
                scanf("%s", novoCPF);
                int cpf_length = strlen(novoCPF);
                if (cpf_length < 4) {
                    printf("CPF inválido! Deve conter pelo menos 4 dígitos.\n");
                } else {
                    strcpy(atual->cpf, novoCPF);
                    atual->id = atoi(&novoCPF[cpf_length - 4]);
                    printf("CPF e ID atualizados com sucesso!\n");
                }
                break;
            }
            default:
                printf("Opção inválida!\n");
        }
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

void adicionaPedido(ListaClientes *lista, ListaFilas *filas) {
    int id_cliente;
    printf("Digite o ID do cliente: ");
    scanf("%d", &id_cliente);

    Cliente *cliente = lista->inicio;
    while (cliente != NULL && cliente->id != id_cliente) {
        cliente = cliente->prox;
    }

    if (cliente == NULL) {
        printf("Cliente não encontrado. Adicionando novo cliente...\n");
        adicionaCliente(lista);
        cliente = lista->inicio;
    }

    Pedido *novo = (Pedido *)malloc(sizeof(Pedido));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para novo pedido.\n");
        return;
    }
    printf("Digite o ID do pedido: ");
    scanf("%d", &novo->id);
    printf("Digite a descrição do pedido: ");
    scanf(" %[^\n]", novo->descricao);  // Lê a linha inteira até a nova linha

    // Defina o status inicial do pedido como "preparando"
    strcpy(novo->status, "preparando");

    novo->prox = cliente->pedidos;
    cliente->pedidos = novo;
    adicionaEntrega(filas, novo->id, cliente->endereco);

    printf("Pedido adicionado com sucesso!\n");
    printf("ID do Pedido: %d, Descrição: %s, Status: %s\n", novo->id, novo->descricao, novo->status);
}


// Função para encontrar cliente por ID
Cliente* encontrarClientePorID(ListaClientes *lista, int id_cliente) {
    Cliente *cliente = lista->inicio;
    while (cliente != NULL && cliente->id != id_cliente) {
        cliente = cliente->prox;
    }
    return cliente;
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
        removeEntrega(filas, cliente->id);
        free(atual);
        printf("Pedido removido com sucesso.\n");
    } else {
        printf("Pedido não encontrado!\n");
    }
}


void editaPedido(Cliente *cliente, ListaFilas *filas) {
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

    int opcao;
    do {
        printf("O que deseja editar?\n");
        printf("1. Descrição\n");
        printf("2. ID do Pedido\n");
        printf("3. Status\n");
        printf("0. Sair\n");
        printf("Digite sua opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a nova descrição do pedido: ");
                scanf(" %[^\n]", atual->descricao);  // Lê a linha inteira até a nova linha
                printf("Descrição atualizada com sucesso.\n");
                break;
            case 2: {
                int novo_id;
                printf("Digite o novo ID do pedido: ");
                scanf("%d", &novo_id);

                // Verificar se o novo ID já existe na lista de pedidos
                Pedido *checar = cliente->pedidos;
                int id_existente = 0;
                while (checar != NULL) {
                    if (checar->id == novo_id) {
                        id_existente = 1;
                        break;
                    }
                    checar = checar->prox;
                }

                if (id_existente) {
                    printf("Erro: ID já existe. Escolha um ID diferente.\n");
                } else {
                    removeEntrega(filas, cliente->id); // Remover entrega com o ID antigo
                    atual->id = novo_id;
                    adicionaEntrega(filas, novo_id, cliente->endereco); // Adicionar entrega com o ID novo
                    printf("ID do pedido atualizado com sucesso.\n");
                }
                break;
            }
            case 3:
                printf("Digite o novo status do pedido: ");
                scanf(" %[^\n]", atual->status);
                printf("Status atualizado com sucesso.\n");
                break;
            case 0:
                printf("Saindo da edição.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}



void imprimePedidos(Cliente *cliente) {
    Pedido *atual = cliente->pedidos;
    if (atual == NULL) {
        printf("Nenhum pedido encontrado para este cliente.\n");
    } else {
        while (atual != NULL) {
            printf("ID: %d, Descrição: %s, Status: %s\n", atual->id, atual->descricao, atual->status);
            atual = atual->prox;
        }
    }
}

void despacharPedido(Cliente *cliente) {
    int id;
    printf("Digite o ID do pedido a ser despachado: ");
    scanf("%d", &id);

    Pedido *atual = cliente->pedidos;

    while (atual != NULL && atual->id != id) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Pedido não encontrado.\n");
        return;
    }

    if (strcmp(atual->status, "despachado") == 0) {
        printf("O pedido já foi despachado.\n");
        return;
    }

    printf("Confirmar o despacho do pedido ID: %d (s/n)? ", id);
    char confirmacao;
    scanf(" %c", &confirmacao);

    //colocar pra qual trasnportadora vai ser despachada

    if (confirmacao == 's' || confirmacao == 'S') {
        strcpy(atual->status, "despachado");
        printf("Pedido despachado com sucesso.\n");
    } else {
        printf("Despacho cancelado.\n");
    }

    printf("Deseja despachar outro pedido (s/n)? ");
    char novo_despacho;
    scanf(" %c", &novo_despacho);

    if (novo_despacho == 's' || novo_despacho == 'S') {
        despacharPedido(cliente);
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
        FilaPorEndereco *novaFila = (FilaPorEndereco *)malloc(sizeof(FilaPorEndereco));
        if (novaFila == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nova fila.\n");
        return;
    }
        strcpy(novaFila->endereco, endereco);
        novaFila->fila.inicio = NULL;
        novaFila->fila.fim = NULL;
        novaFila->prox = filas->inicio;
        filas->inicio = novaFila;
        atual = novaFila;
    }

    Entrega *novaEntrega = (Entrega *)malloc(sizeof(Entrega));
    
    novaEntrega->id_pedido = id_pedido; 
    strcpy(novaEntrega->endereco, endereco);
    novaEntrega->prox = NULL;

    if (atual->fila.fim == NULL) {
        atual->fila.inicio = novaEntrega;
    } else {
        atual->fila.fim->prox = novaEntrega;
    }

    atual->fila.fim = novaEntrega;
}

Entrega* removeEntrega(ListaFilas *filas, int id_pedido) {
    FilaPorEndereco *atual = filas->inicio;
    FilaPorEndereco *anterior = NULL;
    
    while (atual != NULL) {
        Entrega *entregaAnterior = NULL;
        Entrega *entregaAtual = atual->fila.inicio;

        while (entregaAtual != NULL && entregaAtual->id_pedido != id_pedido) {
            entregaAnterior = entregaAtual;
            entregaAtual = entregaAtual->prox;
        }

        if (entregaAtual != NULL) {
            // Remover entrega encontrada
            if (entregaAnterior == NULL) {
                atual->fila.inicio = entregaAtual->prox;
            } else {
                entregaAnterior->prox = entregaAtual->prox;
            }

            if (entregaAtual == atual->fila.fim) {
                atual->fila.fim = entregaAnterior;
            }

            if (atual->fila.inicio == NULL) {
                // Fila vazia, remover FilaPorEndereco
                if (anterior == NULL) {
                    filas->inicio = atual->prox;
                } else {
                    anterior->prox = atual->prox;
                }
                free(atual);
            }
            
            free(entregaAtual);
            return entregaAtual;
        }
        
        anterior = atual;
        atual = atual->prox;
    }

    printf("Entrega não encontrada para o pedido ID: %d.\n", id_pedido);
    return NULL;
}


void imprimeFilas(ListaFilas *filas) {
    FilaPorEndereco *atual = filas->inicio;

    while (atual != NULL) {
        printf("Endereço: %s\n", atual->endereco);
        Entrega *entregaAtual = atual->fila.inicio;
        while (entregaAtual != NULL) {
            printf("  ID do Pedido: %d\n", entregaAtual->id_pedido);
            entregaAtual = entregaAtual->prox;
        }
        atual = atual->prox;
    }
}

// Função de menu
void menuCadastro(ListaClientes *lista, ListaFilas *filas) {
    int opcao;
    int clienteID;
    Cliente *cliente;

    do {
        printf("\nMenu Cadastro:\n");
        printf("1. Adicionar Cliente\n");
        printf("2. Remover Cliente\n");
        printf("3. Editar Cliente\n");
        printf("4. Imprimir Clientes\n");
        printf("5. Adicionar Pedido\n");
        printf("6. Remover Pedido\n");
        printf("7. Editar Pedido\n");
        printf("8. Imprimir Pedidos de um Cliente\n");
        printf("0. Voltar\n");
        printf("Escolha uma opção: ");
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
                printf("Digite o ID do cliente para adicionar um pedido: ");
                scanf("%d", &clienteID);
                cliente = lista->inicio;
                while (cliente != NULL && cliente->id != clienteID) {
                    cliente = cliente->prox;
                }
                if (cliente != NULL) {
                    adicionaPedido(lista, filas);
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
                    removePedido(cliente, NULL);
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
                    editaPedido(cliente, filas);
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
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}
