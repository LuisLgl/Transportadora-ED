#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

int verificaCpfRepetido(Cliente *listaClientes, const char *cpf)
{
    Cliente *atual = listaClientes;
    while (atual != NULL)
    {
        if (strcmp(atual->cpf, cpf) == 0)
        {
            return 1; // CPF já existe
        }
        atual = atual->prox;
    }
    return 0; // CPF não encontrado
}

int verificaIdRepetido(Cliente *listaClientes, int id)
{
    Cliente *atual = listaClientes;
    while (atual != NULL)
    {
        if (atual->id == id)
        {
            return 1; // ID já existe
        }
        atual = atual->prox;
    }
    return 0; // ID não encontrado
}

void adicionaCliente(ListaClientes *lista)
{
    char numero[6];
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo == NULL)
    {
        fprintf(stderr, "\nErro ao alocar memoria para novo cliente.\n");
        return;
    }

    printf("\nDigite o CPF do cliente (apenas numeros): ");
    scanf("%s", novo->cpf);

    if (verificaCpfRepetido(lista->inicio, novo->cpf))
    {
        printf("\nErro: O CPF %s ja esta cadastrado.\n", novo->cpf);
        free(novo);
        return;
    }

    int cpf_length = strlen(novo->cpf);
    if (cpf_length != 11)
    {
        printf("\nCPF invalido! Deve conter 11 dígitos.\n");
        free(novo);
        return;
    }

    // Extrai os últimos 4 dígitos do CPF para definir o ID
    char cpf_ultimos_4[5];
    strncpy(cpf_ultimos_4, novo->cpf + cpf_length - 4, 4);
    cpf_ultimos_4[4] = '\0';
    novo->id = atoi(cpf_ultimos_4);

    // Se o ID for repetido, adiciona um número aleatório a ele
    while (verificaIdRepetido(lista->inicio, novo->id))
    {
        novo->id += rand() % 1000; // Adiciona um número aleatório entre 0 e 999
    }

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", novo->nome);
    formatarString(novo->nome);

    printf("Digite o endereco do cliente: ");
    scanf(" %[^\n]", novo->endereco);
    formatarString(novo->endereco);

    printf("Numero:");
    scanf(" %[^\n]", numero);
    strncat(novo->endereco, ", ", sizeof(novo->endereco) - strlen(novo->endereco) - 1);
    strncat(novo->endereco, numero, sizeof(novo->endereco) - strlen(novo->endereco) - 1);

    novo->pedidos = NULL;
    novo->prox = NULL;

    // Adiciona o novo cliente ao final da lista
    if (lista->inicio == NULL)
    {
        // Lista vazia, o novo cliente será o primeiro
        lista->inicio = novo;
    }
    else
    {
        // Percorre até o final da lista
        Cliente *atual = lista->inicio;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        // Adiciona o novo cliente no final da lista
        atual->prox = novo;
    }

    printf("\nCliente adicionado com sucesso!\n");
    printf("ID: %d, CPF: %s, Nome: %s, Endereco: %s\n", novo->id, novo->cpf, novo->nome, novo->endereco);
}

