#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transportadora.h"

// Funções para a lista de clientes
void inicializaListaClientes(ListaClientes *lista)
{
    lista->inicio = NULL;
}

void adicionaCliente(ListaClientes *lista)
{
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para novo cliente.\n");
        return;
    }

    printf("Digite o CPF do cliente (apenas números, sem pontos ou traços): ");
    scanf("%s", novo->cpf);

    int cpf_length = strlen(novo->cpf);
    if (cpf_length < 4)
    {
        printf("CPF inválido! Deve conter pelo menos 4 dígitos.\n");
        free(novo);
        return;
    }

    // Extrai os últimos 4 dígitos do CPF para definir o ID
    char cpf_ultimos_4[5];
    strncpy(cpf_ultimos_4, novo->cpf + cpf_length - 4, 4);
    cpf_ultimos_4[4] = '\0'; // Garantir que a string está terminada
    novo->id = atoi(cpf_ultimos_4);

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", novo->nome); // Corrigido para aceitar espaços
    printf("Digite o endereço do cliente: ");
    scanf(" %[^\n]", novo->endereco); // Corrigido para aceitar espaços

    novo->pedidos = NULL;
    novo->prox = lista->inicio;
    lista->inicio = novo;

    printf("Cliente adicionado com sucesso!\n");
    printf("ID: %d, CPF: %s, Nome: %s, Endereço: %s\n", novo->id, novo->cpf, novo->nome, novo->endereco);
}

void removeCliente(ListaClientes *lista, FilaFilas *filas)
{
    int id;
    printf("Digite o ID do cliente a ser removido: ");
    scanf("%d", &id);
    Cliente *anterior = NULL;
    Cliente *atual = lista->inicio;

    while (atual != NULL && atual->id != id)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual != NULL)
    {
        Pedido *pedidoAtual = atual->pedidos;
        while (pedidoAtual != NULL)
        {
            Pedido *pedidoRemover = pedidoAtual;
            pedidoAtual = pedidoAtual->prox;
            removeEntrega(filas, pedidoRemover->id, atual->endereco);
            free(pedidoRemover);
        }

        if (anterior == NULL)
        {
            lista->inicio = atual->prox;
        }
        else
        {
            anterior->prox = atual->prox;
        }
        free(atual);
        printf("Cliente removido com sucesso!\n");
    }
    else
    {
        printf("Cliente não encontrado!\n");
    }
}

void editaCliente(ListaClientes *lista)
{
    int id;
    printf("Digite o ID do cliente a ser editado: ");
    scanf("%d", &id);
    Cliente *atual = lista->inicio;
    while (atual != NULL && atual->id != id)
    {
        atual = atual->prox;
    }
    if (atual != NULL)
    {
        int opcao;
        printf("O que deseja editar?\n");
        printf("1. Nome\n");
        printf("2. Endereço\n");
        printf("3. CPF\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Digite o novo nome do cliente: ");
            scanf(" %[^\n]", atual->nome); // Corrigido para aceitar espaços
            printf("Nome atualizado com sucesso!\n");
            break;
        case 2:
            printf("Digite o novo endereço do cliente: ");
            scanf(" %[^\n]", atual->endereco); // Corrigido para aceitar espaços
            printf("Endereço atualizado com sucesso!\n");
            break;
        case 3:
        {
            char novoCPF[12];
            printf("Digite o novo CPF do cliente (apenas números, sem pontos ou traços): ");
            scanf("%s", novoCPF);
            int cpf_length = strlen(novoCPF);
            if (cpf_length < 4)
            {
                printf("CPF inválido! Deve conter pelo menos 4 dígitos.\n");
            }
            else
            {
                strcpy(atual->cpf, novoCPF);
                atual->id = atoi(&novoCPF[cpf_length - 4]);
                printf("CPF e ID atualizados com sucesso!\n");
            }
            break;
        }
        default:
            printf("Opção inválida!\n");
        }
    }
    else
    {
        printf("Cliente não encontrado!\n");
    }
}

void imprimeClientes(ListaClientes *lista)
{
    Cliente *atual = lista->inicio;
    while (atual != NULL)
    {
        printf("ID: %d, Nome: %s, Endereço: %s\n", atual->id, atual->nome, atual->endereco);
        imprimePedidos(atual);
        atual = atual->prox;
    }
}

