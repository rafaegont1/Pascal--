program tye;

var
  a, b, c: integer;
  x, y, z: real;
  msg1, msg2: string;

begin
  { Integer operations }
  a := 10;
  b := 3;
  c := a + b;
  writeln("Integer addition: ", c);
  
  c := a - b;
  writeln("Integer subtraction: ", c);
  
  c := a * b;
  writeln("Integer multiplication: ", c);
  
  c := a div b;
  writeln("Integer division: ", c);
  
  c := a mod b;
  writeln("Integer modulo: ", c);
  
  { Real operations }
  x := 10.5;
  y := 3.2;
  z := x + y;
  writeln("Real addition: ", z);
  
  z := x - y;
  writeln("Real subtraction: ", z);
  
  z := x * y;
  writeln("Real multiplication: ", z);
  
  z := x / y;
  writeln("Real division: ", z);
  
  { Mixed operations (integer to real) }
  z := a + x;
  writeln("Mixed addition (int + real): ", z);
  
  z := x + a;
  writeln("Mixed addition (real + int): ", z);
  
  { String operations }
  msg1 := "Hello";
  msg2 := "World";
  msg1 := msg1 + msg2;
  writeln("String concatenation: ", msg1);
  
  { Comparisons }
  if a > b then
    writeln("Integer comparison (a > b): true");
  
  if x > y then
    writeln("Real comparison (x > y): true");
  
  if a = 10 then
    writeln("Integer equality (a = 10): true");
  
  if x = 10.5 then
    writeln("Real equality (x = 10.5): true");
  
  if msg1 = "HelloWorld" then
    writeln("String equality: true");
  
  { Type conversion tests }
  c := 15;
  x := c;  { integer to real }
  writeln("Integer to real conversion: ", x);
  
  { Edge cases }
  if a <> b then
    writeln("Integer inequality (a <> b): true");
  
  if x >= y then
    writeln("Real greater or equal (x >= y): true");
  
  if a <= 15 then
    writeln("Integer less or equal (a <= 15): true");
  
  writeln("Type operations test completed!");
end. 