void removeCliente(ListaClientes *lista, FilaFilas *filas)
{
    int id;
    printf("\nDigite o ID do cliente a ser removido: ");
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
        printf("\nCliente removido com sucesso!\n");
    }
    else
    {
        printf("\nCliente nao encontrado!\n");
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
    printf("\nDigite o ID do cliente a ser editado: ");
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
        printf("2. Endereco\n");
        printf("3. CPF\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\nDigite o novo nome do cliente: ");
            scanf(" %[^\n]", atual->nome); // Corrigido para aceitar espaços
            formatarString(atual->nome);   // Formata o endereço
            printf("Nome atualizado com sucesso!\n");
            break;
        case 2:
        {
            char novoEndereco[100], numero[100];
            printf("\nDigite o novo endereço do cliente: ");
            printf("Endereco:");
            scanf(" %[^\n]", novoEndereco); // Corrigido para aceitar espaços
            formatarString(novoEndereco);   // Formata o endereço

            printf("Numero:");
            scanf(" %[^\n]", numero);
            strncat(novoEndereco, ", ", sizeof(novoEndereco) - strlen(novoEndereco) - 1);
            strncat(novoEndereco, numero, sizeof(novoEndereco) - strlen(novoEndereco) - 1);

            atualizaEnderecoFilas(filas, atual, novoEndereco);
            strcpy(atual->endereco, novoEndereco);
            printf("Endereco atualizado com sucesso!\n");
            break;
        }
        case 3:
        {
            char novoCPF[12];
            printf("\nDigite o novo CPF do cliente (apenas numeros): ");
            scanf("%s", novoCPF);

            int cpf_length = strlen(novoCPF);
            if (cpf_length != 11)
            {
                printf("CPF invalido! Deve conter 11 digitos.\n");
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
            printf("Opcao invalida!\n");
        }
    }
    else
    {
        printf("\nCliente nao encontrado!\n");
    }
}

void imprimeClientes(ListaClientes *lista)
{
    Cliente *atual = lista->inicio;
    if (atual == NULL)
    {
        printf("\nNenhum cliente cadastrado!\n");
    }
    else
    {
        while (atual != NULL)
        {
            printf("\nID: %d, Nome: %s, Endereco: %s\n", atual->id, atual->nome, atual->endereco);
            imprimePedidos(atual);
            atual = atual->prox;
        }
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

int verificaIDPedidoRepetido(Pedido *listaPedidos, int id_pedido)
{
    Pedido *atual = listaPedidos;
    while (atual != NULL)
    {
        if (atual->id == id_pedido)
        {
            return 1; // ID já existe
        }
        atual = atual->prox;
    }
    return 0; // ID não encontrado
}

void adicionaPedido(ListaClientes *lista)
{
    int id_cliente;
    printf("\nDigite o ID do cliente: ");
    scanf("%d", &id_cliente);

    Cliente *cliente = encontrarClientePorID(lista, id_cliente);

    if (cliente == NULL)
    {
        printf("Cliente nao encontrado.\n");
        return;
    }

    Pedido *novo = (Pedido *)malloc(sizeof(Pedido));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para novo pedido.\n");
        return;
    }

    int id_pedido;
    printf("Digite o ID do pedido: ");
    scanf("%d", &id_pedido);

    if (verificaIDPedidoRepetido(cliente->pedidos, id_pedido))
    {
        printf("Erro: O ID do pedido %d ja existe para este cliente.\n", id_pedido);
        free(novo);
        return;
    }

    novo->id = id_pedido;
    printf("Digite a descricao do pedido: ");
    scanf(" %[^\n]", novo->descricao); // Corrigido para aceitar espaços

    strcpy(novo->status, "preparando");

    novo->prox = cliente->pedidos;
    cliente->pedidos = novo;

    printf("\nPedido adicionado com sucesso!\n");
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
    printf("\nPedido com ID %d despachado com sucesso.\n", id_pedido);

    // Adiciona o pedido à lista de entregas
    adicionaEntrega(filas, pedido->id, cliente->endereco);
}

void removePedido(Cliente *cliente, FilaFilas *filas)
{
    int id;
    printf("\nDigite o ID do pedido a ser removido: ");
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
        printf("\nPedido removido com sucesso.\n");
    }
    else
    {
        printf("\nPedido nao encontrado!\n");
    }
}

