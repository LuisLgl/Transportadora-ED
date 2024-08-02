#include "Transportadora.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
   
    ListaClientes lista;
    inicializaListaClientes(&lista);

    menu(&lista);

    return 0;
}