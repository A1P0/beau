/*
 * example.b
 */
include stdio;

byte @ string: "Hello, World!";

byte
add(byte x: 0, byte y: 0):
{
        ret x + y;
}

byte
main():
{
        writeln(string);
        ret add(40, 2);
}