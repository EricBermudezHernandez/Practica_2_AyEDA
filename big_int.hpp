#ifndef BIG_INT_H
#define BIG_INT_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// Práctica 1: Plantilla clase BigInt
template <size_t Base>
class BigInt {
 public:
  // Constructores
  BigInt(long long n = 0);
  BigInt(std::string&);
  BigInt(const char*);
  BigInt(const BigInt<Base>&);  // Constructor de copia
  // Acceso:
  int sign() const { return signo_; }   // Signo: 1 o -1
  char operator[](int posicion) const;  // Acceso al i-ésimo dígito
  // Operadores:
  BigInt<Base>& operator=(const BigInt<Base>&);
  // Inserción / Extracción
  template <size_t Bass>
  friend std::ostream& operator<<(std::ostream&, const BigInt<Bass>&);
  template <size_t Bass>
  friend std::istream& operator>>(std::istream&, BigInt<Bass>&);
  // Operadores de comparación
  template <size_t Bass>
  friend bool operator==(const BigInt<Bass>& numero1,
                         const BigInt<Bass>& numero2);
  bool operator!=(const BigInt<Base>&) const;
  template <size_t Bass>
  friend bool operator>(const BigInt<Bass>& numero1,
                        const BigInt<Bass>& numero2);
  bool operator>=(const BigInt<Base>&) const;
  template <size_t Bass>
  friend bool operator<(const BigInt<Bass>& numero1,
                        const BigInt<Bass>& numero2);
  bool operator<=(const BigInt<Base>&) const;
  // Operadores de incremento / decremento
  BigInt<Base>& operator++();    // Pre-incremento
  BigInt<Base> operator++(int);  // Post-incremento
  BigInt<Base>& operator--();    // Pre-decremento
  BigInt<Base> operator--(int);  // Post-decremento
  // Operadores aritméticos
  template <size_t Bass>
  friend BigInt<Bass> operator+(const BigInt<Bass>&, const BigInt<Bass>&);
  BigInt<Base> operator-(const BigInt<Base>& numero2) const;
  BigInt<Base> operator-() const;
  BigInt<Base> operator*(const BigInt<Base>& numero2) const;
  template <size_t Bass>
  friend BigInt<Bass> operator/(const BigInt<Bass>& numero1,
                                const BigInt<Bass>& numero2);
  BigInt<Base> operator%(const BigInt<Base>& numero2) const;
  // Potencia ab
  template <size_t Bass>
  friend BigInt<Bass> pow(const BigInt<Bass>&, const BigInt<Bass>&);

  // Modificación: Pasar números de cualquier base a base 2
  template <size_t U>
  operator BigInt<U>() {
    std::string numero, residuos;
    BigInt<Base> aux{*this};
    for (int i{aux.numero_.size() - 1}; i >= 0; --i) {
      if (aux.numero_[i] >= 10) {
        numero.push_back(static_cast<char>('A' + numero_[i] - 10));
      } else {
        numero.push_back((static_cast<char>(numero_[i] + '0')));
      }
    }
    long long aux_numero{std::stoll(numero)};
    int resto;
    while (aux_numero >= U) {
      resto = aux_numero % U;
      residuos.push_back(static_cast<char>(resto + '0'));
      aux_numero /= U;
    }
    residuos.push_back(static_cast<char>(aux_numero + '0'));
    if (aux.sign() == 1 && U == 2) {
      residuos.push_back(static_cast<char>(0 + '0'));
    }
    std::reverse(residuos.begin(), residuos.end());
    BigInt<U> resultado{residuos};
    return resultado;
  }

 private:
  int signo_;
  std::vector<char> numero_;
};

// =============== CONSTRUCTORES ===============

template <size_t Base>
BigInt<Base>::BigInt(long long n) {
  long long resto{0};
  // Comprobamos si el número es menor que 0 para determinar el signo
  if (n >= 0) {
    signo_ = 1;
  } else {
    signo_ = -1;
  }
  while (n > 9) {
    resto = n % 10;
    n /= 10;
    numero_.emplace_back(resto);
  }
  numero_.emplace_back(n);
}

template <size_t Base>
BigInt<Base>::BigInt(std::string& numero_string) {
  // Comprobamos si existe el carácter '-' para ver si el número es negativo o
  // no
  size_t signo_encontrado{numero_string.find('-')};
  if (signo_encontrado ==
      std::string::npos) {  // npos es una constante de string que dice si lo ha
                            // encontrado o no
    signo_ = 1;
  } else {
    signo_ = -1;
  }
  for (int i{numero_string.size() - 1}; i >= 0; --i) {
    if (isalpha(numero_string[i])) {  // Comprobamos si el dígito es un caracter
                                      // para después comprobar si es un
                                      // caracter hexadecimal para hacer la
                                      // conversión y meterlo en el vector
      if (numero_string[i] == 'A' || numero_string[i] == 'B' ||
          numero_string[i] == 'C' || numero_string[i] == 'D' ||
          numero_string[i] == 'E' || numero_string[i] == 'F') {
        switch (numero_string[i]) {  // En caso de que sea una de las letras que
                                     // queremos, comprobamos cual es y metemos
                                     // su correspondiente valor en hexdecimal
          case 'A':
            numero_.emplace_back(10);
            break;
          case 'B':
            numero_.emplace_back(11);
            break;
          case 'C':
            numero_.emplace_back(12);
            break;
          case 'D':
            numero_.emplace_back(13);
            break;
          case 'E':
            numero_.emplace_back(14);
            break;
          case 'F':
            numero_.emplace_back(15);
            break;
          default:
            break;
        }
      } else {  // En caso de no ser ninguna de las letras hexadecimales, el
                // usuario nos habrá metido una incorrecta por lo que indicamos
                // el error
        std::cerr << "Letra incorrecta" << std::endl;
      }
    } else {  // Si no es ninguna letra, es que es un número, por lo que lo
              // metemos directamente
      numero_.emplace_back(
          numero_string[i] -
          '0');  // Le restamos '0' para convertirlo en un entero
    }
  }
}

