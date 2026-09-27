// LoggerI.cpp: implementacao do servant. ARQUIVO DA ENTREGA.
// Esqueleto com os metodos vazios (o que o tao_idl -Gstl -GI gera).
// Pessoa 2: preencher conforme a secao 5 do plano. FEITO

#include "LoggerI.h"
#include <iostream>
#include<ctime>

Logger_i::Logger_i (void)
{
}

Logger_i::~Logger_i (void)
{
}

// Funcao auxiliar para exibir o nome da severidade em vez de um numero.
const char* Logger_i::severidade_para_string(::Severidade s){
  switch (s)
  {
  case ::DEBUG: return "DEBUG";
  case ::WARNING: return "WARNING";
  case ::ERROR: return "ERROR";
  case ::CRITICAL: return "CRITICAL";
  default: return "UNKNOWN";
  }
}

void Logger_i::log (
    ::Severidade severidade,
    std::string endereco,
    ::CORBA::UShort pid,
    ::CORBA::Long hora,
    std::string msg)
{
  // Guardar o ultimo endereco recebido para esta severidade
  this ->ultimos_enderecos_[severidade] = endereco;

  // Converter o timestamp (segundos) para formato legivel de data e hora
  std::time_t tempo = static_cast < std::time_t>(hora);
  char buffer_data[100];
  std::strftime(buffer_data, sizeof(buffer_data), "%Y-%m-%d %H:%M:%S", std::localtime(&tempo));

  // Imprimir os cinco campos na tela
  std::cout <<"==================================================" << std::endl;
  std::cout << "[LOG RECEBIDO]" << std::endl;
  std::cout << "Severidade : " << this-> severidade_para_string(severidade) << std::endl;
  std::cout << "Endereço : " << endereco << std::endl;
  std::cout << "PID : " << pid << std::endl;
  std::cout << "Hora : " << buffer_data << " (" hora << "s)"<< std::endl;

}

std::string Logger_i::locate (
    ::Severidade s)
{
  // Procurar o ultimo endereco de severidade 's' no map
  std::map < ::Severidade, std::string >::iterator it = this-> ultimos_enderecos_.
  return "";
}
