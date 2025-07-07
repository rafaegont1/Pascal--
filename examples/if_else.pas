program ifelse;

var
  a, b: integer;
begin
  a := 2;
  b := 3;

  write("a := ");
  writeln(a);
  write("b := ");
  writeln(b);

  if a > b then
    writeln("a > b");
  else if a = b then
    writeln("a = b");
  else
    writeln("a < b");
end.
