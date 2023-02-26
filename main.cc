#include "big_int.hpp"
#include "board_especializacion.hpp"

int main(int argc, char* argv[]) {
  std::string nombre_fichero(argv[1]), primera_linea;
  std::ifstream fichero;
  fichero.open(nombre_fichero.c_str(), std::ios::in);
  fichero >> primera_linea;
  fichero >> primera_linea;
  fichero >> primera_linea;
  int base{std::stoi(primera_linea)};
  switch (base) {
    case 2: {
      Board<2> pizarra_base2{nombre_fichero};
      pizarra_base2.Start();
      break;
    }
    case 8: {
      Board<8> pizarra_base8{nombre_fichero};
      pizarra_base8.Start();
      break;
    }
    case 10: {
      Board<10> pizarra_base10{nombre_fichero};
      pizarra_base10.Start();
      break;
    }
    case 16: {
      Board<16> pizarra_base16{nombre_fichero};
      pizarra_base16.Start();
      break;
    }
    default:
      std::cerr << "La base " << base
                << " no es compatible con el tipo de dato BigInt" << std::endl;
      exit(1);
      break;
  }
}