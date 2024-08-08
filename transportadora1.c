#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "transportadora.h"

// Funções para a lista de clientes
void inicializaListaClientes(ListaClientes *lista)
{
    lista->inicio = NULL;
}

// Funções para a fila de filas (FilaFilas)
void inicializaFilaFilas(FilaFilas *filas)
{
    filas->inicio = NULL;
}

void adicionaCliente(ListaClientes *lista)
{
    char numero[6];

    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para novo cliente.\n");
        return;
    }

    printf("\nDigite o CPF do cliente (apenas numeros): ");
    scanf("%s", novo->cpf);

    int cpf_length = strlen(novo->cpf);
    if (cpf_length != 11)
    {
        printf("CPF invalido! Deve conter 11 dígitos.\n");
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
    formatarString(novo->nome);   // Formata o nome

    printf("Digite o endereco do cliente: ");
    
    scanf(" %[^\n]", novo->endereco); // Corrigido para aceitar espaços
    formatarString(novo->endereco);   // Formata o endereço

    printf("Numero:");
    scanf(" %[^\n]", numero);
    strncat(novo->endereco, ", ", sizeof(novo->endereco) - strlen(novo->endereco) - 1);
    strncat(novo->endereco, numero, sizeof(novo->endereco) - strlen(novo->endereco) - 1);

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
        printf("Cliente nao encontrado!\n");
    }
}

// Função para atualizar o endereço nas filas de entregas
void atualizaEnderecoFilas(FilaFilas *filas, Cliente *cliente, const char *novo_endereco)
{
    Pedido *pedidoAtual = cliente->pedidos;
    while (pedidoAtual != NULL)
    {
        // Remove o pedido da fila antiga
        removeEntrega(filas, pedidoAtual->id, cliente->endereco);

        // Adiciona o pedido à nova fila
        adicionaEntrega(filas, pedidoAtual->id, novo_endereco);

        pedidoAtual = pedidoAtual->prox;
    }
}

