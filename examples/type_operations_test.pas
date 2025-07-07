program type;

var
  a, b, c: integer;
  x, y, z: real;
  msg1, msg2: string;

begin
  { Integer operations }
  writeln("---> Integer operations <---");
  a := 10;
  b := 3;

  write("a = ");
  writeln(a);
  write("b = ");
  writeln(b);

  c := a + b;
  write("a + b = ");
  writeln(c);

  c := a - b;
  write("a - b = ");
  writeln(c);

  c := a * b;
  write("a * b = ");
  writeln(c);

  c := a div b;
  write("a div b = ");
  writeln(c);

  c := a mod b;
  write("a mod b = ");
  writeln(c);

  { Real operations }
  writeln("\n---> Real operations <---");
  x := 10.5;
  y := 3.2;

  write("x = ");
  writeln(x);
  write("y = ");
  writeln(y);

  z := x + y;
  write("x + y = ");
  writeln(z);

  z := x - y;
  write("x - y = ");
  writeln(z);

  z := x * y;
  write("x * y = ");
  writeln(z);

  z := x / y;
  write("x / y = ");
  writeln(z);

  { Mixed operations (integer to real) }
  writeln("\n---> Mixed operations (integer to real) <---");
  z := a + x;
  write("a + x = ");
  writeln(z);

  z := x + a;
  write("x + a = ");
  writeln(z);

  { Comparisons }
  writeln("\n---> Comparisons <---");

  a := 1;
  b := 2;
  x := 3.0;
  y := 4.0;

  write("a = ");
  writeln(a);
  write("b = ");
  writeln(b);
  write("x = ");
  writeln(x);
  write("y = ");
  writeln(y);

  if a > b then
    writeln("Integer comparison (a > b): true");
  else
    writeln("Integer comparison (a > b): false");

  if x > y then
    writeln("Real comparison (x > y): true");
  else
    writeln("Real comparison (x > y): false");

  if a = 10 then
    writeln("Integer equality (a = 10): true");
  else
    writeln("Integer equality (a = 10): false");

  if x = 10.5 then
    writeln("Real equality (x = 10.5): true");
  else
    writeln("Real equality (x = 10.5): false");

  if a <> b then
    writeln("Integer inequality (a <> b): true");
  else
    writeln("Integer inequality (a <> b): false");

  if x >= y then
    writeln("Real greater or equal (x >= y): true");
  else
    writeln("Real greater or equal (x >= y): false");

  if a <= 15 then
    writeln("Integer less or equal (a <= 15): true");
  else
    writeln("Integer less or equal (a <= 15): false");

  { Type conversion tests }
  writeln("\n---> Type conversion tests <---");

  x := 15.2;
  write("x = ");
  writeln(x);

  c := x;  { real to integer }
  write("c = x = ");
  writeln(c);

  writeln("Type operations test completed!");
end.