void editaPedido(Cliente *cliente)
{
    int id;
    printf("\nDigite o ID do pedido a ser editado: ");
    scanf("%d", &id);

    Pedido *atual = cliente->pedidos;

    while (atual != NULL && atual->id != id)
    {
        atual = atual->prox;
    }

    if (atual == NULL)
    {
        printf("\nPedido não encontrado.\n");
        return;
    }

    // Verifica se o status do pedido é "preparando"
    if (strcmp(atual->status, "preparando") != 0)
    {
        printf("\nPedido não pode ser editado. Status atual: %s\n", atual->status);
        return;
    }

    int opcao;
    do
    {
        printf("\nO que deseja editar?\n");
        printf("1. Descrição\n");
        printf("2. ID do Pedido\n");
        printf("0. Sair\n");
        printf("Digite sua opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\nDigite a nova descrição do pedido: ");
            scanf(" %[^\n]", atual->descricao); // Corrigido para aceitar espaços
            printf("Descrição atualizada com sucesso.\n");
            break;
        case 2:
        {
            int novo_id;
            printf("\nDigite o novo ID do pedido: ");
            scanf("%d", &novo_id);
            atual->id = novo_id;
            printf("ID do pedido atualizado com sucesso.\n");
            break;
        }
        case 0:
            printf("\nSaindo da edição de pedidos.\n");
            break;
        default:
            printf("\nOpção inválida!\n");
        }
    } while (opcao != 0);
}

void imprimePedidos(Cliente *cliente)
{
    Pedido *pedido = cliente->pedidos;
    if (pedido == NULL)
        printf("Sem pedidos cadastrados!\n");
    else
    {
        printf("\nPedidos do Cliente %s:\n", cliente->nome);
        while (pedido != NULL)
        {
            printf("ID: %d, Descricao: %s, Status: %s\n", pedido->id, pedido->descricao, pedido->status);
            pedido = pedido->prox;
        }
    }
}

void adicionaFilaNoFinal(FilaFilas *filas, FilaPorEndereco *novaFila)
{
    if (filas->inicio == NULL)
    {
        filas->inicio = novaFila;
    }
    else
    {
        FilaPorEndereco *atual = filas->inicio;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        atual->prox = novaFila;
    }
}