template <size_t Base>
BigInt<Base>::BigInt(const char* numero) {
  std::vector<char> vector_aux;
  long long aux{std::stoll(numero)};
  // Comprobamos si el número es menor que 0 para determinar el signo
  if (aux >= 0) {
    signo_ = 1;
  } else {
    signo_ = -1;
  }
  while (*numero != '\0') {
    if (isalpha(*numero)) {  // Comprobamos si el dígito es un caracter para
                             // después comprobar si es un caracter hexadecimal
                             // para hacer la conversión y meterlo en el vector
      if (*numero == 'A' || *numero == 'B' || *numero == 'C' ||
          *numero == 'D' || *numero == 'E' || *numero == 'F') {
        switch (*numero++) {  // En caso de que sea una de las letras que
                              // queremos, comprobamos cual es y metemos su
                              // correspondiente valor en hexdecimal
          case 'A':
            vector_aux.emplace_back(10);
            break;
          case 'B':
            vector_aux.emplace_back(11);
            break;
          case 'C':
            vector_aux.emplace_back(12);
            break;
          case 'D':
            vector_aux.emplace_back(13);
            break;
          case 'E':
            vector_aux.emplace_back(14);
            break;
          case 'F':
            vector_aux.emplace_back(15);
            break;
          default:
            break;
        }
      } else {
        std::cerr << "Letra incorrecta" << std::endl;
        exit(1);
      }
    } else {  // Si no es ninguna letra, es que es un número, por lo que lo
              // metemos directamente
      vector_aux.emplace_back(
          (*numero++) - '0');  // Le restamos '0' para convertirlo en un entero
    }
  }
  std::reverse(vector_aux.begin(),
               vector_aux.end());  // Invertimos el vector para que se guarde
                                   // alrevés en el vector de chars del objeto
  numero_ = vector_aux;
}

template <size_t Base>
BigInt<Base>::BigInt(const BigInt<Base>& segundo_numero) {
  BigInt<Base> aux{};
  if (segundo_numero >= aux) {
    signo_ = 1;
  } else {
    signo_ = -1;
  }
  numero_ = segundo_numero.numero_;
}

// =============== SOBRECARGA DE OPERADOR [] ===============

template <size_t Base>
char BigInt<Base>::operator[](int posicion) const {
  // Usamos la "fórmula" [(numero_.size() - 1) - posicion] para acceder a el
  // número que queremos, esto lo hacemos así ya que el vector de chars del
  // BigInt está dado la vuelta
  if (numero_[(numero_.size() - 1) - posicion] >=
      10) {  // Si el número es mayor o igual que 10 es que es una letra en
             // hexadecimal así que la convertimos a un número
    return (
        static_cast<char>('A' + numero_[(numero_.size() - 1) - posicion] - 10));
  }  // Si es menor que 10 es que es un número, por lo que lo convertimos a un
     // char o lo sacamos por pantalla
  return (static_cast<char>(numero_[(numero_.size() - 1) - posicion] + '0'));
}

// ============= SOBRECARGA DE OPERADORES DE INSERCIÓN/EXTRACCIÓN =============

template <size_t Base>
std::ostream& operator<<(std::ostream& os, const BigInt<Base>& numero) {
  for (int i{numero.numero_.size() - 1}; i >= 0; --i) {
    if (numero.numero_[i] >=
        10) {  // Si el número es mayor o igual que 10 es que es una letra en
               // hexadecimal así que la convertimos a un número
      os << (static_cast<char>('A' + numero.numero_[i] - 10));
    } else {  // Si es menor que 10 es que es un número, por lo que lo
              // convertimos a un char o lo sacamos por pantalla
      os << (static_cast<char>(numero.numero_[i] + '0'));
    }
  }
  return os;
}

template <size_t Base>
std::istream& operator>>(std::istream& is, BigInt<Base>& big_int) {
  std::string aux_numero;
  is >> aux_numero;  // Guardamos el número en un string
  BigInt<Base> aux{
      aux_numero};  // Creamos un BigInt auxiliar para que el vector de chars de
                    // el número que queremos introducirle el valor sea cambiado
                    // por el que queramos
  big_int.numero_.clear();  // Borramos el contenido del BigInt para
                            // introducirle el nuevo valor
  big_int.numero_ = aux.numero_;
  return is;
}

// =============== SOBRECARGA DE OPERADOR '=' ===============

template <size_t Base>
BigInt<Base>& BigInt<Base>::operator=(const BigInt<Base>& segundo_numero) {
  numero_ = segundo_numero.numero_;
  return *this;
}

// =============== SOBRECARGA DE OPERADORES DE COMPARACIÓN ===============

template <size_t Base>
bool operator==(const BigInt<Base>& numero1, const BigInt<Base>& numero2) {
  return numero1.numero_ == numero2.numero_;
}

