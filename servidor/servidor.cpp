// servidor.cpp: inicializa o ORB, ativa o servant e publica o Logger no Naming Service

#include <iostream>
#include "LoggerI.h"
#include "orbsvcs/CosNamingC.h"

int main (int argc, char * argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "ORB");

    CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_obj.in ());
    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
    poa_manager->activate ();

    Logger_i logger_i;
    Logger_var logger = logger_i._this ();

    CORBA::Object_var ns_obj =
        orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (ns_obj.in ());

    CosNaming::Name name (1);
    name.length (1);
    name[0].id   = CORBA::string_dup ("Logger");
    name[0].kind = CORBA::string_dup ("");

    naming_context->rebind (name, logger.in ());

    std::cout << "Servidor Logger no ar, publicado no Naming Service como 'Logger'." << std::endl;

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