Cliente *encontrarClientePorID(ListaClientes *lista, int id_cliente)
{
    Cliente *cliente = lista->inicio;
    while (cliente != NULL && cliente->id != id_cliente)
    {
        cliente = cliente->prox;
    }
    return cliente;
}

void adicionaPedido(ListaClientes *lista, FilaFilas *filas)
{
    int id_cliente;
    printf("Digite o ID do cliente: ");
    scanf("%d", &id_cliente);

    Cliente *cliente = encontrarClientePorID(lista, id_cliente);

    if (cliente == NULL)
    {
        printf("Cliente não encontrado. Adicionando novo cliente...\n");
        adicionaCliente(lista);
        cliente = encontrarClientePorID(lista, id_cliente);
    }

    Pedido *novo = (Pedido *)malloc(sizeof(Pedido));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para novo pedido.\n");
        return;
    }
    printf("Digite o ID do pedido: ");
    scanf("%d", &novo->id);
    printf("Digite a descrição do pedido: ");
    scanf(" %[^\n]", novo->descricao); // Corrigido para aceitar espaços

    strcpy(novo->status, "preparando");

    novo->prox = cliente->pedidos;
    cliente->pedidos = novo;

    printf("Pedido adicionado com sucesso!\n");
    printf("ID do Pedido: %d, Descrição: %s, Status: %s\n", novo->id, novo->descricao, novo->status);

    adicionaEntrega(filas, novo->id, cliente->endereco);
}

void removePedido(Cliente *cliente, FilaFilas *filas)
{
    int id;
    printf("Digite o ID do pedido a ser removido: ");
    scanf("%d", &id);
    Pedido *anterior = NULL;
    Pedido *atual = cliente->pedidos;

    while (atual != NULL && atual->id != id)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual != NULL)
    {
        if (anterior == NULL)
        {
            cliente->pedidos = atual->prox;
        }
        else
        {
            anterior->prox = atual->prox;
        }
        removeEntrega(filas, atual->id, cliente->endereco);
        free(atual);
        printf("Pedido removido com sucesso.\n");
    }
    else
    {
        printf("Pedido não encontrado!\n");
    }
}

