// servidor.cpp: PLACEHOLDER da Pessoa 1, existe so para o esqueleto compilar e linkar.
//
// Pessoa 3: substituir este arquivo pelo servidor de verdade (secao 4 do plano):
//   1. ORB_init  2. RootPOA + activate  3. Logger_i logger_i  4. _this()
//   5. rebind no Naming Service  6. orb->run()  7. destroy
// tudo dentro de try/catch (CORBA::Exception&).

#include <iostream>
#include "LoggerI.h"

int main (int argc, char * argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "ORB");

    Logger_i logger_i;   // confirma que o servant linka com o skeleton

    std::cout << "servidor: esqueleto compilado e linkado. "
              << "Falta o servidor.cpp da Pessoa 3." << std::endl;

    orb->destroy ();
  }
  catch (CORBA::Exception & e)
  {
    std::cerr << "Erro CORBA no servidor: " << e._name () << std::endl;
    return 1;
  }
  return 0;
}
