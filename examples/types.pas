program types;
var
  i: integer;
  r: real;
  s: string;
begin
  i := 1;
  r := 1.0;
  s := "a";

  i := 1 + 1.0;
  // i := 1 + "str";
  // i := 1.0 + "str";
  i := i + i;
  i := i + r;
  // i := i + s;
  i := r + r;
  // i := r + s;
  // i := s + s;

  r := 1 + 1.0;
  // r := 1 + "str";
  // r := 1.0 + "str";
  r := i + i;
  r := i + r;
  // r := i + s;
  r := r + r;
  // r := r + s;
  // r := s + s;

  // s := 1 + 1.0;
  // s := 1 + "str";
  // s := 1.0 + "str";
  // s := i + i;
  // s := i + r;
  // s := i + s;
  // s := r + r;
  // s := r + s;
  s := s + s;

  writeln(i);
  writeln(r);
  writeln(s);
end.