template <size_t Base>
bool operator>(const BigInt<Base>& numero1, const BigInt<Base>& numero2) {
  std::vector<char> aux_vector_numero1{numero1.numero_},
      aux_vector_numero2{numero2.numero_};
  long unsigned int i{aux_vector_numero1.size() - 1},
      j{aux_vector_numero2.size() - 1}, size_maximo{};
  // Eliminamos los posibles 0s a la izquierda que puedan haber, para poder
  // comparar por el tamaño de los vectores
  while (static_cast<char>(aux_vector_numero1[i] + '0') == '0' &&
         aux_vector_numero1.size() > 1) {
    aux_vector_numero1.pop_back();
    i--;
  }
  while (static_cast<char>(aux_vector_numero2[j] + '0') == '0' &&
         aux_vector_numero2.size() > 1) {
    aux_vector_numero1.pop_back();
    j--;
  }
  if (aux_vector_numero1.size() > aux_vector_numero2.size() ||
      (numero1.sign() == 1 && numero2.sign() == -1)) {
    return true;
  } else if (aux_vector_numero2.size() > aux_vector_numero1.size() ||
             (numero1.sign() == -1 && numero2.sign() == 1)) {
    return false;
  }
  // Si hemos llegado a este punto, es que tienen el mismo tamaño, tenemos
  // invertir los vectores por que están alrevés por el propio constructor de
  // BigInt para después compararlos
  std::reverse(aux_vector_numero1.begin(), aux_vector_numero1.end());
  std::reverse(aux_vector_numero2.begin(), aux_vector_numero2.end());
  return aux_vector_numero1 > aux_vector_numero2;
}

template <size_t Bass>
bool operator<(const BigInt<Bass>& numero1, const BigInt<Bass>& numero2) {
  std::vector<char> aux_vector_numero1{numero1.numero_},
      aux_vector_numero2{numero2.numero_};
  long unsigned int i{aux_vector_numero1.size() - 1},
      j{aux_vector_numero2.size() - 1}, size_maximo{};
  // Eliminamos los posibles 0s a la izquierda que puedan haber, para poder
  // comparar por el tamaño de los vectores
  while (static_cast<char>(aux_vector_numero1[i] + '0') == '0' &&
         aux_vector_numero1.size() > 1) {
    aux_vector_numero1.pop_back();
    i--;
  }
  while (static_cast<char>(aux_vector_numero2[j] + '0') == '0' &&
         aux_vector_numero2.size() > 1) {
    aux_vector_numero1.pop_back();
    j--;
  }

  if (aux_vector_numero1.size() < aux_vector_numero2.size() ||
      (numero1.sign() == -1 && numero2.sign() == 1)) {
    return true;
  } else if (aux_vector_numero2.size() < aux_vector_numero1.size() ||
             (numero1.sign() == 1 && numero2.sign() == -1)) {
    return false;
  }
  // Si hemos llegado a este punto, es que tienen el mismo tamaño, tenemos
  // invertir los vectores por que están alrevés por el propio constructor de
  // BigInt para después compararlos
  std::reverse(aux_vector_numero1.begin(), aux_vector_numero1.end());
  std::reverse(aux_vector_numero2.begin(), aux_vector_numero2.end());
  return aux_vector_numero1 < aux_vector_numero2;
}

template <size_t Base>
bool BigInt<Base>::operator!=(const BigInt<Base>& numero2) const {
  return numero_ != numero2.numero_;
}

template <size_t Base>
bool BigInt<Base>::operator>=(const BigInt<Base>& numero2) const {
  std::vector<char> aux_vector_numero1{numero_},
      aux_vector_numero2{numero2.numero_};
  long unsigned i{aux_vector_numero1.size() - 1},
      j{aux_vector_numero2.size() - 1}, size_maximo{};
  // Eliminamos los posibles 0s a la izquierda que puedan haber, para poder
  // comparar por el tamaño de los vectores
  while (static_cast<char>(aux_vector_numero1[i] + '0') == '0' &&
         aux_vector_numero1.size() > 1) {
    aux_vector_numero1.pop_back();
    i--;
  }
  while (static_cast<char>(aux_vector_numero2[j] + '0') == '0' &&
         aux_vector_numero2.size() > 1) {
    aux_vector_numero1.pop_back();
    j--;
  }

  if (aux_vector_numero1.size() > aux_vector_numero2.size() ||
      (signo_ == 1 && numero2.sign() == -1)) {
    return true;
  } else if (aux_vector_numero2.size() > aux_vector_numero1.size() ||
             (signo_ == -1 && numero2.sign() == 1)) {
    return false;
  }
  // Si hemos llegado a este punto, es que tienen el mismo tamaño, tenemos
  // invertir los vectores por que están alrevés por el propio constructor de
  // BigInt para después compararlos
  std::reverse(aux_vector_numero1.begin(), aux_vector_numero1.end());
  std::reverse(aux_vector_numero2.begin(), aux_vector_numero2.end());
  return aux_vector_numero1 >= aux_vector_numero2;
}

template <size_t Base>
bool BigInt<Base>::operator<=(const BigInt<Base>& numero2) const {
  std::vector<char> aux_vector_numero1{numero_},
      aux_vector_numero2{numero2.numero_};
  long unsigned i{aux_vector_numero1.size() - 1},
      j{aux_vector_numero2.size() - 1}, size_maximo{};
  // Eliminamos los posibles 0s a la izquierda que puedan haber, para poder
  // comparar por el tamaño de los vectores
  while (static_cast<char>(aux_vector_numero1[i] + '0') == '0' &&
         aux_vector_numero1.size() > 1) {
    aux_vector_numero1.pop_back();
    i--;
  }
  while (static_cast<char>(aux_vector_numero2[j] + '0') == '0' &&
         aux_vector_numero2.size() > 1) {
    aux_vector_numero1.pop_back();
    j--;
  }
  if (aux_vector_numero1.size() < aux_vector_numero2.size() ||
      (signo_ == -1 && numero2.sign() == 1)) {
    return true;
  } else if (aux_vector_numero2.size() < aux_vector_numero1.size() ||
             (signo_ == 1 && numero2.sign() == -1)) {
    return false;
  }
  // Si hemos llegado a este punto, es que tienen el mismo tamaño, tenemos
  // invertir los vectores por que están alrevés por el propio constructor de
  // BigInt para después compararlos
  std::reverse(aux_vector_numero1.begin(), aux_vector_numero1.end());
  std::reverse(aux_vector_numero2.begin(), aux_vector_numero2.end());
  return aux_vector_numero1 <= aux_vector_numero2;
}

