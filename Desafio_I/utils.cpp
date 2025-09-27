#include "utils.h"
#include <iostream>
#include <fstream>
#include "encriptacion.h"

void obtenerNombresArchivos(const char* numero, char* nombreArchivo, char* nombrePista) {
    const char* prefijos[] = {"Encriptado", "pista"};
    char* nombres[] = {nombreArchivo, nombrePista};

    for (int k = 0; k < 2; k++) {
        int pos = 0;
        for (int i = 0; prefijos[k][i] != '\0'; i++) nombres[k][pos++] = prefijos[k][i];
        for (int i = 0; numero[i] != '\0'; i++) nombres[k][pos++] = numero[i];
        for (int i = 0; ".txt"[i] != '\0'; i++) nombres[k][pos++] = ".txt"[i];
        nombres[k][pos] = '\0';
    }
}

unsigned char* leerArchivoBinario(const char* nombreArchivo, int& tamaño) {
    std::ifstream archivo(nombreArchivo, std::ios::binary | std::ios::ate);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << nombreArchivo << std::endl;
        tamaño = 0;
        return nullptr;
    }

    tamaño = archivo.tellg();
    archivo.seekg(0, std::ios::beg);

    unsigned char* datos = new unsigned char[tamaño];
    archivo.read(reinterpret_cast<char*>(datos), tamaño);
    archivo.close();

    return datos;
}

int calcularLongitud(const char* str) {
    int len = 0;
    while (str[len] != '\0' && len < 1000) len++;
    return len;
}

bool buscarSubcadena(const char* texto, int lenTexto, const char* subcadena, int lenSubcadena) {
    if (lenSubcadena <= 0 || lenTexto < lenSubcadena) return false;

    for (int i = 0; i <= lenTexto - lenSubcadena; i++) {
        bool encontrado = true;
        for (int j = 0; j < lenSubcadena; j++) {
            if (texto[i + j] != subcadena[j]) {
                encontrado = false;
                break;
            }
        }
        if (encontrado) return true;
    }
    return false;
}

void guardarResultado(const char* numero, const char* nombrePista, int metodo, const char* resultado) {
    char nombreSalida[50];
    obtenerNombresArchivos(numero, nombreSalida, nombreSalida);

    int len = calcularLongitud(nombreSalida);
    if (metodo == 1) {
        nombreSalida[len-4] = '_';
        nombreSalida[len-3] = 'r';
        nombreSalida[len-2] = 'l';
        nombreSalida[len-1] = 'e';
        nombreSalida[len] = '.';
        nombreSalida[len+1] = 't';
        nombreSalida[len+2] = 'x';
        nombreSalida[len+3] = 't';
        nombreSalida[len+4] = '\0';
    } else {
        nombreSalida[len-4] = '_';
        nombreSalida[len-3] = 'l';
        nombreSalida[len-2] = 'z';
        nombreSalida[len-1] = '7';
        nombreSalida[len] = '8';
        nombreSalida[len+1] = '.';
        nombreSalida[len+2] = 't';
        nombreSalida[len+3] = 'x';
        nombreSalida[len+4] = 't';
        nombreSalida[len+5] = '\0';
    }

    std::ofstream archivo(nombreSalida);
    if (archivo.is_open()) {
        archivo << resultado;
        archivo.close();
        std::cout << "\nResultado guardado en: " << nombreSalida << std::endl;
    } else {
        std::cout << "\nError: No se pudo guardar el resultado" << std::endl;
    }
}

void mostrarEncabezado() {
    std::cout << "DESAFIO I" << std::endl;
    std::cout << "Informatica II" << std::endl;
    std::cout << std::endl;
}

