program execution;
var
    i, j, k: integer;
    r, s, t: real;
    str1, str2: string;
    result: integer;
begin
    writeln("=== EXECUTION TEST START ===");

    // Test 1: Basic integer arithmetic
    writeln("Test 1: Integer arithmetic");
    i := 10;
    j := 5;
    writeln("i = ", i);
    writeln("j = ", j);

    k := 1 + 1 + 1;
    writeln("1 + 1 + 1 = ", k);

    k := i + j;
    writeln("i + j = ", k);

    k := i - j;
    writeln("i - j = ", k);

    k := i * j;
    writeln("i * j = ", k);

    k := i div j;
    writeln("i div j = ", k);

    k := i mod j;
    writeln("i mod j = ", k);

    // Test 2: Real arithmetic
    writeln("\nTest 2: Real arithmetic");

    r := 10.5;
    s := 3.2;

    write("r = ");
    writeln(r);

    write("s = ");
    writeln(s);

    write("r + s = ");
    t :=  r + s;
    writeln(t);

    write("r - s = ");
    t :=  r - s;
    writeln(t);

    write("r * s = ");
    t :=  r * s;
    writeln(t);

    write("r / s = ");
    t :=  r / s;
    writeln(t);

    // Test 3: Mixed arithmetic (integer to real)
    writeln("\nTest 3: Mixed arithmetic");
    s := 1.5;
    i := 5;

    write("s = ");
    writeln(s);

    write("i = ");
    writeln(i);

    r := i + s;
    write("i + s = ");
    writeln(r);

    r := i * s;
    write("i * s = ");
    writeln(r);

    // Test 4: Integer comparisons
    writeln("\nTest 4: Integer comparisons");

    i := 10;
    j := 5;

    write("i = ", i);
    writeln(i);
    write("j = ");
    writeln(j);

    write("i < j = ");
    k :=  i < j;
    writeln(k);

    write("i <= j = ");
    k :=  i <= j;
    writeln(k);

    write("i > j = ");
    k :=  i > j;
    writeln(k);

    write("i >= j = ");
    k :=  i >= j;
    writeln(k);

    write("i = j = ");
    k :=  i = j;
    writeln(k);

    write("i <> j = ");
    k :=  i <> j;
    writeln(k);

    // Test 5: Real comparisons
    writeln("\nTest 5: Real comparisons");
    r := 10.5;
    s := 10.5;

    write("r = ");
    writeln(r);
    write("s = ");
    writeln(s);

    write("r < s = ");
    t :=  r < s;
    writeln(t);

    write("r <= s = ");
    t :=  r <= s;
    writeln(t);

    write("r > s = ");
    t :=  r > s;
    writeln(t);

    write("r >= s = ");
    t :=  r >= s;
    writeln(t);

    write("r = s = ");
    t :=  r = s;
    writeln(t);

    write("r <> s = ");
    t :=  r <> s;
    writeln(t);

    // Test 6: Control flow
    writeln("\nTest 6: Control flow");
    i := 5;
    write("i = ");
    writeln(i);

    if i > 3 then
        writeln("i is greater than 3");
    else
        writeln("i is not greater than 3");

    if i < 8 then
        writeln("i is less than 8");
    else
        writeln("i is not less than 8");

    // Test 7: Complex expressions
    writeln("\nTest 7: Complex expressions");
    i := 10;
    j := 4;

    write("i = ");
    writeln(i);
    write("j = ");
    writeln(j);

    result := (i + j) * 2;
    write("(i + j) * 2 = ");
    writeln(result);

    result := i div j + 1;
    write("i div j + 1 = ");
    writeln(result);

    // Test 8: Type conversions in expressions
    writeln("\nTest 8: Type conversions");
    i := 7;
    r := 3.5;

    write("i = ");
    writeln(i);
    write("r = ");
    writeln(r);

    write("i + r = ");
    t := i + r;
    writeln(t);

    write("i * r = ");
    t := i * r;
    writeln(t);

    // Test 9: Edge cases
    writeln("\nTest 9: Edge cases");
    i := 0;
    j := 1;

    write("i = ");
    writeln(i);
    write("j = ");
    writeln(j);

    write("not i = ");
    k := not i;
    writeln(k);

    write("not j = ");
    k := not j;
    writeln(k);

    write("i and j = ");
    k := i and j;
    writeln(k);

    write("i or j = ");
    k := i or j;
    writeln(k);

    writeln("=== EXECUTION TEST END ===");
end.
