program execution_test;
var
    i, j, k: integer;
    r, s: real;
    str1, str2: string;
    result: integer;
begin
    writeln('=== EXECUTION TEST START ===');
    
    // Test 1: Basic integer arithmetic
    writeln('Test 1: Integer arithmetic');
    i := 10;
    j := 5;
    k := i + j;
    writeln('i = ', i);
    writeln('j = ', j);
    writeln('i + j = ', k);
    
    k := i - j;
    writeln('i - j = ', k);
    
    k := i * j;
    writeln('i * j = ', k);
    
    k := i div j;
    writeln('i div j = ', k);
    
    k := i mod j;
    writeln('i mod j = ', k);
    
    // Test 2: Real arithmetic
    writeln;
    writeln('Test 2: Real arithmetic');
    r := 10.5;
    s := 3.2;
    writeln('r = ', r);
    writeln('s = ', s);
    writeln('r + s = ', r + s);
    writeln('r - s = ', r - s);
    writeln('r * s = ', r * s);
    writeln('r / s = ', r / s);
    
    // Test 3: Mixed arithmetic (integer to real)
    writeln;
    writeln('Test 3: Mixed arithmetic');
    r := i + s;
    writeln('i + s = ', r);
    r := i * s;
    writeln('i * s = ', r);
    
    // Test 4: String operations
    writeln;
    writeln('Test 4: String operations');
    str1 := 'Hello';
    str2 := ' World';
    writeln('str1 = ', str1);
    writeln('str2 = ', str2);
    writeln('str1 + str2 = ', str1 + str2);
    
    // Test 5: Integer comparisons
    writeln;
    writeln('Test 5: Integer comparisons');
    i := 10;
    j := 5;
    writeln('i = ', i, ', j = ', j);
    writeln('i < j = ', i < j);
    writeln('i <= j = ', i <= j);
    writeln('i > j = ', i > j);
    writeln('i >= j = ', i >= j);
    writeln('i = j = ', i = j);
    writeln('i <> j = ', i <> j);
    
    // Test 6: Real comparisons
    writeln;
    writeln('Test 6: Real comparisons');
    r := 10.5;
    s := 10.5;
    writeln('r = ', r, ', s = ', s);
    writeln('r < s = ', r < s);
    writeln('r <= s = ', r <= s);
    writeln('r > s = ', r > s);
    writeln('r >= s = ', r >= s);
    writeln('r = s = ', r = s);
    writeln('r <> s = ', r <> s);
    
    // Test 7: String comparisons
    writeln;
    writeln('Test 7: String comparisons');
    str1 := 'apple';
    str2 := 'banana';
    writeln('str1 = ', str1, ', str2 = ', str2);
    writeln('str1 < str2 = ', str1 < str2);
    writeln('str1 <= str2 = ', str1 <= str2);
    writeln('str1 > str2 = ', str1 > str2);
    writeln('str1 >= str2 = ', str1 >= str2);
    writeln('str1 = str2 = ', str1 = str2);
    writeln('str1 <> str2 = ', str1 <> str2);
    
    // Test 8: Logical operations
    writeln;
    writeln('Test 8: Logical operations');
    i := 1;  // true
    j := 0;  // false
    writeln('i = ', i, ' (true), j = ', j, ' (false)');
    writeln('not i = ', not i);
    writeln('not j = ', not j);
    writeln('i and j = ', i and j);
    writeln('i or j = ', i or j);
    writeln('i and i = ', i and i);
    writeln('j or j = ', j or j);
    
    // Test 9: Control flow
    writeln;
    writeln('Test 9: Control flow');
    i := 5;
    if i > 3 then
        writeln('i is greater than 3')
    else
        writeln('i is not greater than 3');
    
    if i < 3 then
        writeln('i is less than 3')
    else
        writeln('i is not less than 3');
    
    // Test 10: Complex expressions
    writeln;
    writeln('Test 10: Complex expressions');
    i := 10;
    j := 5;
    r := 2.5;
    result := (i + j) * 2;
    writeln('(i + j) * 2 = ', result);
    
    result := i div j + 1;
    writeln('i div j + 1 = ', result);
    
    // Test 11: Type conversions in expressions
    writeln;
    writeln('Test 11: Type conversions');
    i := 7;
    r := 3.5;
    writeln('i = ', i, ', r = ', r);
    writeln('i + r = ', i + r);
    writeln('i * r = ', i * r);
    
    // Test 12: Edge cases
    writeln;
    writeln('Test 12: Edge cases');
    i := 0;
    j := 1;
    writeln('i = ', i, ', j = ', j);
    writeln('not i = ', not i);
    writeln('not j = ', not j);
    writeln('i and j = ', i and j);
    writeln('i or j = ', i or j);
    
    writeln('=== EXECUTION TEST END ===');
end. 