// =============== OPERADORES ARITMÉTICOS ===============

// Operador cambio de signo
template <size_t Base>
BigInt<Base> BigInt<Base>::operator-() const {
  BigInt<Base> aux{*this};
  if (aux.sign() == 1) {  // Si numero_.sign() = 1, es que es positivo, por lo
                          // que le añadimos el signo -(45)
    aux.signo_ = -1;
    aux.numero_.emplace_back('-' - '0');
  } else if (aux.sign() ==
             -1) {  // Si numero_.sign() = -1, es que es negativo, por lo que
                    // le quitamos el signo - para hacerlo positivo
    aux.signo_ = 1;
    aux.numero_.pop_back();
  }
  return aux;
}

// Operador resta
template <size_t Base>
BigInt<Base> BigInt<Base>::operator-(const BigInt<Base>& numero2) const {
  int carry = 0;
  BigInt<Base> aux_numero1{*this}, aux_numero2{numero2}, result;

  // Rellenando el número más corto con 0s
  int diferencia = aux_numero1.numero_.size() - aux_numero2.numero_.size();
  if (diferencia < 0) {  // numero2 es el menor de los dos
    for (int i = 0; i < -diferencia; i++) {
      aux_numero1.numero_.push_back(0);
    }
  } else if (diferencia > 0) {  // numero1 es el menor de los dos
    for (int i = 0; i < diferencia; i++) {
      aux_numero2.numero_.push_back(0);
    }
  }

  // Restando los números
  for (int i = 0; i < aux_numero1.numero_.size(); i++) {
    int difference = aux_numero1.numero_[i] - aux_numero2.numero_[i] - carry;
    if (difference < 0) {
      carry = 1;
      difference += Base;
    } else {
      carry = 0;
    }
    result.numero_.push_back(difference);
  }

  // Al final de cada operación se añade un 0 adicional al resultado de la
  // multiplicación, por lo que se elimina antes de devolver el resultado
  // invirtiendo el número, quitando el 0 y volviendo a invertir el número
  std::reverse(result.numero_.begin(), result.numero_.end());
  result.numero_.pop_back();
  std::reverse(result.numero_.begin(), result.numero_.end());
  return result;
}

// Operador suma
template <size_t Base>
BigInt<Base> operator+(const BigInt<Base>& numero1,
                       const BigInt<Base>& numero2) {
  int carry = 0;
  BigInt<Base> aux_numero1{numero1}, aux_numero2{numero2},
      result;  // resultado como BigInt

  // Rellenando el número más corto con 0s
  int diferencia{aux_numero1.numero_.size() - aux_numero2.numero_.size()};
  if (diferencia < 0) {  // numero1 es el menor de los dos
    for (int i{0}; i < -diferencia; ++i) {
      aux_numero1.numero_.push_back(0);
    }
  } else if (diferencia > 0) {  // numero2 es el menor de los dos
    for (int i = 0; i < diferencia; i++) {
      aux_numero2.numero_.push_back(0);
    }
  }

  // Sumando los números
  for (int i = 0; i < aux_numero1.numero_.size(); i++) {
    int sum = aux_numero1.numero_[i] + aux_numero2.numero_[i] + carry;
    result.numero_.push_back(sum % Base);
    carry = sum / Base;
  }

  // Añadiendo el carry si es que hay
  if (carry > 0) {
    result.numero_.push_back(carry);
  }

  // Al final de cada operación se añade un 0 adicional al resultado de la suma,
  // por lo que se elimina antes de devolver el resultado invirtiendo el número,
  // quitando el 0 y volviendo a invertir el número
  std::reverse(result.numero_.begin(), result.numero_.end());
  result.numero_.pop_back();
  std::reverse(result.numero_.begin(), result.numero_.end());
  return result;
}

// Operador multiplicación
template <size_t Base>
BigInt<Base> BigInt<Base>::operator*(const BigInt<Base>& numero2) const {
  BigInt<Base> aux{*this}, result{"0"};
  for (int i = 0; i < aux.numero_.size(); i++) {
    int carry = 0;
    BigInt<Base> aux_result;
    for (int j = 0; j < numero2.numero_.size() || carry > 0; j++) {
      int mul = aux.numero_[i] *
                    (j < numero2.numero_.size() ? numero2.numero_[j] : 0) +
                carry;
      aux_result.numero_.push_back(mul % Base);
      carry = mul / Base;
    }
    for (int j = 0; j < i; j++) {
      aux_result.numero_.insert(aux_result.numero_.begin(), 0);
    }
    result = result + aux_result;
  }
  // Al final de cada operación se añade un 0 adicional al resultado de la
  // multiplicación, por lo que se elimina antes de devolver el resultado
  // invirtiendo el número, quitando el 0 y volviendo a invertir el número
  std::reverse(result.numero_.begin(), result.numero_.end());
  result.numero_.pop_back();
  std::reverse(result.numero_.begin(), result.numero_.end());
  return result;
}