void adicionaEntrega(FilaFilas *filas, int id, const char *endereco)
{
    FilaPorEndereco *atual = filas->inicio;

    // Procura por uma fila existente para o endereço
    while (atual != NULL)
    {
        if (strcmp(atual->endereco, endereco) == 0)
        {
            // Adiciona um novo pedido ao final da fila existente
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

    // Cria uma nova fila para o endereço
    FilaPorEndereco *novaFila = (FilaPorEndereco *)malloc(sizeof(FilaPorEndereco));
    if (novaFila == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para nova fila de entregas.\n");
        return;
    }

    novaFila->inicio = NULL;
    novaFila->prox = NULL;
    strcpy(novaFila->endereco, endereco);

    Pedido *novoPedido = (Pedido *)malloc(sizeof(Pedido));
    if (novoPedido == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para novo pedido de entrega.\n");
        free(novaFila); // Liberar memória alocada para novaFila em caso de erro
        return;
    }
    novoPedido->id = id;
    novoPedido->prox = NULL;

    novaFila->inicio = novoPedido;

    // Adiciona a nova fila no final da lista de filas
    adicionaFilaNoFinal(filas, novaFila);

    printf("Nova fila de entregas criada para o endereco %s.\n", endereco);
}

void removeEntrega(FilaFilas *filas, int id, const char *endereco)
{
    FilaPorEndereco *anterior = NULL;
    FilaPorEndereco *atual = filas->inicio;

    // Procura pela fila associada ao endereço fornecido
    while (atual != NULL && strcmp(atual->endereco, endereco) != 0)
    {
        anterior = atual;
        atual = atual->prox;
    }

    // Se a fila não foi encontrada
    if (atual == NULL)
    {
        printf("Fila de entregas nao encontrada para o endereco %s.\n", endereco);
        return;
    }

    Pedido *pedidoAnterior = NULL;
    Pedido *pedidoAtual = atual->inicio;

    // Procura pelo pedido com o ID fornecido
    while (pedidoAtual != NULL && pedidoAtual->id != id)
    {
        pedidoAnterior = pedidoAtual;
        pedidoAtual = pedidoAtual->prox;
    }

    // Se o pedido não foi encontrado
    if (pedidoAtual == NULL)
    {
        printf("Pedido de entrega com ID %d nao encontrado na fila para o endereco %s.\n", id, endereco);
        return;
    }

    // Remove o pedido da lista de pedidos
    if (pedidoAnterior == NULL)
    {
        atual->inicio = pedidoAtual->prox;
    }
    else
    {
        pedidoAnterior->prox = pedidoAtual->prox;
    }

    free(pedidoAtual);

    // Se a fila está vazia após a remoção do pedido
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
        
        printf("Fila de entregas para o endereco %s foi removida pois nao possui mais pedidos.\n", endereco);
    }
}

void imprimeFilaFilas(FilaFilas *filas)
{
    if (filas->inicio == NULL)
    {
        printf("\nNenhuma entrega na fila.\n");
        return;
    }

    FilaPorEndereco *atual = filas->inicio;
    while (atual != NULL)
    {
        printf("\nEndereco: %s\n", atual->endereco);
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

void inicializaFilaDevolucao(FilaDevolucao **filadevolucao)
{
    *filadevolucao = NULL;
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

void adicionaFilaDevolucao(FilaDevolucao **fila, Pedido *pedido)
{
    if (pedido == NULL)
    {
        fprintf(stderr, "Pedido invalido.\n");
        return;
    }

    // Atualiza o status do pedido para "devolvido"
    snprintf(pedido->status, sizeof(pedido->status), "devolvido");

    FilaDevolucao *novo = (FilaDevolucao *)malloc(sizeof(FilaDevolucao));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memoria para a fila de devolucao.\n");
        return;
    }
    novo->pedido = pedido;
    novo->prox = NULL;

    if (*fila == NULL)
    {
        // Se a fila estiver vazia, o novo elemento será o primeiro
        *fila = novo;
    }
    else
    {
        // Caso contrário, percorre a fila até o último elemento e adiciona o novo elemento no final
        FilaDevolucao *temp = *fila;
        while (temp->prox != NULL)
        {
            temp = temp->prox;
        }
        temp->prox = novo;
    }
}

void concluirEntrega(FilaFilas *filas, PilhaNaoEfetuada **pilhaNaoEfetuada, FilaDevolucao **filadevolucao, int *pontos)
{    
    if (filas->inicio == NULL)
    {
        printf("\nNao ha filas de entregas para processar.\n");
        return;
    }

    FilaPorEndereco *filaAtual = filas->inicio;
    while (filaAtual != NULL)
    {
        printf("\nEndereco: %s\n", filaAtual->endereco);
        Pedido *pedidoAtual = filaAtual->inicio;

        if (pedidoAtual == NULL)
        {
            printf("\nNenhum pedido para o endereco %s.\n", filaAtual->endereco);
            filaAtual = filaAtual->prox;
            continue;
        }

        Pedido *pedidoParaRemover;

        // Usar um loop para evitar alterar a lista durante a iteração
        while (pedidoAtual != NULL)
        {
            Pedido *proximoPedido = pedidoAtual->prox; // Salvar o próximo pedido

            int sorteio = rand() % 100;
            if (sorteio < 20)
            {
                printf("Entrega do pedido %d realizada com sucesso.\n", pedidoAtual->id);
                 (*pontos) += 5;
                strcpy(pedidoAtual->status, "entregue"); // Marcar como entregue

                pedidoParaRemover = pedidoAtual;
                //removeEntrega(filas, pedidoParaRemover->id, filaAtual->endereco);
            }
            else
            {
                printf("Entrega do pedido %d nao realizada. Adicionando a pilha de nao efetuadas.\n", pedidoAtual->id);
                pedidoParaRemover = pedidoAtual;
                adicionaPilhaNaoEfetuada(pilhaNaoEfetuada, pedidoParaRemover);
                removeEntrega(filas, pedidoParaRemover->id, filaAtual->endereco);
            }

            pedidoAtual = proximoPedido; // Avançar para o próximo pedido
        }

       

        filaAtual = filaAtual->prox;
    }
    printf("\nFila de entregas processada. Pontos atuais: %d\n", *pontos);

    // processaPilhaNaoEfetuada(filas, pilhaNaoEfetuada, pilhaDevolucao, pontos);
    processaPilhaNaoEfetuada(pilhaNaoEfetuada, filadevolucao, pontos);
}

void processaPilhaNaoEfetuada(PilhaNaoEfetuada **pilhaNaoEfetuada, FilaDevolucao **filaDevolucao, int *pontos)
{
    while (*pilhaNaoEfetuada != NULL)
    {
        PilhaNaoEfetuada *pedidoNaoEfetuado = *pilhaNaoEfetuada;
        *pilhaNaoEfetuada = pedidoNaoEfetuado->prox;

        Pedido *pedido = pedidoNaoEfetuado->pedido;
        printf("\nReprocessando pedido ID %d\n", pedido->id);

        int sorteio = rand() % 100;
        if (sorteio < 20)
        {
            printf("Reentrega do pedido %d realizada com sucesso.\n", pedido->id);
            strcpy(pedido->status, "entregue"); // Marcar como entregue

            (*pontos) += 3;
            free(pedido); // Remover pedido da pilha de não efetuadas
        }
        else
        {
            printf("Reentrega do pedido %d falhou. Enviando para devolucao.\n", pedido->id);
            (*pontos) -= 1;
            adicionaFilaDevolucao(filaDevolucao, pedido);
        }

        free(pedidoNaoEfetuado);
    }
    printf("\nPilha de nao efetuadas processada. Pontos atuais: %d\n", *pontos);
}

void processaFilaDevolucao(FilaDevolucao **filaDevolucao)
{
    FilaDevolucao *atual = *filaDevolucao;
    while (atual != NULL)
    {
        Pedido *pedido = atual->pedido;
        printf("Processando devolucao do pedido ID %d\n", pedido->id);

        // Mudar o status do pedido para "devolvido"
        strcpy(pedido->status, "devolvido");

        // Exibir status atualizado
        printf("Pedido ID %d marcado como devolvido.\n", pedido->id);

        // Avançar para o próximo pedido na fila
        atual = atual->prox;
    }
}

void imprimeFilaDevolucao(FilaDevolucao *fila)
{
    if (fila == NULL)
    {
        printf("\nA fila de devolucoes esta vazia.\n");
        return;
    }

    printf("\nPedidos devolvidos:\n");
    FilaDevolucao *atual = fila;
    while (atual != NULL)
    {
        printf("\nID do Pedido: %d\n", atual->pedido->id);
        printf("Status do Pedido: %s\n", atual->pedido->status);
        atual = atual->prox;
    }
}

void imprimePilhaNaoEfetuada(PilhaNaoEfetuada *pilha)
{
    if (pilha == NULL)
    {
        printf("A pilha de pedidos nao efetuados esta vazia.\n");
        return;
    }

    printf("Pedidos nao efetuados:\n");
    PilhaNaoEfetuada *atual = pilha;
    while (atual != NULL)
    {
        printf("ID do Pedido: %d\n", atual->pedido->id);
        printf("Descricao do Pedido: %s\n", atual->pedido->descricao);
        printf("Status do Pedido: %s\n", atual->pedido->status);
        atual = atual->prox;
    }
}

void formatarString(char *str)
{
    int capitalize = 1; // Indica se a próxima letra deve ser maiúscula
    int len = strlen(str);

    // Remover todos os espaços em branco finais
    while (len > 0 && isspace((unsigned char)str[len - 1]))
    {
        str[len - 1] = '\0';
        len--; // Ajusta o comprimento da string
    }

    for (int i = 0; i < len; i++)
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

void despacharTudo(ListaClientes *lista, FilaFilas *filas)
{
    if (lista == NULL || filas == NULL)
    {
        printf("\nErro: Lista de clientes ou fila de entregas é nula.\n");
        return;
    }

    Cliente *clienteAtual = lista->inicio;
    if (clienteAtual == NULL)
    {
        printf("\nNenhum cliente encontrado na lista.\n");
        return;
    }

    // Percorre todos os clientes
    while (clienteAtual != NULL)
    {
        printf("\nProcessando cliente ID %d.\n", clienteAtual->id);

        Pedido *pedidoAtual = clienteAtual->pedidos;
        if (pedidoAtual == NULL)
        {
            printf("Nenhum pedido encontrado para o cliente ID %d.\n", clienteAtual->id);
        }

        // Percorre todos os pedidos do cliente
        while (pedidoAtual != NULL)
        {
            if (strcmp(pedidoAtual->status, "preparando") == 0) // Verifica se o pedido está em "preparando"
            {
                // Atualiza o status do pedido para 'despachado'
                strcpy(pedidoAtual->status, "despachado");
                printf("Pedido com ID %d do cliente %d despachado com sucesso.\n",
                       pedidoAtual->id, clienteAtual->id);

                // Adiciona o pedido à fila de entregas, mantendo a ordem FIFO
                adicionaEntrega(filas, pedidoAtual->id, clienteAtual->endereco);
            }
            else
            {
                printf("Pedido com ID %d já despachado ou entregue. Status atual: %s\n",
                       pedidoAtual->id, pedidoAtual->status);
            }

            pedidoAtual = pedidoAtual->prox; // Move para o próximo pedido
        }

        clienteAtual = clienteAtual->prox; // Move para o próximo cliente
    }

    printf("\nDespacho completo.\n");
}

void limparMemoria(ListaClientes *listaClientes, FilaFilas *filaFilas, PilhaNaoEfetuada **pilhaNaoEfetuada, FilaDevolucao **filaDevolucao)
{
    // Limpar lista de clientes
    Cliente *atualCliente = listaClientes->inicio;
    while (atualCliente != NULL)
    {
        Cliente *clienteRemover = atualCliente;
        atualCliente = atualCliente->prox;

        // Limpar pedidos do cliente
        Pedido *atualPedido = clienteRemover->pedidos;
        while (atualPedido != NULL)
        {
            Pedido *pedidoRemover = atualPedido;
            atualPedido = atualPedido->prox;
            free(pedidoRemover);
        }

        free(clienteRemover);
    }

    // Limpar fila de filas
    FilaPorEndereco *atualFila = filaFilas->inicio;
    while (atualFila != NULL)
    {
        FilaPorEndereco *filaRemover = atualFila;
        atualFila = atualFila->prox;

        // Limpar pedidos na fila
        Pedido *atualPedido = filaRemover->inicio;
        while (atualPedido != NULL)
        {
            Pedido *pedidoRemover = atualPedido;
            atualPedido = atualPedido->prox;
            free(pedidoRemover);
        }

        free(filaRemover);
    }

    // Limpar pilha de não efetuadas
    PilhaNaoEfetuada *atualPilha = *pilhaNaoEfetuada;
    while (atualPilha != NULL)
    {
        PilhaNaoEfetuada *topo = atualPilha;
        atualPilha = atualPilha->prox;

        // Limpar pedidos na pilha
        Pedido *atualPedido = topo->pedido; // Supondo que a pilha armazena pedidos
        while (atualPedido != NULL)
        {
            Pedido *pedidoRemover = atualPedido;
            atualPedido = atualPedido->prox;
            free(pedidoRemover);
        }

        free(topo);
    }
    *pilhaNaoEfetuada = NULL; // Atualizar o ponteiro da pilha para NULL

    // Limpar fila de devoluções
    FilaDevolucao *atualDevolucao = *filaDevolucao;
    while (atualDevolucao != NULL)
    {
        FilaDevolucao *devolucaoRemover = atualDevolucao;
        atualDevolucao = atualDevolucao->prox;

        // Limpar pedidos na fila de devoluções
        Pedido *atualPedido = devolucaoRemover->pedido; // Supondo que a fila de devoluções armazena pedidos
        while (atualPedido != NULL)
        {
            Pedido *pedidoRemover = atualPedido;
            atualPedido = atualPedido->prox;
            free(pedidoRemover);
        }

        free(devolucaoRemover);
    }
    *filaDevolucao = NULL; // Atualizar o ponteiro da fila de devoluções para NULL
}
