program code

var 
    x:integer;
function sumar(i: integer; j: integer): integer;
begin
    sumar := i + j;
end;

begin
    x := 0;
    if (x >= 3) then
    begin
        x := x+1;
    end
    else 
    begin
        x = x + 2;
    end;
    
    for i := 0;  to 150; do
    begin
        x := x + 1;
        writeln(x);
    end;
end.