// Operador división
template <size_t Base>
BigInt<Base> operator/(const BigInt<Base>& numero1,
                       const BigInt<Base>& numero2) {
  // Si se cumplen las siguientes dos condiciones, el divisor, que en este caso
  // es "numero2", es 0. Una división por 0 no es posible, por lo que lo
  // indicamos y salimos del programa
  if (numero2.numero_.size() == 1 && numero2.numero_[0] == 0) {
    std::cout << "No es posible realizar una división por 0" << std::endl;
    exit(1);
  }

  // Verificar si el dividendo(numero1) es menor que el divisor(numero2), si
  // esto es así, el cociente de la división dará 0, por lo que retornamos un
  // BigInt con valor 0.
  if (numero1 < numero2) {
    return BigInt<Base>{};
  }

  // realizar la división
  BigInt<Base> resta{numero1},
      resultado{};  // Contador que después de hacer restas consecutivas, será
                    // el resultado de la división
  while (resta >= numero2) {
    resta = resta - numero2;
    resultado++;
  }
  return resultado;
}

// Operador módulo
template <size_t Base>
BigInt<Base> BigInt<Base>::operator%(const BigInt<Base>& numero2) const {
  // Si se cumplen las siguientes dos condiciones, el divisor, que en este
  // caso es "numero2", es 0. Una división por 0 no es posible, por lo que lo
  // indicamos y salimos del programa
  if (numero2.numero_.size() == 1 && numero2.numero_[0] == 0) {
    std::cout << "No es posible realizar una división por 0" << std::endl;
    exit(1);
  }

  // Verificar si el dividendo(numero1) es menor que el divisor(numero2), si
  // esto es así, el cociente de la división dará 0, por lo que retornamos un
  // BigInt con valor 0.
  if (*this < numero2) {
    return BigInt<Base>{};
  }

  // realizar la división
  BigInt<Base> resta{*this},
      resultado{};  // Contador que después de hacer restas consecutivas, será
                    // el resultado de la división
  while (resta >= numero2) {
    resta = resta - numero2;
    resultado++;
  }

  // Quitamos el 0 sobrante
  resta.numero_.pop_back();
  return resta;
}

// Potencia
template <size_t Base>
BigInt<Base> pow(const BigInt<Base>& base, const BigInt<Base>& exponente) {
  BigInt<Base> resultado{1}, contador{};
  while (contador < exponente) {
    resultado = resultado * base;  // vamos haciendo la potencia multiplicando
                                   // el número por si mismo iterativamente
    contador++;
  }
  return resultado;
}

// =============== OPERADORES DE INCREMENTO / DECREMENTO ===============

template <size_t Base>
BigInt<Base>& BigInt<Base>::operator++() {  // Pre-incremento
  // BigInt auxiliar inicializado a '1' para poder hacer el incremento con la
  // sobrecarga del operador '+'
  BigInt<Base> aux{1};
  *this = *this + aux;
  return *this;
}

template <size_t Base>
BigInt<Base> BigInt<Base>::operator++(int) {  // Post-incremento
  // primero hacemos el incremento del objeto que está invocando el método y
  // después devolvemos una copia del mismo objeto antes de ser incrementado.
  // Esto se hace así para que el operador de post-incremento no sume antes de
  // utilizar el número
  BigInt<Base> aux{1}, result{*this};
  *this = *this + aux;
  return result;
}

template <size_t Base>
BigInt<Base>& BigInt<Base>::operator--() {  // Pre-decremento
  // Como no se puede restar un BigInt con una constante, Inicializamos un
  // BigInt con valor 1 para poder restar aprovechando la sobrecarga del
  // operador
  BigInt<Base> restando{1};
  *this = *this - restando;
  return *this;
}

template <size_t Base>
BigInt<Base> BigInt<Base>::operator--(int) {  // Post-decremento
  BigInt<Base> aux{*this}, restando{1};
  *this = *this - restando;
  return aux;
}

// ====== PRÁCTICA 2: ESPECIALIZACIÓN CLASE BigInt PARA NÚMEROS BINARIOS ======
// Para esta entrega se tiene en cuenta que todo número que nos pasen va a estar
// en complemento a 2

// Función para complementar a 2 el vetor de bool:

template <>
class BigInt<2> {
 public:
  // ========== CONSTRUCTORES ==========
  BigInt(long long n = 0);
  BigInt(std::string& numero);
  BigInt(const char* numero);
  BigInt(const BigInt<2>& numero);
  // Método para complementar a 2 un BigInt
  BigInt<2>& C2();
  // Acceso
  int sign() const;
  bool operator[](int posicion) const;
  // Operadores:
  BigInt<2>& operator=(const BigInt<2>& segundo_numero);
  // Inserción / Extracción
  friend std::ostream& operator<<(std::ostream& os, const BigInt<2>& numero);
  friend std::istream& operator>>(std::istream& is, BigInt<2>& numero);
  // Operadores de comparación
  friend bool operator==(const BigInt<2>& numero1, const BigInt<2>& numero2);
  bool operator!=(const BigInt<2>& numero1) const;
  friend bool operator<(const BigInt<2>& numero1, const BigInt<2>& numero2);
  friend bool operator>(const BigInt<2>& numero1, const BigInt<2>& numero2);
  bool operator>=(const BigInt<2>&) const;
  bool operator<=(const BigInt<2>&) const;
  // Operadores de incremento / decremento
  BigInt<2>& operator++();    // Pre-incremento
  BigInt<2> operator++(int);  // Post-incremento
  BigInt<2>& operator--();    // Pre-decremento
  BigInt<2> operator--(int);  // Post-decremento
  // Operadores aritméticos
  friend BigInt<2> operator+(const BigInt<2>&, const BigInt<2>&);
  BigInt<2> operator-(const BigInt<2>& numero2) const;
  // Operador de cambio de signo
  BigInt<2> operator-() const;
  BigInt<2> operator*(const BigInt<2>& numero2) const;
  friend BigInt<2> operator/(const BigInt<2>& numero1,
                                const BigInt<2>& numero2);
  BigInt<2> operator%(const BigInt<2>& numero2) const;
  // Potencia ab
  friend BigInt<2> pow(const BigInt<2>&, const BigInt<2>&);
  // ==== CONVERSORES DE BINARIO A OTRAS BASES: ====
  // Conversor de binario a octal:
  operator BigInt<8>();
  operator BigInt<10>();
  operator BigInt<16>();

