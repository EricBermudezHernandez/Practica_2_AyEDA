#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "big_int.hpp"

template <size_t Base>
class Board {
 public:
  // Constructor
  Board(const std::string& fichero);
  // Método que empieza a leer todas las líneas del fichero, omitiendo la
  // primera
  void Start();

 private:
  std::map<std::string, BigInt<Base>> variables_;
  std::ifstream fichero_;
};

template <size_t Base>
Board<Base>::Board(const std::string& fichero) {
  fichero_.open(fichero.c_str(), std::ios::in);
}

// Método que empieza a leer todas las líneas del fichero y las procesa,
// metiendo los valores y evaluando las expresiones en RPN
template <size_t Base>
void Board<Base>::Start() {
  // Se definen los operandos para las operaciones y el
  // resultado
  BigInt<2> operando_1, operando_2;
  BigInt<Base> resultado;  
  std::stack<BigInt<Base>>
      pila;  // Se crea una pila para guardar los operandos y resultados
  std::string linea, palabra, identificador;  // Se definen variables para
                                              // procesar las líneas del fichero
  std::istringstream
      linea_stream;  // Se crea un flujo de entrada para procesar las líneas
  std::getline(fichero_, linea);  // Se lee la primera línea del fichero y se
                                  // guarda en la variable "linea"
  std::ofstream fichero_salida;   // Se crea un archivo de salida para guardar
                                  // los resultados
  fichero_salida.open("fichero_salida.txt",
                      std::ios::out);  // Se abre el archivo de salida
  fichero_salida
      << linea;  // Se escribe la primera línea en el archivo de salida
  fichero_salida
      << std::endl;  // Se escribe un salto de línea en el archivo de salida
  while (std::getline(fichero_, linea)) {  // Se itera sobre las líneas del fichero
                                          // (omitiendo la primera línea)
    linea_stream.str(linea);  // Se establece la línea actual como entrada para el flujo
    linea_stream >> palabra;  // Se lee la primera palabra de la línea
    identificador = palabra;  // Se guarda la primera palabra como identificador
                              // de la variable
    linea_stream >> palabra;  // Se lee la siguiente palabra
    while (linea_stream >> palabra) {  // Se itera sobre las palabras restantes de la línea
      if (variables_.find(palabra) !=  variables_.end()) {  // Si la palabra es una variable ya definida, se
                                                           // apila su valor en la pila
        pila.push(variables_[palabra]);
      } else if (palabra == "+") {  // Si la palabra es un operador de suma, se
                                    // sacan dos elementos de la pila, se suman,
                                    // y se apila el resultado
        operando_2 = pila.top();
        pila.pop();
        operando_1 = pila.top();
        pila.pop();
        resultado = (operando_1 + operando_2);
        pila.push(resultado);
      } else if (palabra == "-") {  // Lo mismo para los otros operadores aritméticos
        operando_2 = pila.top();
        pila.pop();
        operando_1 = pila.top();
        pila.pop();
        resultado = (operando_1 - operando_2);
        pila.push(resultado);
      } else if (palabra == "*") {
        operando_2 = pila.top();
        pila.pop();
        operando_1 = pila.top();
        pila.pop();
        resultado = (operando_1 * operando_2);
        pila.push(resultado);
      } else if (palabra == "/") {
        operando_2 = pila.top();
        pila.pop();
        operando_1 = pila.top();
        pila.pop();
        resultado = (operando_1 / operando_2);
        pila.push(resultado);
      } else if (palabra == "%") {
        operando_2 = pila.top();
        pila.pop();
        operando_1 = pila.top();
        pila.pop();
        resultado = (operando_1 % operando_2);
        pila.push(resultado);
      } else {
        BigInt<Base> numero{palabra};
        pila.push(numero);
      }
    }
    variables_.insert(std::make_pair(identificador, pila.top()));
    pila.pop();
    linea_stream.clear();
  }
  // Generamos el fichero salida iterarando en el Board para guardar los valores
  auto it = variables_.begin();
  while (it != variables_.end()) {
    fichero_salida << it->first << " = " << it->second << '\n';
    it++;
  }
}

#endif