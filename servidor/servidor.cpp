// servidor.cpp: PLACEHOLDER da Pessoa 1, existe so para o esqueleto compilar e linkar.
//
// Pessoa 3: substituir este arquivo pelo servidor de verdade (secao 4 do plano):
//   1. ORB_init  2. RootPOA + activate  3. Logger_i logger_i  4. _this()
//   5. rebind no Naming Service  6. orb->run()  7. destroy
// tudo dentro de try/catch (CORBA::Exception&).

#include <iostream>
#include "LoggerI.h"
#include "orbsvcs/CosNamingC.h"

int main (int argc, char * argv[])
{
  try
  {
    // inicializa o ORB		  
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "ORB");

    // ativa o RootPOA
    CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_obj.in ());
    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
    poa_manager->activate ();

    Logger_i logger_i;   // confirma que o servant linka com o skeleton
    
    // registra no POA e obtem a referencia CORBA
    Logger_var logger = logger_i._this ();

    // publica no Naming Service
    CORBA::Object_var ns_obj =
        orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (ns_obj.in ());

    CosNaming::Name name (1);
    name.length (1);
    name[0].id   = CORBA::string_dup ("Logger");
    name[0].kind = CORBA::string_dup ("");

    naming_context->rebind (name, logger.in ());


    std::cout << "servidor: esqueleto compilado e linkado. "
              << "Falta o servidor.cpp da Pessoa 3." << std::endl;

    orb->run ();

    root_poa->destroy (true, true);
    orb->destroy ();
  }
  catch (CORBA::Exception & e)
  {
    std::cerr << "Erro CORBA no servidor: " << e._name () << std::endl;
    return 1;
  }
  return 0;
}
