#include <iostream>
#include "utils.h"

int main() {
    mostrarEncabezado();

    bool continuar = true;

    while (continuar) {
        mostrarMenu();

        int opcion;
        std::cin >> opcion;

        if (opcion == 1) {
            bool exito = procesarArchivo();
            std::cout << "-----------------------------" << std::endl;
            if (exito) {
                std::cout << "Archivo procesado correctamente." << std::endl;
            } else {
                std::cout << "No se pudo procesar el archivo." << std::endl;
            }
        }
        else if (opcion == 2) {
            continuar = false;
        }
        else {
            std::cout << "Opcion no valida. Por favor, seleccione 1 o 2." << std::endl;
        }
    }

    return 0;
}