 private:
  // tener en cuenta que en "numero_" los números se guardan al revés
  std::vector<bool> numero_;
};

// =============== CONSTRUCTORES ===============

BigInt<2>::BigInt(long long n) {
  int resto;
  while (n > 9) {
    resto = n % 10;
    n /= 10;
    numero_.emplace_back(resto);
  }
  numero_.emplace_back(n);
}

BigInt<2>::BigInt(std::string& numero) {
  for (int i{numero.size() - 1}; i >= 0; --i) {
    numero_.emplace_back(numero[i] - '0');
  }
}

BigInt<2>::BigInt(const char* numero) {
  while (*numero != '\0') {
    numero_.emplace_back((*numero++ - '0'));
  }
  // Le damos la vuelta para guardarlo al revés
  std::reverse(numero_.begin(), numero_.end());
}

BigInt<2>::BigInt(const BigInt<2>& numero) {
  numero_ = numero.numero_;
}  // Constructor de copia

// Complemento a 2 para realizar ciertas operaciones de manera más visual
BigInt<2>& BigInt<2>::C2() {
  // Se hace la operación NEG de izquierda a derecha por que el número está
  // guardado alrevés
  bool primer_uno_encontrado{
      false};  // variable que nos sirve para decir que el primer 1 ha sido
               // encontrado y hacer NOT de los demás bits
  for (int i{0}; i < numero_.size(); ++i) {
    if (primer_uno_encontrado) {
      if (numero_[i] == 0) {
        numero_[i] = 1;
      } else {
        numero_[i] = 0;
      }
    }
    if (numero_[i] == 1) {
      primer_uno_encontrado = true;
    }
  }
  return *this;
}

// =============== SOBRECARGA DE OPERADOR [] y getter signo ===============

bool BigInt<2>::operator[](int posicion) const {  // Acceso al i-ésimo dígito
  // Usamos la "fórmula" [(numero_.size() - 1) - posicion] para acceder a el
  // número que queremos, esto lo hacemos así ya que el vector de bool del
  // BigInt está dado la vuelta
  return numero_[numero_.size() - 1];
}

int BigInt<2>::sign() const {
  // Si hay un 1 en la primera posición devolvemos un 1, si no es un 1 es
  // positivo y por lo tanto devolvemos un 0
  if (numero_[numero_.size() - 1] == 1) {
    return 1;
  }
  return 0;
}
// ============= SOBRECARGA DE OPERADORES DE INSERCIÓN/EXTRACCIÓN =============

std::istream& operator>>(std::istream& is, BigInt<2>& numero) {
  // Este operador funciona asumiendo que el número que te van a introducir es
  // en binario
  std::string aux_numero;
  is >> aux_numero;
  BigInt<2> numero_final{aux_numero};
  numero.numero_.clear();
  numero = numero_final;
  return is;
}

std::ostream& operator<<(std::ostream& os, const BigInt<2>& numero) {
  for (int i{numero.numero_.size() - 1}; i >= 0; --i) {
    os << numero.numero_[i];
  }
  return os;
}

// =============== SOBRECARGA DE OPERADOR '=' ===============

BigInt<2>& BigInt<2>::operator=(const BigInt<2>& segundo_numero) {
  numero_ = segundo_numero.numero_;
  return *this;
}

// =============== SOBRECARGA DE OPERADORES DE COMPARACIÓN ===============
bool operator==(const BigInt<2>& numero1, const BigInt<2>& numero2) {
  return numero1.numero_ == numero2.numero_;
}

bool BigInt<2>::operator!=(const BigInt<2>& numero1) const {
  return numero_ != numero1.numero_;
}

bool operator>(const BigInt<2>& numero1, const BigInt<2>& numero2) {
  BigInt<2> aux_numero1{numero1}, aux_numero2{numero2};
  BigInt<10> aux_numero1_decimal{aux_numero1},
      aux_numero2_decimal{
          aux_numero2};  // Usamos dos BigInt<10> auxiliares para comprobar si
                         // uno es mayor que el otro
  return aux_numero1_decimal > aux_numero2_decimal;
}

bool operator<(const BigInt<2>& numero1, const BigInt<2>& numero2) {
  BigInt<2> aux_numero1{numero1}, aux_numero2{numero2};
  BigInt<10> aux_numero1_decimal{aux_numero1},
      aux_numero2_decimal{
          aux_numero2};  // Usamos dos BigInt<10> auxiliares para comprobar si
                         // uno es menor que el otro
  return aux_numero1_decimal < aux_numero2_decimal;
}

bool BigInt<2>::operator>=(const BigInt<2>& numero2) const {
  BigInt<2> aux_numero1{*this}, aux_numero2{numero2};
  BigInt<10> aux_numero1_decimal{aux_numero1},
      aux_numero2_decimal{
          aux_numero2};  // Usamos dos BigInt<10> auxiliares para comprobar si
                         // uno es mayor o igual que el otro
  return aux_numero1_decimal >= aux_numero2_decimal;
}