void mostrarMenu() {
    std::cout << "MENU PRINCIPAL" << std::endl;
    std::cout << "1. Desencriptar y descomprimir mensaje" << std::endl;
    std::cout << "2. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarResultados(int rotacion, unsigned char clave, int metodo, int posicion, int longitud, const char* resultado) {
    std::cout << "------------------------------" << std::endl;
    std::cout << "- Rotacion (n): " << rotacion << " bits a la izquierda" << std::endl;
    std::cout << "- Clave XOR (K): 0x" << std::hex << (int)clave << std::dec;
    std::cout << " (" << (int)clave << " decimal)" << std::endl;
    std::cout << "- Metodo de compresion: " << (metodo == 1 ? "RLE" : "LZ78") << std::endl;
    std::cout << "- Posicion de la pista: caracter #" << posicion + 1 << std::endl;
    std::cout << "- Longitud del texto: " << longitud << " caracteres" << std::endl;
    std::cout << "------------------------------" << std::endl;
    std::cout << resultado << std::endl;
}

void mostrarError() {
    std::cout << "------------------------------" << std::endl;
    std::cout << "Posibles causas:" << std::endl;
    std::cout << "1. La pista no coincide con el texto" << std::endl;
    std::cout << "2. Los parametros estan fuera del rango probado" << std::endl;
    std::cout << "3. El formato del archivo es diferente" << std::endl;
    std::cout << "4. Se uso un metodo de compresion diferente" << std::endl;
}

bool procesarArchivo() {
    char numero[10], nombreArchivo[50], nombrePista[50];
    std::cout << "Ingrese el numero del archivo: ";
    std::cin >> numero;

    obtenerNombresArchivos(numero, nombreArchivo, nombrePista);

    std::cout << "\nArchivos a procesar" << std::endl;
    std::cout << "- Encriptado: " << nombreArchivo << std::endl;
    std::cout << "- Pista: " << nombrePista << std::endl;
    std::cout << std::endl;

    int tamaño;
    unsigned char* datosEncriptados = leerArchivoBinario(nombreArchivo, tamaño);
    if (!datosEncriptados) {
        std::cout << "Error: No se pudo cargar el archivo encriptado." << std::endl;
        return false;
    }
    std::cout << "Archivo encriptado cargado" << std::endl;

    std::ifstream pistaFile(nombrePista);
    if (!pistaFile.is_open()) {
        std::cout << "Error: No se pudo abrir " << nombrePista << std::endl;
        delete[] datosEncriptados;
        return false;
    }

    char pista[1000] = {0};
    pistaFile.getline(pista, 1000);
    pistaFile.close();

    int lenPista = calcularLongitud(pista);
    if (lenPista > 0 && pista[lenPista - 1] == '\n') {
        pista[lenPista - 1] = '\0';
    }

    int largoPista = calcularLongitud(pista);
    std::cout << "Pista conocida: " << pista << std::endl;
    std::cout << std::endl;

    char* resultadoFinal = nullptr;
    int resultadoLen = 0;
    int rotacionEncontrada = -1, metodoEncontrado = 0, posicionPista = -1;
    unsigned char claveEncontrada = 0;
    bool encontrado = false;

    for (int rotacion = 1; rotacion <= 7 && !encontrado; rotacion++) {
        for (int clave = 0; clave <= 255 && !encontrado; clave++) {
            int posicion = -1, metodo = 0;
            if (probarCombinacion(datosEncriptados, tamaño, pista, largoPista,
                                  rotacion, (unsigned char)clave, resultadoFinal, resultadoLen, metodo, posicion)) {
                rotacionEncontrada = rotacion;
                claveEncontrada = (unsigned char)clave;
                metodoEncontrado = metodo;
                posicionPista = posicion;
                encontrado = true;
            }
        }
    }

    if (encontrado) {
        mostrarResultados(rotacionEncontrada, claveEncontrada, metodoEncontrado,
                          posicionPista, resultadoLen, resultadoFinal);
        guardarResultado(numero, nombrePista, metodoEncontrado, resultadoFinal);

        delete[] resultadoFinal;
    } else {
        mostrarError();
    }

    delete[] datosEncriptados;
    return encontrado;
}
