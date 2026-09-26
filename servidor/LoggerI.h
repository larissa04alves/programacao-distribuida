// LoggerI.h: declaracao do servant (a classe que implementa a interface Logger).
// Mesmo formato que o tao_idl -Gstl -GI gera: herda do skeleton POA_Logger.
// Com -Gstl, os strings do IDL viram std::string (e nao char*).
// Quem implementa os metodos e a Pessoa 2, em LoggerI.cpp.

#ifndef LOGGERI_H_
#define LOGGERI_H_

#include <string>
#include "LoggerS.h"

class Logger_i : public virtual POA_Logger
{
public:
  Logger_i (void);
  virtual ~Logger_i (void);

  // oneway void log(in Severidade, in string, in unsigned short, in long, in string)
  virtual void log (
      ::Severidade severidade,
      std::string endereco,
      ::CORBA::UShort pid,
      ::CORBA::Long hora,
      std::string msg);

  // string locate(in Severidade) raises (SeveridadeInexistente)
  virtual std::string locate (
      ::Severidade s);
};

#endif /* LOGGERI_H_ */