bool BigInt<2>::operator<=(const BigInt<2>& numero2) const {
  BigInt<2> aux_numero1{*this}, aux_numero2{numero2};
  BigInt<10> aux_numero1_decimal{aux_numero1},
      aux_numero2_decimal{
          aux_numero2};  // Usamos dos BigInt<10> auxiliares para comprobar si
                         // uno es menor o igual que el otro
  return aux_numero1_decimal <= aux_numero2_decimal;
}

// =============== OPERADORES ARITMÉTICOS ===============
BigInt<2> operator+(const BigInt<2>& numero1, const BigInt<2>& numero2) {
  int carry = 0;
  BigInt<2> aux_numero1{numero1}, aux_numero2{numero2},
      result;  // resultado como BigInt

  // Rellenando el número más corto con 0s
  int diferencia{aux_numero1.numero_.size() - aux_numero2.numero_.size()};
  if (diferencia < 0) {  // numero1 es el menor de los dos
    for (int i{0}; i < -diferencia; ++i) {
      aux_numero1.numero_.push_back(0);
    }
  } else if (diferencia > 0) {  // numero2 es el menor de los dos
    for (int i = 0; i < diferencia; i++) {
      aux_numero2.numero_.push_back(0);
    }
  }
  // Sumando los números
  for (int i = 0; i < aux_numero1.numero_.size(); i++) {
    int sum = aux_numero1.numero_[i] + aux_numero2.numero_[i] + carry;
    result.numero_.push_back(sum % 2);
    carry = sum / 2;
  }

  // En complemento a 2, si hay un carry final se descarta asi que no lo
  // añadimos al final, pero si los dos números son positivos entonces si se
  // tiene que añadir el carry, por lo que comprobamos si los dos números son
  // positivos mirando si el primer bit de signo de los dos números es '0
  if (aux_numero1.sign() == 0 && aux_numero2.sign() == 0 ||
      (aux_numero1.numero_.size() == 1 && aux_numero2.numero_.size() == 1)) {
      result.numero_.push_back(carry);
      // Al estar los números en C2, si un número empieza en 1 es que es negativo, 
      // pudiendo dar error al convertirse en otra base, ya que se tomaría como un número negativo
      // Por lo que, añadimos un 0 que no cambia el valor de el número y lo "vuelve postivo"
      result.numero_.push_back(0); 
  }

  // Al final de cada operación se añade un 0 adicional al resultado de la suma,
  // por lo que se elimina antes de devolver el resultado invirtiendo el número,
  // quitando el 0 y volviendo a invertir el número
  std::reverse(result.numero_.begin(), result.numero_.end());
  result.numero_.pop_back();
  std::reverse(result.numero_.begin(), result.numero_.end());
  return result;
}

BigInt<2> BigInt<2>::operator-(const BigInt<2>& numero2) const {
  int carry = 0;
  BigInt<2> aux_numero1{*this}, aux_numero2{numero2}, result;

  // Rellenando el número más corto con 0s
  int diferencia = aux_numero1.numero_.size() - aux_numero2.numero_.size();
  if (diferencia < 0) {  // numero2 es el menor de los dos
    for (int i = 0; i < -diferencia; i++) {
      aux_numero1.numero_.push_back(0);
    }
  } else if (diferencia > 0) {  // numero1 es el menor de los dos
    for (int i = 0; i < diferencia; i++) {
      aux_numero2.numero_.push_back(0);
    }
  }
  // Si el número es negativo, se va a convertir en una suma, ej: 34 - (-23) =
  // 34 + 23 Complementamos a 2 para transformar el número en positivo y hacer
  // una suma Si no es negativo, es positivo por lo que hacemos una suma
  // negativa ej: 3 + (-4)
  aux_numero2.C2();
  // Restando los números
  result = aux_numero1 + aux_numero2;
  // Si los dos números son positivos en una suma binaria, se añade el bit de
  // carry, pero en una resta siempre se quita, por lo que comprobamos si los
  // dos números son positivos para quitarles el carry
  if (aux_numero1.sign() == 0 && aux_numero2.sign() == 0) {
    result.numero_.pop_back();
  }

  return result;
}

// Operador cambio de signo
BigInt<2> BigInt<2>::operator-() const {
  // Para cambiar un número binario de signo, se hace el C2. Por lo que
  // simplemente usamos el método que ya tenemos y devolvemos el número
  BigInt<2> aux{*this};
  return aux.C2();
}

// Multiplicación
BigInt<2> BigInt<2>::operator*(const BigInt<2>& numero2) const {
  // Primero comprobamos si se va a multiplicar el número por 0, para en ese caso, devolver un 0
  BigInt<2> aux{"0"}, multiplicando{*this}, resultado, iterador;
  if (numero2 == aux) {
    return aux;
  }
  while (iterador < numero2) {
    resultado = resultado + multiplicando;
    iterador++;
  }
  return resultado;
}

// División
BigInt<2> operator/(const BigInt<2>& numero1, const BigInt<2>& divisor) {
  // Si se cumplen las siguientes dos condiciones, el divisor, que en este caso
  // es "numero2", es 0. Una división por 0 no es posible, por lo que lo
  // indicamos y salimos del programa
  if (divisor.numero_.size() == 1 && divisor.numero_[0] == 0) {
    std::cout << "No es posible realizar una división por 0" << std::endl;
    exit(1);
  }

  // Verificar si el dividendo(numero1) es menor que el divisor(numero2), si
  // esto es así, el cociente de la división dará 0, por lo que retornamos un
  // BigInt con valor 0.
  if (numero1 < divisor) {
    std::cout << "Entró\n";
    return BigInt<2>{};
  }

  // realizar la división
  BigInt<2> dividendo{numero1},
      resultado{};  // Contador que después de hacer restas consecutivas, será
                    // el resultado de la división
  while (dividendo >= divisor) {
    dividendo = dividendo - divisor;
    resultado++;
  }
  return resultado;
}

