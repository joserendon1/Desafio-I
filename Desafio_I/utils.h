#ifndef UTILS_H
#define UTILS_H

void obtenerNombresArchivos(const char* numero, char* nombreArchivo, char* nombrePista);
unsigned char* leerArchivoBinario(const char* nombreArchivo, int& tamaño);
int calcularLongitud(const char* str);
bool buscarSubcadena(const char* texto, int lenTexto, const char* subcadena, int lenSubcadena);
void guardarResultado(const char* numero, const char* nombrePista, int metodo, const char* resultado);
void mostrarEncabezado();
void mostrarMenu();
void mostrarResultados(int rotacion, unsigned char clave, int metodo, int posicion, int longitud, const char* resultado);
void mostrarError();
bool procesarArchivo();

#endif
