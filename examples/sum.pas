program SomaSimples;
var
  i, soma: integer;
begin
  soma := 0;
  for i := 1 to 5 do
    soma := soma + i;
  writeln('Soma = ', soma);
end.