// Modulo
BigInt<2> BigInt<2>::operator%(const BigInt<2>& numero2) const {
  // Si se cumplen las siguientes dos condiciones, el divisor, que en este
  // caso es "numero2", es 0. Una división por 0 no es posible, por lo que lo
  // indicamos y salimos del programa
  if (numero2.numero_.size() == 1 && numero2.numero_[0] == 0) {
    std::cout << "No es posible realizar una división por 0" << std::endl;
    exit(1);
  }

  // Verificar si el dividendo(numero1) es menor que el divisor(numero2), si
  // esto es así, el cociente de la división dará 0, por lo que retornamos un
  // BigInt con valor 0.
  if (*this < numero2) {
    return BigInt<2>{};
  }

  // realizar la división
  BigInt<2> resta{*this},
      resultado{};  // Contador que después de hacer restas consecutivas, será
                    // el resultado de la división
  while (resta >= numero2) {
    resta = resta - numero2;
    resultado++;
  }

  // Quitamos el 0 sobrante
  resta.numero_.pop_back();
  return resta;
}

// Potencia
BigInt<2> pow(const BigInt<2>& base, const BigInt<2>& exponente) {
  BigInt<2> resultado{1}, contador{};
  while (contador < exponente) {
    resultado = resultado * base;  // vamos haciendo la potencia multiplicando
                                   // el número por si mismo iterativamente
    contador++;
  }
  return resultado;
}

// =============== OPERADORES DE INCREMENTO / DECREMENTO ===============
BigInt<2>& BigInt<2>::operator++() {  // Pre-incremento
  BigInt<2> aux{1};
  *this = *this + aux;
  return *this;
}
BigInt<2> BigInt<2>::operator++(int) {  // Post-incremento
  BigInt<2> aux{1}, copia{*this};
  *this = *this + aux;
  return copia;  // devolvemos la copia del número antes de ser incrementado e
                 // incrementamos antes el original
}

BigInt<2>& BigInt<2>::operator--() {  // Pre-decremento
  BigInt<2> aux{1};
  *this = *this - aux;
  return *this;
}

BigInt<2> BigInt<2>::operator--(int) {  // Post-decremento
  BigInt<2> aux{1}, copia{*this};
  *this = *this - aux;
  return copia;  // devolvemos la copia del número antes de ser incrementado e
                 // incrementamos antes el original
}

// =============== CONVERSORES DE BINARIO A OTRAS BASES ===============

BigInt<2>::operator BigInt<8UL>() {
  BigInt<2> aux{*this};
  std::string numero_convertido_aux{""};
  while (aux.numero_.size() % 3 != 0) {
    aux.numero_.emplace_back(0);
  }
  int valor_digito{0}, exponente{0};
  for (int i{0}; i < aux.numero_.size(); ++i) {
    valor_digito += (pow(2, exponente) * aux.numero_[i]);
    exponente++;
    if (exponente == 3) {  // Para pasar a octal se agrupa de 3 en 3 los bits y
                           // se resetea el exponente
      numero_convertido_aux += (std::to_string(valor_digito));
      valor_digito = 0;
      exponente = 0;
    }
  }
  std::reverse(
      numero_convertido_aux.begin(),
      numero_convertido_aux.end());  // Le damos la vuelta al string para que se
                                     // guarde al revés en el vector de bool
  BigInt<8> numero_convertido{numero_convertido_aux};
  return numero_convertido;
}

BigInt<2>::operator BigInt<10UL>() {
  BigInt<2> aux{*this};
  long long valor_digito{0}, exponente{0};
  for (int i{0}; i < aux.numero_.size(); ++i) {
    if (i < aux.numero_.size() - 1) {
      valor_digito += (pow(2, exponente) * aux.numero_[i]);
      exponente++;
    } else {  // Cómo los números están en complemento a 2, para pasarlos a
              // decimal se resta la última potencia de 2
      valor_digito -= (pow(2, exponente) * aux.numero_[i]);
    }
  }
  std::string aux2{std::to_string(valor_digito)};
  BigInt<10> numero_convertido{aux2};
  return numero_convertido;
}

BigInt<2>::operator BigInt<16UL>() {
  BigInt<2> aux{*this};
  std::string numero_convertido_aux{""};
  while (aux.numero_.size() % 4 != 0) {
    aux.numero_.emplace_back(0);
  }
  int valor_digito{0}, exponente{0};
  for (int i{0}; i < aux.numero_.size(); ++i) {
    valor_digito += (pow(2, exponente) * aux.numero_[i]);
    exponente++;
    if (exponente == 4) {  // Para pasar a hexadecimal se agrupa de 4 en 4 los
                           // bits y se resetea el exponente
      switch (valor_digito) {
        case 10:
          numero_convertido_aux.push_back('A');
          break;
        case 11:
          numero_convertido_aux.push_back('B');
          break;
        case 12:
          numero_convertido_aux.push_back('C');
          break;
        case 13:
          numero_convertido_aux.push_back('D');
          break;
        case 14:
          numero_convertido_aux.push_back('E');
          break;
        case 15:
          numero_convertido_aux.push_back('F');
          break;
        default:
          numero_convertido_aux += std::to_string(valor_digito);
          break;
      }
      valor_digito = 0;
      exponente = 0;
    }
  }
  std::reverse(
      numero_convertido_aux.begin(),
      numero_convertido_aux.end());  // Le damos la vuelta al string para que se
                                     // guarde al revés en el vector de bool
  BigInt<16> numero_convertido{numero_convertido_aux};
  return numero_convertido;
}

#endif
