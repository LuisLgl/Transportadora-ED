#ifndef TRANSPORTADORA_H
#define TRANSPORTADORA_H

// Estrutura para representar um pedido
typedef struct pedido {
    int id;                        // ID do pedido
    char descricao[100];           // Descrição do pedido
    char status[20];               // Status do pedido
    struct pedido *prox;           // Ponteiro para o próximo pedido
} Pedido;

// Estrutura para representar um cliente
typedef struct cliente {
    int id;                        // ID do cliente (últimos 4 dígitos do CPF)
    char cpf[12];                  // CPF do cliente
    char nome[100];                // Nome do cliente
    char endereco[150];            // Endereço do cliente
    Pedido *pedidos;               // Ponteiro para a lista de pedidos do cliente
    struct cliente *prox;          // Ponteiro para o próximo cliente
} Cliente;

// Estrutura para a lista de clientes
typedef struct listaClientes {
    Cliente *inicio;               // Ponteiro para o início da lista de clientes
} ListaClientes;

// Estrutura para representar uma entrega
typedef struct entrega {
    int id_pedido;                 // ID do pedido associado à entrega
    char endereco[150];            // Endereço de entrega
    struct entrega *prox;          // Ponteiro para a próxima entrega
} Entrega;

// Estrutura para representar uma fila de entregas para um endereço específico
typedef struct filaPorEndereco {
    char endereco[150];            // Endereço associado à fila
    Pedido *inicio;                // Ponteiro para o início da fila de entregas
    Pedido *fim;                   // Ponteiro para o fim da fila de entregas
    struct filaPorEndereco *prox;  // Ponteiro para a próxima fila de endereços
} FilaPorEndereco;

// Estrutura para representar a fila de filas de entregas
typedef struct filaFilas {
    FilaPorEndereco *inicio;       // Ponteiro para o início da fila de filas
    FilaPorEndereco *fim;          // Ponteiro para o fim da fila de filas
} FilaFilas;

// Estrutura para representar a pilha de pedidos não efetuados
typedef struct pilhaNaoEfetuada {
    Pedido *pedido;                 // Pedido não efetuado
    struct pilhaNaoEfetuada *prox;  // Ponteiro para o próximo pedido não efetuado
} PilhaNaoEfetuada;

// Estrutura para representar a fila de devoluções
typedef struct filaDevolucao {
    Pedido *pedido;                 // Pedido para devolução
    struct filaDevolucao *prox;     // Ponteiro para o próximo pedido na fila de devolução
} FilaDevolucao;

// Protótipos de funções

// Funções para Clientes
void inicializaListaClientes(ListaClientes *lista);
void adicionaCliente(ListaClientes *lista);
void removeCliente(ListaClientes *lista, FilaFilas *filas);
void editaCliente(ListaClientes *lista, FilaFilas *filas);
void imprimeClientes(ListaClientes *lista);
Cliente *encontrarClientePorID(ListaClientes *lista, int id_cliente);
void removeTodosOsPedidos(FilaFilas *filas);
void liberarPilhaNaoEfetuada(PilhaNaoEfetuada **pilhaNaoEfetuada);



// Funções para Pedidos
void adicionaPedido(ListaClientes *lista);
void removePedido(Cliente *cliente, FilaFilas *filas);
void editaPedido(Cliente *cliente);
void imprimePedidos(Cliente *cliente);
void despacharPedido(ListaClientes *lista, FilaFilas *filas);
void imprimirPilhaNaoEfetuada(PilhaNaoEfetuada **pilhaNaoEfetuada);


// Funções para Filas de Entrega
void inicializaFilaFilas(FilaFilas *filas);
void adicionaEntrega(FilaFilas *filas, int id_pedido, const char *endereco);
void removeEntrega(FilaFilas *filas, int id_pedido, const char *endereco);
void imprimeFilaFilas(FilaFilas *filas);

// Funções para Pilha de Não Efetuadas e Fila de Devolução
void inicializaPilhaNaoEfetuada(PilhaNaoEfetuada **pilha);
void inicializaFilaDevolucao(FilaDevolucao **filadevolucao);
void adicionaPilhaNaoEfetuada(PilhaNaoEfetuada **pilha, Pedido *pedido);
void imprimePilhaNaoEfetuada(PilhaNaoEfetuada *pilha);

void inicializaFilaDevolucao(FilaDevolucao **fila);
void adicionaFilaDevolucao(ListaClientes *clientes, FilaDevolucao **fila, Pedido *pedido);
void imprimeFilaDevolucao(FilaDevolucao *fila);

void concluirEntrega(ListaClientes *clientes,FilaFilas *filas, PilhaNaoEfetuada **pilhaNaoEfetuada, FilaDevolucao **filadevolucao, int *pontos);
void processaPilhaNaoEfetuada(ListaClientes *clientes,PilhaNaoEfetuada **pilhaNaoEfetuada, FilaDevolucao **filaDevolucao, int *pontos);
void processaFilaDevolucao(FilaDevolucao **fila);

// Outras funções
void formatarString(char *str);
void despacharTudo(ListaClientes *lista, FilaFilas *filas);
void adicionaFilaNoFinal(FilaFilas *filas, FilaPorEndereco *novaFila);
void limparMemoria(ListaClientes *listaClientes, FilaFilas *filaFilas, PilhaNaoEfetuada **pilhaNaoEfetuada, FilaDevolucao **filaDevolucao);
void atualizarCliente(ListaClientes *clientes, int idPedido, const char *novoStatus);
#endif // TRANSPORTADORA_H