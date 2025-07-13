program arithmeticTypeTest;

var
  a: integer;
  b: real;
  c: string;
  d: integer;

begin
  writeln("Testando operações aritméticas com tipos corretos:");
  
  { Operações válidas }
  a := 10;
  b := 3.5;
  c := "hello";
  
  d := a + 5;        { integer + integer = integer }
  writeln("a + 5 = ", d);
  
  b := b + 2.5;      { real + real = real }
  writeln("b + 2.5 = ", b);
  
  b := a + b;        { integer + real = real }
  writeln("a + b = ", b);
  
  c := c + " world"; { string + string = string }
  writeln("c + ' world' = ", c);
  
  writeln("\nTestando operações inválidas (devem dar erro):");
  
  { Estas operações devem gerar erro de runtime }
  d := a + c;        { integer + string = erro }
  d := c + a;        { string + integer = erro }
  d := b + c;        { real + string = erro }
  
  writeln("Teste concluído!");
end. 