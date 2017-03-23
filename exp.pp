var i : integer,
var j : integer,
var b : boolean,
var t : array of array of integer
defpro inf(x : integer, y : integer)
{
if(x <= y) then
b := true
else
b := false
}
defun test(x : integer) : boolean
var k : integer
{
k := 0;
test := x = k
}
i := 5;
j := 0;
inf(i, j);
while(not test(i) and not b) do {i := i - 1; j := 0};
t := new array of array of integer [5];
t[0] := new array of integer [2];
t[0][j] := j