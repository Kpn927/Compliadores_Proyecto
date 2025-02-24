program Hola;
var 
    x,y,z,po,culo,webo:integer;
    z:float;
begin

// Hola, esto no deberia aparecer
/* Hola, esto tampoco deberia aparecer */
/* Esto si deberia aparecer */


for i := 0; to 150; do
    begin
        x := x + 1;
        writeln(x);
    end;

for i := 0; downto 150; do
    begin
        x := x + 1;
        writeln(x);
    end;

for i := 0; to 150; do;
    begin
        x := x + 1;
        writeln(x);
    end;