void editaPedido(Cliente *cliente)
{
    int id;
    printf("Digite o ID do pedido a ser editado: ");
    scanf("%d", &id);

    Pedido *atual = cliente->pedidos;

    while (atual != NULL && atual->id != id)
    {
        atual = atual->prox;
    }

    if (atual == NULL)
    {
        printf("Pedido não encontrado.\n");
        return;
    }

    int opcao;
    do
    {
        printf("O que deseja editar?\n");
        printf("1. Descrição\n");
        printf("2. ID do Pedido\n");
        printf("3. Status\n");
        printf("0. Sair\n");
        printf("Digite sua opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Digite a nova descrição do pedido: ");
            scanf(" %[^\n]", atual->descricao); // Corrigido para aceitar espaços
            printf("Descrição atualizada com sucesso.\n");
            break;
        case 2:
        {
            int novo_id;
            printf("Digite o novo ID do pedido: ");
            scanf("%d", &novo_id);
            atual->id = novo_id;
            printf("ID do pedido atualizado com sucesso.\n");
            break;
        }
        case 3:
            printf("Digite o novo status do pedido: ");
            scanf(" %[^\n]", atual->status); // Corrigido para aceitar espaços
            printf("Status atualizado com sucesso.\n");
            break;
        case 0:
            printf("Saindo da edição de pedidos.\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void imprimePedidos(Cliente *cliente)
{
    Pedido *pedido = cliente->pedidos;
    printf("Pedidos do Cliente %s:\n", cliente->nome);
    while (pedido != NULL)
    {
        printf("ID: %d, Descrição: %s, Status: %s\n", pedido->id, pedido->descricao, pedido->status);
        pedido = pedido->prox;
    }
}

void atualizaStatusPedido(Cliente *cliente, int id, const char *novo_status)
{
    Pedido *pedido = cliente->pedidos;

    while (pedido != NULL && pedido->id != id)
    {
        pedido = pedido->prox;
    }

    if (pedido == NULL)
    {
        printf("Pedido não encontrado.\n");
        return;
    }

    strcpy(pedido->status, novo_status);
    printf("Status do pedido atualizado para '%s'.\n", pedido->status);
}

// Funções para a fila de filas (FilaFilas)
void inicializaFilaFilas(FilaFilas *filas)
{
    filas->inicio = NULL;
}

void adicionaEntrega(FilaFilas *filas, int id, const char *endereco)
{
    FilaPorEndereco *atual = filas->inicio;

    while (atual != NULL)
    {
        if (strcmp(atual->endereco, endereco) == 0)
        {
            Pedido *novoPedido = (Pedido *)malloc(sizeof(Pedido));
            if (novoPedido == NULL)
            {
                fprintf(stderr, "Erro ao alocar memória para nova entrega.\n");
                return;
            }
            novoPedido->id = id;
            novoPedido->prox = NULL;

            if (atual->inicio == NULL)
            {
                atual->inicio = novoPedido;
            }
            else
            {
                Pedido *temp = atual->inicio;
                while (temp->prox != NULL)
                {
                    temp = temp->prox;
                }
                temp->prox = novoPedido;
            }
            printf("Entrega adicionada na fila existente para o endereço %s.\n", endereco);
            return;
        }
        atual = atual->prox;
    }

    FilaPorEndereco *novaFila = (FilaPorEndereco *)malloc(sizeof(FilaPorEndereco));
    if (novaFila == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para nova fila de entregas.\n");
        return;
    }

    novaFila->inicio = NULL;
    novaFila->prox = filas->inicio;
    filas->inicio = novaFila;

    strcpy(novaFila->endereco, endereco);

    Pedido *novoPedido = (Pedido *)malloc(sizeof(Pedido));
    if (novoPedido == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para novo pedido de entrega.\n");
        return;
    }
    novoPedido->id = id;
    novoPedido->prox = NULL;

    novaFila->inicio = novoPedido;

    printf("Nova fila de entregas criada para o endereço %s.\n", endereco);
}

void removeEntrega(FilaFilas *filas, int id, const char *endereco)
{
    FilaPorEndereco *anterior = NULL;
    FilaPorEndereco *atual = filas->inicio;

    while (atual != NULL && strcmp(atual->endereco, endereco) != 0)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL)
    {
        printf("Fila de entregas não encontrada para o endereço %s.\n", endereco);
        return;
    }

    Pedido *pedidoAnterior = NULL;
    Pedido *pedidoAtual = atual->inicio;

    while (pedidoAtual != NULL && pedidoAtual->id != id)
    {
        pedidoAnterior = pedidoAtual;
        pedidoAtual = pedidoAtual->prox;
    }

    if (pedidoAtual == NULL)
    {
        printf("Pedido de entrega com ID %d não encontrado na fila para o endereço %s.\n", id, endereco);
        return;
    }

    if (pedidoAnterior == NULL)
    {
        atual->inicio = pedidoAtual->prox;
    }
    else
    {
        pedidoAnterior->prox = pedidoAtual->prox;
    }

    free(pedidoAtual);

    if (atual->inicio == NULL)
    {
        if (anterior == NULL)
        {
            filas->inicio = atual->prox;
        }
        else
        {
            anterior->prox = atual->prox;
        }
        free(atual);
        printf("Fila de entregas para o endereço %s foi removida pois não possui mais pedidos.\n", endereco);
    }
}

void imprimeFilaFilas(FilaFilas *filas)
{
    FilaPorEndereco *atual = filas->inicio;
    while (atual != NULL)
    {
        printf("Endereço: %s\n", atual->endereco);
        Pedido *pedido = atual->inicio;
        while (pedido != NULL)
        {
            printf("\tID do Pedido: %d\n", pedido->id);
            pedido = pedido->prox;
        }
        atual = atual->prox;
    }
}

// Função para criar uma nova entrega com base nos pedidos prontos para envio
void processaPedidosProntos(ListaClientes *lista, FilaFilas *filas)
{
    Cliente *clienteAtual = lista->inicio;
    while (clienteAtual != NULL)
    {
        Pedido *pedidoAtual = clienteAtual->pedidos;
        while (pedidoAtual != NULL)
        {
            if (strcmp(pedidoAtual->status, "pronto para envio") == 0)
            {
                adicionaEntrega(filas, pedidoAtual->id, clienteAtual->endereco);
            }
            pedidoAtual = pedidoAtual->prox;
        }
        clienteAtual = clienteAtual->prox;
    }
}