// Função para editar cliente
void editaCliente(ListaClientes *lista, FilaFilas *filas)
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
            formatarString(atual->nome);   // Formata o endereço
            printf("Nome atualizado com sucesso!\n");
            break;
        case 2:
        {
            char novoEndereco[100], numero[100];
            printf("Digite o novo endereço do cliente: ");
            printf("Endereco:");
            scanf(" %[^\n]", novoEndereco); // Corrigido para aceitar espaços
            formatarString(novoEndereco);   // Formata o endereço

            printf("Numero:");
            scanf(" %[^\n]", numero);
            strncat(novoEndereco, ", ", sizeof(novoEndereco) - strlen(novoEndereco) - 1);
            strncat(novoEndereco, numero, sizeof(novoEndereco) - strlen(novoEndereco) - 1);
            printf("Endereço atualizado com sucesso!\n");

            atualizaEnderecoFilas(filas, atual, novoEndereco);
            strcpy(atual->endereco, novoEndereco);
            printf("Endereço atualizado com sucesso!\n");
            break;
        }
        case 3:
        {
            char novoCPF[12];
            printf("Digite o novo CPF do cliente (apenas numeros): ");
            scanf("%s", novoCPF);

            int cpf_length = strlen(novoCPF);
            if (cpf_length != 11)
            {
                printf("CPF invalido! Deve conter 11 dígitos.\n");                
                return;
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

void adicionaPedido(ListaClientes *lista)
{
    int id_cliente;
    printf("Digite o ID do cliente: ");
    scanf("%d", &id_cliente);

    Cliente *cliente = encontrarClientePorID(lista, id_cliente);

    if (cliente == NULL)
    {
        printf("Cliente nao encontrado. Adicionando novo cliente...\n");
        adicionaCliente(lista);
        cliente = encontrarClientePorID(lista, id_cliente);
    }

    Pedido *novo = (Pedido *)malloc(sizeof(Pedido));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para novo pedido.\n");
        return;
    }
    printf("Digite o ID do pedido: ");
    scanf("%d", &novo->id);
    printf("Digite a descricao do pedido: ");
    gets(novo->descricao); // Corrigido para aceitar espaços
    getchar();

    strcpy(novo->status, "preparando");

    novo->prox = cliente->pedidos;
    cliente->pedidos = novo;

    printf("Pedido adicionado com sucesso!\n");
    printf("ID do Pedido: %d, Descricao: %s, Status: %s\n", novo->id, novo->descricao, novo->status);
}

// Função para despachar um pedido e adicionar à lista de entregas
void despacharPedido(ListaClientes *lista, FilaFilas *filas)
{
    int id_cliente, id_pedido;
    printf("Digite o ID do cliente: ");
    scanf("%d", &id_cliente);

    Cliente *cliente = encontrarClientePorID(lista, id_cliente);

    if (cliente == NULL)
    {
        printf("Cliente nao encontrado.\n");
        return;
    }

    printf("Digite o ID do pedido a ser despachado: ");
    scanf("%d", &id_pedido);

    Pedido *pedido = cliente->pedidos;
    while (pedido != NULL && pedido->id != id_pedido)
    {
        pedido = pedido->prox;
    }

    if (pedido == NULL)
    {
        printf("Pedido nao encontrado.\n");
        return;
    }

    // Atualiza o status do pedido para 'despachado'
    strcpy(pedido->status, "despachado");
    printf("Pedido com ID %d despachado com sucesso.\n", id_pedido);

    // Adiciona o pedido à lista de entregas
    adicionaEntrega(filas, pedido->id, cliente->endereco);
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
        printf("Pedido nao encontrado!\n");
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
        printf("Pedido nao encontrado.\n");
        return;
    }

    int opcao;
    do
    {
        printf("O que deseja editar?\n");
        printf("1. Descricao\n");
        printf("2. ID do Pedido\n");
        printf("0. Sair\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Digite a nova descricao do pedido: ");
            scanf(" %[^\n]", atual->descricao); // Corrigido para aceitar espaços
            printf("Descricao atualizada com sucesso.\n");
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
        case 0:
            printf("Saindo da edicao de pedidos.\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

void imprimePedidos(Cliente *cliente)
{
    Pedido *pedido = cliente->pedidos;
    printf("Pedidos do Cliente %s:\n", cliente->nome);
    while (pedido != NULL)
    {
        printf("ID: %d, Descricao: %s, Status: %s\n", pedido->id, pedido->descricao, pedido->status);
        pedido = pedido->prox;
    }
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
                fprintf(stderr, "Erro ao alocar memoria para nova entrega.\n");
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
            printf("Entrega adicionada na fila existente para o endereco %s.\n", endereco);
            return;
        }
        atual = atual->prox;
    }

    FilaPorEndereco *novaFila = (FilaPorEndereco *)malloc(sizeof(FilaPorEndereco));
    if (novaFila == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para nova fila de entregas.\n");
        return;
    }

    novaFila->inicio = NULL;
    novaFila->prox = filas->inicio;
    filas->inicio = novaFila;

    strcpy(novaFila->endereco, endereco);

    Pedido *novoPedido = (Pedido *)malloc(sizeof(Pedido));
    if (novoPedido == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para novo pedido de entrega.\n");
        return;
    }
    novoPedido->id = id;
    novoPedido->prox = NULL;

    novaFila->inicio = novoPedido;

    printf("Nova fila de entregas criada para o endereco %s.\n", endereco);
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
        printf("Fila de entregas nao encontrada para o endereco %s.\n", endereco);
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
        printf("Pedido de entrega com ID %d nao encontrado na fila para o endereco %s.\n", id, endereco);
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
        printf("Fila de entregas para o endereco %s foi removida pois nao possui mais pedidos.\n", endereco);
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

void inicializaPilhaNaoEfetuada(PilhaNaoEfetuada **pilha)
{
    *pilha = NULL;
}

void inicializaPilhaDevolucao(PilhaDevolucao **pilha)
{
    *pilha = NULL;
}

void adicionaPilhaNaoEfetuada(PilhaNaoEfetuada **pilha, Pedido *pedido)
{
    PilhaNaoEfetuada *novo = (PilhaNaoEfetuada *)malloc(sizeof(PilhaNaoEfetuada));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para a pilha de nao efetuadas.\n");
        return;
    }
    novo->pedido = pedido;
    novo->prox = *pilha;
    *pilha = novo;
}

void adicionaPilhaDevolucao(PilhaDevolucao **pilha, Pedido *pedido)
{
    PilhaDevolucao *novo = (PilhaDevolucao *)malloc(sizeof(PilhaDevolucao));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para a pilha de devolucao.\n");
        return;
    }
    novo->pedido = pedido;
    novo->prox = *pilha;
    *pilha = novo;
}

