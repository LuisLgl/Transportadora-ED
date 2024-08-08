#ifndef TRANSPORTADORA_H
#define TRANSPORTADORA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    Pedido *inicio;               // Ponteiro para o início da fila de entregas
    Pedido *fim;                  // Ponteiro para o fim da fila de entregas
    struct filaPorEndereco *prox;  // Ponteiro para a próxima fila de endereços
} FilaPorEndereco;

// Estrutura para representar a fila de filas de entregas
typedef struct filaFilas {
    FilaPorEndereco *inicio;       // Ponteiro para o início da fila de filas
    FilaPorEndereco *fim;          // Ponteiro para o fim da fila de filas
} FilaFilas;

// Protótipos de funções

// Funções para Clientes
void inicializaListaClientes(ListaClientes *lista);
void adicionaCliente(ListaClientes *lista);
void removeCliente(ListaClientes *lista, FilaFilas *filas);
void editaCliente(ListaClientes *lista);
void imprimeClientes(ListaClientes *lista);
Cliente *encontrarClientePorID(ListaClientes *lista, int id_cliente);

// Funções para Pedidos
void adicionaPedido(ListaClientes *lista, FilaFilas *filas);
void removePedido(Cliente *cliente, FilaFilas *filas);
void editaPedido(Cliente *cliente);
void imprimePedidos(Cliente *cliente);
void despacharPedido(Cliente *cliente, FilaFilas *filas, int id_pedido);

// Funções para Filas de Entrega
void inicializaFilaFilas(FilaFilas *filas);
void adicionaEntrega(FilaFilas *filas, int id_pedido, const char *endereco);
void removeEntrega(FilaFilas *filas, int id_pedido, const char *endereco);
void editaEntrega(FilaFilas *filas);
void imprimeFila(FilaFilas *filas);
void limparMemoria(ListaClientes *clientes, FilaFilas *filas);

#endif // TRANSPORTADORA_H
