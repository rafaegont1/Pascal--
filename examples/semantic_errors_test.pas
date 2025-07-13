program semanticErrors;

var
  a: integer;
  b: real;
  c: string;
  d: integer;

begin
  { Problema 1: Comparação com tipos diferentes onde não faz sentido }
  writeln("Testando comparações com tipos incompatíveis:");
  
  { Comparação entre integer e string - deveria dar erro semântico }
  if a = "hello" then
    writeln("Erro: comparando integer com string");
  
  { Comparação entre real e string - deveria dar erro semântico }
  if b > "test" then
    writeln("Erro: comparando real com string");
  
  { Comparação entre string e integer - deveria dar erro semântico }
  if c < 42 then
    writeln("Erro: comparando string com integer");
  
  { Problema 2: Leitura de tipo diferente dos da variável nos comandos CALL READ }
  writeln("\nTestando leitura com tipos incompatíveis:");
  
  { Tentando ler string para variável integer - deveria dar erro semântico }
  read(a);  { Se o usuário digitar "abc", deveria dar erro }
  
  { Tentando ler string para variável real - deveria dar erro semântico }
  read(b);  { Se o usuário digitar "xyz", deveria dar erro }
  
  { Tentando ler número para variável string - deveria dar erro semântico }
  read(c);  { Se o usuário digitar 123, deveria dar erro }
  
  writeln("Testes de erros semânticos concluídos!");
end. 