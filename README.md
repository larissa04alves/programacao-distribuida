# Logger distribuído (CORBA / ACE+TAO)

Trabalho de Sistemas Distribuídos (PUCPR). Servidor CORBA `Logger` que recebe eventos
de clientes na rede pela operação `log()` (oneway) e responde `locate(s)` com o endereço
do último evento da severidade `s`. A referência do servidor é publicada no Naming Service.

## Estrutura

```
idl/Logger.idl         contrato (entrega)
servidor/LoggerI.cpp   servant (entrega)
servidor/servidor.cpp  inicializa o ORB e publica o Logger no Naming Service
cliente/cliente.cpp    testa toda a interface
bin/                   executáveis
```

## Compilar (na VM, com ACE_ROOT e TAO_ROOT definidos)

```
cd servidor && make
cd cliente && make
```

O `make` roda o `tao_idl` sozinho quando o IDL muda. `make cleanall` apaga os gerados.

## Rodar

```
tao_cosnaming -ORBEndpoint iiop://localhost:1050
./bin/servidor -ORBInitRef NameService=corbaloc:iiop:localhost:1050/NameService
./bin/cliente  -ORBInitRef NameService=corbaloc:iiop:localhost:1050/NameService
```
## Implementação do Servant (LoggerI) - Pessoa 2

A classe `Logger_i` gerencia o estado do servidor e processa as chamadas remotas dos clientes:

### Estrutura de Dados
- **`ultimos_enderecos_`**: Um `std::map< ::Severidade, std::string >` mantido em memória para registrar o endereço IP/porta do último evento recebido para cada nível de severidade.

### Métodos Implementados
1. **`log(...)`**:
   - Atualiza o registro interno (`ultimos_enderecos_`) com o endereço recebido.
   - Formata o timestamp Unix recebido para data/hora legível (`YYYY-MM-DD HH:MM:SS`).
   - Imprime no terminal os dados do evento (Severidade, Endereço, PID, Hora e Mensagem).

2. **`locate(Severidade s)`**:
   - Realiza a busca no mapa pelo último endereço da severidade enviada.
   - **Lança a exceção `SeveridadeInexistente`** caso nenhum evento com essa severidade tenha sido recebido previamente.
   - Retorna a `std::string` correspondente ao endereço caso seja localizado.
