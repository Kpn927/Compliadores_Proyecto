program Hola;
var 
    x,y,z,y1:integer;
    ELPEPEMAGICO:double;
begin
    readln(x);

     x := 0;
    if (x > 3) then
    begin
        if (y < 5) then
            z := 10;
    end;
    writeln("Hola" +);

for x := 0 to 150 do
    begin
        x := x + 1;
    end;
// Hola, esto no deberia aparecer
(* Hola, esto tampoco deberia aparecer *)
{ Hola, esto tampoco deberia aparecer }
    x := x + 1;
    y1 := 10;
    z := x + 10;
    y := x+y1+(z*3+2);


end.
