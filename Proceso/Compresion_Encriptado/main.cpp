#include <iostream>
#include <fstream>

void compressRLE(const char* texto, int textoLen, unsigned char* comprimido, int& comprimidoLen) {
    comprimidoLen = 0;

    if (textoLen <= 0) return;

    int i = 0;
    while (i < textoLen) {
        char actual = texto[i];
        int count = 1;

        while (i + count < textoLen && texto[i + count] == actual && count < 1000) {
            count++;
        }

        if (comprimidoLen + 3 <= 100000) {
            comprimido[comprimidoLen++] = (count >> 8) & 0xFF;
            comprimido[comprimidoLen++] = count & 0xFF;
            comprimido[comprimidoLen++] = actual;
        }

        i += count;
    }
}

void compressLZ78(const char* texto, int textoLen, unsigned char* comprimido, int& comprimidoLen) {
    comprimidoLen = 0;
    int i = 0;

    while (i < textoLen) {
        if (comprimidoLen + 3 <= 100000) {
            comprimido[comprimidoLen++] = 0;  // Índice high byte
            comprimido[comprimidoLen++] = 0;  // Índice low byte
            comprimido[comprimidoLen++] = texto[i];
        }
        i++;
    }
}

unsigned char rotarIzquierda(unsigned char byte, int n) {
    n = n % 8;
    if (n == 0) return byte;
    return (byte << n) | (byte >> (8 - n));
}

void encriptar(const unsigned char* datos, int tamaño, unsigned char* encriptado,
               int rotacion, unsigned char clave) {
    for (int i = 0; i < tamaño; i++) {
        unsigned char byte = datos[i];
        byte = rotarIzquierda(byte, rotacion);
        byte = byte ^ clave;
        encriptado[i] = byte;
    }
}

char* leerArchivoTexto(const char* nombreArchivo, int& tamaño) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << nombreArchivo << std::endl;
        tamaño = 0;
        return nullptr;
    }

    archivo.seekg(0, std::ios::end);
    tamaño = archivo.tellg();
    archivo.seekg(0, std::ios::beg);

    if (tamaño <= 0) {
        std::cout << "Error: Archivo vacío o inválido" << std::endl;
        return nullptr;
    }

    char* contenido = new char[tamaño + 1];
    archivo.read(contenido, tamaño);
    contenido[tamaño] = '\0';
    archivo.close();

    return contenido;
}

void generarArchivoPrueba(const char* texto, int textoLen, const char* pista,
                          int metodoCompresion, int rotacion, unsigned char clave,
                          const char* nombreArchivo, const char* nombrePista) {

    std::cout << "Generando archivo de prueba..." << std::endl;
    std::cout << "Texto: " << textoLen << " caracteres" << std::endl;
    std::cout << "Metodo: " << (metodoCompresion == 1 ? "RLE" : "LZ78") << std::endl;
    std::cout << "Rotacion: " << rotacion << ", Clave: " << (int)clave << std::endl;

    unsigned char comprimido[100000];
    int comprimidoLen = 0;

    if (metodoCompresion == 1) {
        compressRLE(texto, textoLen, comprimido, comprimidoLen);
    } else {
        compressLZ78(texto, textoLen, comprimido, comprimidoLen);
    }

    unsigned char encriptado[100000];
    encriptar(comprimido, comprimidoLen, encriptado, rotacion, clave);

    std::ofstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo.is_open()) {
        std::cout << "Error creando " << nombreArchivo << std::endl;
        return;
    }
    archivo.write(reinterpret_cast<const char*>(encriptado), comprimidoLen);
    archivo.close();

    std::ofstream pistaFile(nombrePista);
    if (!pistaFile.is_open()) {
        std::cout << "Error creando " << nombrePista << std::endl;
        return;
    }
    pistaFile << pista;
    pistaFile.close();

    std::cout << "Archivos generados: " << nombreArchivo << " y " << nombrePista << std::endl;
}

void menuGenerador() {
    std::cout << "=== GENERADOR DE ARCHIVOS ENCRIPTADOS ===" << std::endl;

    char archivoEntrada[100];
    char pista[100];
    int metodo, rotacion, claveDec;
    char numero[10];

    std::cout << "Archivo de texto de entrada: ";
    std::cin.ignore();
    std::cin.getline(archivoEntrada, 100);

    std::cout << "Pista a incluir: ";
    std::cin.getline(pista, 100);

    std::cout << "Metodo (1=RLE, 2=LZ78): ";
    std::cin >> metodo;

    std::cout << "Rotacion (1-7): ";
    std::cin >> rotacion;

    std::cout << "Clave (0-255): ";
    std::cin >> claveDec;

    std::cout << "Numero para archivos: ";
    std::cin >> numero;

    int textoLen;
    char* texto = leerArchivoTexto(archivoEntrada, textoLen);

    if (texto == nullptr) {
        return;
    }

    bool pistaEncontrada = false;
    int pistaLen = 0;
    while (pista[pistaLen] != '\0') pistaLen++;

    for (int i = 0; i <= textoLen - pistaLen; i++) {
        bool coincide = true;
        for (int j = 0; j < pistaLen; j++) {
            if (texto[i + j] != pista[j]) {
                coincide = false;
                break;
            }
        }
        if (coincide) {
            pistaEncontrada = true;
            break;
        }
    }

    if (!pistaEncontrada) {
        std::cout << "Advertencia: La pista no se encontro en el texto" << std::endl;
        std::cout << "Continuar? (s/n): ";
        char respuesta;
        std::cin >> respuesta;
        if (respuesta != 's' && respuesta != 'S') {
            delete[] texto;
            return;
        }
    }

    char nombreArchivo[50];
    char nombrePista[50];

    int pos = 0;
    const char prefijoArchivo[] = "Encriptado";
    const char prefijoPista[] = "pista";
    const char sufijo[] = ".txt";

    for (int i = 0; prefijoArchivo[i] != '\0'; i++) nombreArchivo[pos++] = prefijoArchivo[i];
    for (int i = 0; numero[i] != '\0'; i++) nombreArchivo[pos++] = numero[i];
    for (int i = 0; sufijo[i] != '\0'; i++) nombreArchivo[pos++] = sufijo[i];
    nombreArchivo[pos] = '\0';

    pos = 0;
    for (int i = 0; prefijoPista[i] != '\0'; i++) nombrePista[pos++] = prefijoPista[i];
    for (int i = 0; numero[i] != '\0'; i++) nombrePista[pos++] = numero[i];
    for (int i = 0; sufijo[i] != '\0'; i++) nombrePista[pos++] = sufijo[i];
    nombrePista[pos] = '\0';

    generarArchivoPrueba(texto, textoLen, pista, metodo, rotacion,
                         (unsigned char)claveDec, nombreArchivo, nombrePista);

    delete[] texto;
}


int main() {
    std::cout << "GENERADOR DE ARCHIVOS ENCRIPTADOS" << std::endl;
    std::cout << "1. Generar desde archivo TXT" << std::endl;
    std::cout << "2. Salir" << std::endl;
    std::cout << "Opcion: ";

    int opcion;
    std::cin >> opcion;

    if (opcion == 1) {
        menuGenerador();
    } else {
        return 0;
    }

    std::cout << "Proceso completado" << std::endl;

    return 0;
}
