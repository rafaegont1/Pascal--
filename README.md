# Pascal--

## Dependências

Para compilar o projeto é necessário instalar os seguintes pacotes (supondo que você esteja usando uma distribuição baseada em debian):

```sudo apt install g++ cmake```

## Compilação

A compilação irá gerar o executável `pascal--` dentro da pasta `build`. Para tanto, é necessário executar, a partir do diretório do repositório local, os seguintes comandos:

```mkdir build```

```cd build```

```cmake ..```

```make```

## Uso

Partindo do pressuposto que você está no diretório do repositório local:

```./build/pascal-- ../examples/hello.pas```

Com isso, será gerada, como saída, uma lista contendo (token, lexema, linha, coluna):

```
(PROGRAM, "program", 1, 1)
(VAR_NAME, "hello", 1, 9)
(SEMICOLON, ";", 1, 14)
(BEGIN, "begin", 5, 1)
(WRITELN, "writeln", 6, 3)
(OPEN_PARENTHESES, "(", 6, 11)
(LITERAL_STRING, "Hello, world.", 6, 12)
(CLOSE_PARENTHESES, ")", 6, 27)
(SEMICOLON, ";", 6, 28)
(END, "end", 7, 1)
(PERIOD, ".", 7, 4)
```
