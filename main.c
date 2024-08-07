#include "Transportadora.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    ListaClientes lista;
    inicializaListaClientes(&lista);

    ListaFilas filas;
    inicializaListaFilas(&filas);

    char escolha;

    do {
        // Display the main menu
        printf("Menu Principal:\n");
        printf("1. Cadastro de Clientes e Pedidos\n");
        printf("2. Transportes\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &escolha);

        switch (escolha) {
            case '1':
                menuCadastro(&lista, &filas);
                break;
            case '2':
                // menuTransportes(&lista);
                break;
            case '0':
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (escolha != '0');

    // Libere a memória aqui, quando terminar

    return 0;
}
