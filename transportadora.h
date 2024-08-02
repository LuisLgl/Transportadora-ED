#ifndef TRANSPORTADORA_H
#define TRANSPORTADORA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições para pedidos
typedef struct Pedido {
    int id;
    char descricao[100];
    struct Pedido *prox;
} Pedido;

// Definições para clientes
typedef struct Cliente {
    int id;
    char nome[50];
    char endereco[100];
    Pedido *pedidos;  // Lista de pedidos do cliente
    struct Cliente *prox;
} Cliente;

typedef struct {
    Cliente *inicio;
} ListaClientes;

// Definições para filas de entrega
typedef struct Entrega {
    int id_pedido;
    char endereco[100];
    struct Entrega *prox;
} Entrega;

typedef struct {
    Entrega *inicio;
    Entrega *fim;
} FilaEntrega;

typedef struct FilaPorEndereco {
    char endereco[100];
    FilaEntrega fila;
    struct FilaPorEndereco *prox;
} FilaPorEndereco;

typedef struct {
    FilaPorEndereco *inicio;
} ListaFilas;

// Funções para a lista de clientes
void inicializaListaClientes(ListaClientes *lista);
void adicionaCliente(ListaClientes *lista);
void removeCliente(ListaClientes *lista);
void editaCliente(ListaClientes *lista);
void imprimeClientes(ListaClientes *lista);

// Funções para a lista de pedidos
void adicionaPedido(Cliente *cliente, ListaFilas *filas);
void removePedido(Cliente *cliente, ListaFilas *filas);
void editaPedido(Cliente *cliente);
void imprimePedidos(Cliente *cliente);

// Funções para a lista de filas de entregas
void inicializaListaFilas(ListaFilas *filas);
void adicionaEntrega(ListaFilas *filas, int id_pedido, char *endereco);
Entrega* removeEntrega(ListaFilas *filas, char *endereco);
void imprimeFilas(ListaFilas *filas);

#endif // TRANSPORTADORA_H
