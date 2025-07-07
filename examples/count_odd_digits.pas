program countOddDigits;

var
  n, contadorImpar, nTemp, digito, resto, ehImpar: integer;

begin
  write("Digite um numero:");
  readln(n);

  contadorImpar := 0;
  nTemp := n;

  while nTemp > 0 do
  begin
    digito := nTemp mod 10;
    resto := digito mod 2;
    ehImpar := resto = 1;

    if ehImpar then
      contadorImpar := contadorImpar + 1;

    nTemp := nTemp div 10;
  end;

  write("Quantidade de dígitos ímpares: ");
  writeln(contadorImpar);
end.
