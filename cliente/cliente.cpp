// cliente.cpp: testa toda a interface do Logger (log, locate e a excecao)
// uso: ./bin/cliente -ORBInitRef NameService=corbaloc:iiop:localhost:1050/NameService [ip:porta]

#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include "LoggerC.h"
#include "orbsvcs/CosNamingC.h"

static const char * nome (::Severidade s)
{
  switch (s)
  {
    case ::DEBUG:    return "DEBUG";
    case ::WARNING:  return "WARNING";
    case ::ERROR:    return "ERROR";
    case ::CRITICAL: return "CRITICAL";
    default:         return "?";
  }
}

int main (int argc, char * argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "ORB");

    std::string endereco = (argc > 1) ? argv[1] : "192.168.1.1:1500";

    CORBA::Object_var ns_obj = orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (ns_obj.in ());

    CosNaming::Name name (1);
    name.length (1);
    name[0].id   = CORBA::string_dup ("Logger");
    name[0].kind = CORBA::string_dup ("");

    CORBA::Object_var obj = naming_context->resolve (name);
    Logger_var logger = Logger::_narrow (obj.in ());

    if (CORBA::is_nil (logger.in ()))
    {
      std::cerr << "cliente: nao foi possivel obter a referencia do Logger" << std::endl;
      return 1;
    }

    std::cout << "cliente [" << endereco << "] conectado ao Logger" << std::endl;

    // 1. locate antes de qualquer log: deve lancar SeveridadeInexistente
    std::cout << "\n[1] locate(CRITICAL) antes de enviar qualquer log" << std::endl;
    try
    {
      std::string r = logger->locate (::CRITICAL);
      std::cout << "    servidor ja tinha um CRITICAL de outro cliente: " << r << std::endl;
    }
    catch (::SeveridadeInexistente &)
    {
      std::cout << "    excecao SeveridadeInexistente recebida, como esperado" << std::endl;
    }

    // 2. log nas quatro severidades
    std::cout << "\n[2] enviando log() nas quatro severidades" << std::endl;
    ::Severidade severidades[] = { ::DEBUG, ::WARNING, ::ERROR, ::CRITICAL };
    const char * mensagens[]   = { "iniciando servico",
                                   "uso de memoria acima de 80%",
                                   "falha ao abrir arquivo de configuracao",
                                   "disco cheio, servico parado" };

    CORBA::UShort pid  = static_cast<CORBA::UShort> (getpid () & 0xFFFF);
    CORBA::Long   hora = static_cast<CORBA::Long> (std::time (NULL));

    for (int i = 0; i < 4; ++i)
    {
      logger->log (severidades[i], endereco, pid, hora + i, mensagens[i]);
      std::cout << "    log(" << nome (severidades[i]) << ", " << endereco
                << ", " << pid << ", " << hora + i << ", \"" << mensagens[i] << "\")"
                << std::endl;
    }

    // log e oneway: da tempo para o servidor processar antes do locate
    sleep (1);

    // 3. locate nas quatro severidades
    std::cout << "\n[3] locate() nas quatro severidades" << std::endl;
    for (int i = 0; i < 4; ++i)
    {
      try
      {
        std::string r = logger->locate (severidades[i]);
        std::cout << "    locate(" << nome (severidades[i]) << ") = " << r
                  << (r == endereco ? "  (confere com o enviado)"
                                    : "  (outro cliente enviou depois)")
                  << std::endl;
      }
      catch (::SeveridadeInexistente &)
      {
        std::cout << "    locate(" << nome (severidades[i])
                  << ") lancou SeveridadeInexistente (inesperado)" << std::endl;
      }
    }

    orb->destroy ();
  }
  catch (CosNaming::NamingContext::NotFound &)
  {
    std::cerr << "cliente: nome 'Logger' nao encontrado no Naming Service. "
              << "O servidor esta no ar?" << std::endl;
    return 1;
  }
  catch (CORBA::Exception & e)
  {
    std::cerr << "Erro CORBA no cliente: " << e._name () << std::endl;
    return 1;
  }
  return 0;
}
