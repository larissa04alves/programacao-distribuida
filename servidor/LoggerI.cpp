// LoggerI.cpp: implementacao do servant Logger

#include "LoggerI.h"
#include <iostream>
#include <ctime>

Logger_i::Logger_i (void)
{
}

Logger_i::~Logger_i (void)
{
}

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
  this->ultimos_enderecos_[severidade] = endereco;

  // converte os segundos desde 1/1/1970 em data e hora legiveis
  std::time_t tempo = static_cast<std::time_t>(hora);
  char buffer_data[100];
  std::strftime(buffer_data, sizeof(buffer_data), "%Y-%m-%d %H:%M:%S", std::localtime(&tempo));

  std::cout << "==================================================" << std::endl;
  std::cout << "[LOG RECEBIDO]" << std::endl;
  std::cout << "Severidade : " << this->severidade_para_string(severidade) << std::endl;
  std::cout << "Endereco : " << endereco << std::endl;
  std::cout << "PID : " << pid << std::endl;
  std::cout << "Hora : " << buffer_data << " (" << hora << "s)" << std::endl;
  std::cout << "Mensagem : " << msg << std::endl;
}

std::string Logger_i::locate (
    ::Severidade s)
{
  std::map< ::Severidade, std::string >::iterator it = this->ultimos_enderecos_.find(s);
  if (it == this->ultimos_enderecos_.end())
  {
    throw ::SeveridadeInexistente();
  }
  return it->second;
}
