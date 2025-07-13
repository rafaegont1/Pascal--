program forprogram;

var
  i, soma: integer;

begin
  for i := 1 to 5 do
  begin
    writeln("continuing...");
    continue;
    soma := soma + i;
  end;
  writeln("Soma = ", soma);
end.
