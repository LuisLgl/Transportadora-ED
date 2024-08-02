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



// Funções para a lista de clientes
void inicializaListaClientes(ListaClientes *lista);
void adicionaCliente(ListaClientes *lista);
void removeCliente(ListaClientes *lista);
void editaCliente(ListaClientes *lista);
void imprimeClientes(ListaClientes *lista);

// Funções para a lista de pedidos
void adicionaPedido(Cliente *cliente);
void removePedido(Cliente *cliente);
void editaPedido(Cliente *cliente);
void imprimePedidos(Cliente *cliente);
int menu(ListaClientes *lista);

#endif // TRANSPORTADORA_H
