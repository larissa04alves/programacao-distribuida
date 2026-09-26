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
