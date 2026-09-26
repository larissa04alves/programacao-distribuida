// cliente.cpp: PLACEHOLDER da Pessoa 1, existe so para o esqueleto compilar e linkar.
//
// Pessoa 4: substituir este arquivo pelo cliente de verdade (secao 6 do plano):
//   ORB_init -> resolve "Logger" no Naming Service -> Logger::_narrow ->
//   locate(CRITICAL) esperando a excecao -> log nas 4 severidades -> sleep(1) ->
//   locate nas 4 severidades -> orb->destroy.
//
// Convencao de linha de comando:
//   ./bin/cliente -ORBInitRef NameService=corbaloc:iiop:localhost:1050/NameService [ip:porta]
// O ORB_init consome os argumentos -ORB*, entao depois dele argv[1] e o endereco ficticio
// que este cliente vai usar nos log().

#include <iostream>
#include "LoggerC.h"

int main (int argc, char * argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "ORB");

    const char * endereco = (argc > 1) ? argv[1] : "192.168.1.1:1500";

    std::cout << "cliente [" << endereco << "]: esqueleto compilado e linkado. "
              << "Falta o cliente.cpp da Pessoa 4." << std::endl;

    orb->destroy ();
  }
  catch (CORBA::Exception & e)
  {
    std::cerr << "Erro CORBA no cliente: " << e._name () << std::endl;
    return 1;
  }
  return 0;
}