void concluirEntrega(FilaFilas *filas, PilhaNaoEfetuada **pilhaNaoEfetuada, PilhaDevolucao **pilhaDevolucao, int *pontos)
{
    if (filas->inicio == NULL)
    {
        printf("Nao ha filas de entregas para processar.\n");
        return;
    }

    FilaPorEndereco *filaAtual = filas->inicio;
    while (filaAtual != NULL)
    {
        // Imprime o endereço e IDs dos pedidos na fila atual
        printf("Endereco: %s\n", filaAtual->endereco);
        Pedido *pedidoAtual = filaAtual->inicio;
        if (pedidoAtual == NULL)
        {
            printf("Nenhum pedido para o endereco %s.\n", filaAtual->endereco);
            filaAtual = filaAtual->prox;
            continue;
        }

        printf("Pedidos a serem entregues:\n");
        while (pedidoAtual != NULL)
        {
            printf("ID do Pedido: %d\n", pedidoAtual->id);
            pedidoAtual = pedidoAtual->prox;
        }

        // Processa cada pedido na fila
        pedidoAtual = filaAtual->inicio;
        Pedido *pedidoParaRemover;
        while (pedidoAtual != NULL)
        {
            int id;
            printf("Digite o ID do pedido entregue (separado por espaço, finalize com -1): ");
            scanf("%d", &id);

            if (id == -1)
            {
                break;
            }

            if (pedidoAtual->id == id)
            {
                // Remove o pedido da fila de entregas
                removeEntrega(filas, id, filaAtual->endereco);

                // Adiciona pontos pela entrega
                (*pontos) += 5;
            }
            else
            {
                // Adiciona à pilha de não efetuadas
                pedidoParaRemover = pedidoAtual;
                pedidoAtual = pedidoAtual->prox;
                adicionaPilhaNaoEfetuada(pilhaNaoEfetuada, pedidoParaRemover);
                continue;
            }

            pedidoAtual = pedidoAtual->prox;
        }

        // Adiciona os pedidos não entregues para a pilha de devolução
        while (*pilhaNaoEfetuada != NULL)
        {
            PilhaNaoEfetuada *pedidoNaoEfetuado = *pilhaNaoEfetuada;
            *pilhaNaoEfetuada = pedidoNaoEfetuado->prox;
            adicionaPilhaDevolucao(pilhaDevolucao, pedidoNaoEfetuado->pedido);
            free(pedidoNaoEfetuado);
            (*pontos) -= 1;
        }

        // Avança para a próxima fila de entrega
        filaAtual = filaAtual->prox;
    }

    // Tenta entregar pedidos na pilha de não efetuadas
    processaPilhaNaoEfetuada(filas, pilhaNaoEfetuada, pilhaDevolucao, pontos);
}

void processaPilhaNaoEfetuada(FilaFilas *filas, PilhaNaoEfetuada **pilhaNaoEfetuada, PilhaDevolucao **pilhaDevolucao, int *pontos)
{
    while (*pilhaNaoEfetuada != NULL)
    {
        PilhaNaoEfetuada *pedidoNaoEfetuado = *pilhaNaoEfetuada;
        *pilhaNaoEfetuada = pedidoNaoEfetuado->prox;

        Pedido *pedido = pedidoNaoEfetuado->pedido;
        printf("Reprocessando pedido ID %d\n", pedido->id);

        // Adiciona à lista de entregas novamente
        adicionaEntrega(filas, pedido->id, "endereco temporario");

        // Adiciona à pilha de devolução se ainda não for possível entregar
        adicionaPilhaDevolucao(pilhaDevolucao, pedido);
        free(pedidoNaoEfetuado);
        (*pontos) -= 1;
    }
    printf("Pilha de nao efetuadas processada. Pontos atuais: %d\n", *pontos);
}
void processaPilhaDevolucao(PilhaDevolucao **pilhaDevolucao)
{
    while (*pilhaDevolucao != NULL)
    {
        PilhaDevolucao *pedidoDevolucao = *pilhaDevolucao;
        *pilhaDevolucao = pedidoDevolucao->prox;

        printf("Pedido ID %d devolvido e removido.\n", pedidoDevolucao->pedido->id);
        free(pedidoDevolucao->pedido);
        free(pedidoDevolucao);
    }
}

void imprimePilhaNaoEfetuada(PilhaNaoEfetuada *pilha)
{
    if (pilha == NULL)
    {
        printf("A pilha de pedidos nao efetuados esta vazia.\n");
        return;
    }

    printf("Pedidos não efetuados:\n");
    PilhaNaoEfetuada *atual = pilha;
    while (atual != NULL)
    {
        printf("ID do Pedido: %d\n", atual->pedido->id);
        printf("Descrição do Pedido: %s\n", atual->pedido->descricao);
        printf("Status do Pedido: %s\n", atual->pedido->status);
        atual = atual->prox;
    }
}

void imprimePilhaDevolucao(PilhaDevolucao *pilha)
{
    if (pilha == NULL)
    {
        printf("A pilha de devoluções está vazia.\n");
        return;
    }

    printf("Pedidos devolvidos:\n");
    PilhaDevolucao *atual = pilha;
    while (atual != NULL)
    {
        printf("ID do Pedido: %d\n", atual->pedido->id);
        printf("Descrição do Pedido: %s\n", atual->pedido->descricao);
        printf("Status do Pedido: %s\n", atual->pedido->status);
        atual = atual->prox;
    }
}
void formatarString(char *str)
{
    int capitalize = 1; // Indica se a próxima letra deve ser maiúscula

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isspace((unsigned char)str[i]))
        {
            capitalize = 1;
        }
        else if (capitalize)
        {
            str[i] = toupper((unsigned char)str[i]);
            capitalize = 0;
        }
        else
        {
            str[i] = tolower((unsigned char)str[i]);
        }
    }
}