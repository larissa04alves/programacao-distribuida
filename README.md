# Logger distribuído (CORBA / ACE+TAO)

Trabalho da disciplina de Sistemas Distribuídos, Escola Politécnica, PUCPR.

**Grupo T1-15:** Alana da Conceição Queiroz, Larissa Alves da Silva,
Leticia Maria Maia de Andrade Vieira, Thomas Manussadjian Steinhausser.

## O problema

Um servidor CORBA chamado `Logger` recebe notificações de eventos vindas de clientes
espalhados na rede. Cada evento tem uma severidade (`DEBUG`, `WARNING`, `ERROR` ou
`CRITICAL`), o endereço `ip:porta` de onde veio, o PID do processo, a hora e uma mensagem.

O Logger imprime na tela cada evento recebido e guarda, para cada severidade, o endereço
do último evento. A operação `locate(s)` devolve esse endereço, ou lança a exceção
`SeveridadeInexistente` se ainda não chegou nenhum evento com a severidade `s`.

A referência do Logger é publicada no Servidor de Nomes (Naming Service) com o nome
`Logger`. Os clientes a obtêm de lá, sem precisar de arquivo `.ior`.

## A interface (Logger.idl)

```idl
enum Severidade { DEBUG, WARNING, ERROR, CRITICAL };

exception SeveridadeInexistente {};

interface Logger {
    oneway void log(in Severidade severidade, in string endereco,
                    in unsigned short pid, in long hora, in string msg);
    string locate(in Severidade s) raises (SeveridadeInexistente);
};
```

- `log` é `oneway`: o cliente envia e não espera resposta (notificação assíncrona).
  Por isso não tem retorno nem parâmetros de saída.
- `locate` é uma chamada normal, com retorno e exceção declarada no `raises`.

## Estrutura do projeto

```
idl/Logger.idl         contrato da interface (arquivo da entrega)
servidor/LoggerI.h     declaração do servant Logger_i
servidor/LoggerI.cpp   implementação do servant (arquivo da entrega)
servidor/servidor.cpp  inicializa o ORB, ativa o servant e publica no Naming Service
servidor/Makefile      Makefile do exemplo Conta do professor, adaptado
cliente/cliente.cpp    cliente que testa toda a interface
cliente/Makefile       idem, versão cliente
bin/                   executáveis gerados
```

Os arquivos `LoggerC.*` (stub) e `LoggerS.*` (skeleton) são gerados em `idl/` pelo
`tao_idl` durante o `make` e não ficam no Git.

## Como funciona

**Servant (`LoggerI.cpp`).** A classe `Logger_i` herda do skeleton `POA_Logger` e guarda
um `std::map<Severidade, std::string>` com o último endereço de cada severidade.
`log` atualiza o mapa e imprime os cinco campos, com a hora convertida para data legível.
`locate` procura no mapa e lança `SeveridadeInexistente` se não encontrar.

**Servidor (`servidor.cpp`).** Segue os passos vistos em aula: inicializa o ORB, obtém e
ativa o RootPOA, instancia o servant, obtém a referência com `_this()`, registra no
Naming Service com `rebind` (para não falhar se o nome já existir) e entra em `orb->run()`.

**Cliente (`cliente.cpp`).** Resolve o nome `Logger` no Naming Service, faz o `_narrow`
e testa a interface em três passos:

1. `locate(CRITICAL)` antes de qualquer `log`, esperando a exceção.
2. `log` nas quatro severidades, com dados fictícios e o endereço passado na linha de comando.
3. `locate` nas quatro severidades, conferindo o endereço devolvido.

Entre os passos 2 e 3 há um `sleep(1)`: como `log` é `oneway`, o cliente não espera o
servidor processar, e um `locate` imediato poderia chegar antes do `log`.

## Compilar

Na VM da disciplina, com `ACE_ROOT` e `TAO_ROOT` definidos:

```
cd servidor && make
cd cliente && make
```

O `make` roda o `tao_idl -Gstl` sozinho quando o IDL muda. `make cleanall` apaga
objetos, executáveis e arquivos gerados.

## Executar

Na VM o Naming Service já está no ar na porta 2809 (`pgrep -af cosnaming` mostra).
Em terminais separados:

```
./bin/servidor -ORBInitRef NameService=corbaloc:iiop:localhost:2809/NameService
./bin/cliente  -ORBInitRef NameService=corbaloc:iiop:localhost:2809/NameService 192.168.1.1:1500
./bin/cliente  -ORBInitRef NameService=corbaloc:iiop:localhost:2809/NameService 192.168.1.2:1600
```

O último argumento do cliente é o endereço fictício que ele envia nos `log()`. Rodar
dois ou três clientes com endereços diferentes reproduz a figura do enunciado: o
primeiro recebe a exceção no passo 1, o segundo já encontra o `CRITICAL` do primeiro,
e o `locate` passa a devolver o endereço do cliente mais recente.

## Saída esperada

No cliente:

```
[1] locate(CRITICAL) antes de enviar qualquer log
    excecao SeveridadeInexistente recebida, como esperado

[2] enviando log() nas quatro severidades
    log(DEBUG, 192.168.1.1:1500, 7184, 1790619513, "iniciando servico")
    ...

[3] locate() nas quatro severidades
    locate(DEBUG) = 192.168.1.1:1500  (confere com o enviado)
    ...
```

No servidor, um bloco por evento:

```
[LOG RECEBIDO]
Severidade : DEBUG
Endereco : 192.168.1.1:1500
PID : 7184
Hora : 2026-09-28 14:38:33 (1790619513s)
Mensagem : iniciando servico
```

## Entrega

`idl/Logger.idl` e `servidor/LoggerI.cpp`.
