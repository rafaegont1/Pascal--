program readTypeTest;

var
  a: integer;
  b: real;
  c: string;

begin
  writeln("Testando leitura com tipos corretos:");
  
  { Estas leituras devem funcionar corretamente }
  writeln("Digite um inteiro para a:");
  read(a);
  
  writeln("Digite um real para b:");
  read(b);
  
  writeln("Digite uma string para c:");
  read(c);
  
  writeln("Valores lidos:");
  writeln("a = ", a);
  writeln("b = ", b);
  writeln("c = ", c);
  
  writeln("Teste concluído!");
end. 