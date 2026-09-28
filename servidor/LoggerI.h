// LoggerI.h: servant que implementa a interface Logger

#ifndef LOGGERI_H_
#define LOGGERI_H_

#include <string>
#include <map>
#include "LoggerS.h"

class Logger_i : public virtual POA_Logger
{
public:
  Logger_i (void);
  virtual ~Logger_i (void);

  virtual void log (
      ::Severidade severidade,
      std::string endereco,
      ::CORBA::UShort pid,
      ::CORBA::Long hora,
      std::string msg);

  virtual std::string locate (
      ::Severidade s);

private:
  // ultimo endereco recebido de cada severidade
  std::map< ::Severidade, std::string > ultimos_enderecos_;

  const char* severidade_para_string(::Severidade s);
};

#endif /* LOGGERI_H_ */