#include <iostream>
#include <fstream>

unsigned char rotarDerecha(unsigned char byte, int n) {
    return (byte >> n) | (byte << (8 - n));
}

unsigned char* leerArchivoBinario(const char* nombreArchivo, int& tamaño) {
    std::ifstream archivo(nombreArchivo, std::ios::binary | std::ios::ate);
    if (!archivo.is_open()) {
        std::cout << "Error al abrir: " << nombreArchivo << std::endl;
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

int main() {

    const char* nombreArchivo = "Encriptado2.txt";
    const int rotacion = 3;
    const unsigned char clave = 0x5A;

    std::cout << "Archivo: " << nombreArchivo << std::endl;
    std::cout << "Parametros: Rotacion=" << rotacion << ", Clave=0x" << std::hex << (int)clave << std::dec << std::endl;

    int tamañoEntrada;
    unsigned char* datosEncriptados = leerArchivoBinario(nombreArchivo, tamañoEntrada);

    if (!datosEncriptados || tamañoEntrada == 0) {
        std::cout << "Error: No se pudo leer el archivo" << std::endl;
        return 1;
    }

    std::cout << "size del archivo: " << tamañoEntrada << " bytes" << std::endl;

    unsigned char* resultado = new unsigned char[tamañoEntrada + 1];

    for (int i = 0; i < tamañoEntrada; i++) {
        unsigned char byte = datosEncriptados[i];
        byte = byte ^ clave;
        byte = rotarDerecha(byte, rotacion);
        resultado[i] = byte;
    }
    resultado[tamañoEntrada] = '\0';

    std::cout << std::endl;

    std::cout << "unsigned char resultado[" << tamañoEntrada + 1 << "] = {";
    for (int i = 0; i < tamañoEntrada; i++) {
        std::cout << "0x";
        if (resultado[i] < 16) std::cout << "0";
        std::cout << std::hex << (int)resultado[i];
        if (i < tamañoEntrada - 1) std::cout << ", ";
        if ((i + 1) % 15 == 0) std::cout << "\n    ";
    }
    std::cout << "};" << std::dec << std::endl;

    std::cout << std::endl;

    std::cout << "char resultado[" << tamañoEntrada + 1 << "] = {";
    for (int i = 0; i < tamañoEntrada; i++) {
        if (resultado[i] >= 32 && resultado[i] <= 126) {
            std::cout << "'" << resultado[i] << "'";
        } else {
            std::cout << "0x";
            if (resultado[i] < 16) std::cout << "0";
            std::cout << std::hex << (int)resultado[i];
        }
        if (i < tamañoEntrada - 1) std::cout << ", ";
        if ((i + 1) % 10 == 0) std::cout << "\n    ";
    }
    std::cout << "};" << std::dec << std::endl;

    std::cout << std::endl;

    std::cout << "Primeros 300 caracteres:" << std::endl;
    for (int i = 0; i < 300 && i < tamañoEntrada; i++) {
        if (resultado[i] >= 32 && resultado[i] <= 126) {
            std::cout << resultado[i];
        } else {
            std::cout << "[" << std::hex << (int)resultado[i] << std::dec << "]";
        }
    }
    std::cout << std::endl;

    delete[] datosEncriptados;
    delete[] resultado;
    return 0;
}
