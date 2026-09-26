// LoggerI.cpp: implementacao do servant. ARQUIVO DA ENTREGA.
// Esqueleto com os metodos vazios (o que o tao_idl -Gstl -GI gera).
// Pessoa 2: preencher conforme a secao 5 do plano.

#include "LoggerI.h"

Logger_i::Logger_i (void)
{
}

Logger_i::~Logger_i (void)
{
}

void Logger_i::log (
    ::Severidade severidade,
    std::string endereco,
    ::CORBA::UShort pid,
    ::CORBA::Long hora,
    std::string msg)
{
  // TODO (Pessoa 2): imprimir os cinco campos na tela e guardar
  //                  o ultimo endereco por severidade.
}

std::string Logger_i::locate (
    ::Severidade s)
{
  // TODO (Pessoa 2): devolver o endereco do ultimo evento com severidade s,
  //                  ou lancar SeveridadeInexistente().
  return "";
}
