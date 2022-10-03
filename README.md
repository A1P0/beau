<div align="center">
        <img src="head.png" width="200">
        <h1>Beau</h1>
</div>

###### Match C's capability in style

Beau is a language I am defining and implementing for learning purposes,
but also an interest in seeing some of my ideas applied. Beau will match C's
capability in style. I want a utilitarian systems language that is pleasing to
the eye.

Some of the ideas I've brought to the language are originally inspired by the
original B language but with my own twist. 

## A Taste

#### No Preprocessor
One of the main goals of Beau is to rid us of the preprocessor of old. It's just
gross. It adds a whole other step in the compilation process, and for what?
Uglier, messier, and harder to understand code and structure. So the first step
in meeting this goal is compile time includes! As development continues and the
compiler becomes operable more compile time statements will be implemented to
fill in classic preprocessor use cases.

#### Data types
In many languages it has always bothered me that it's not immediately obvious
what a data type actually is, and it is often implementation dependent. Beau
will have built-in data types that are named very clearly. `byte`, `word`,
`dword`, `qword`. We know exactly the size of those values immediately. I chose
this over the more common (lately) `i8` - `i64` simply for aesthetic purposes.
Numbers in identifiers clutter appearance, and its not like we save that much
space by saying `i8` over `byte` anyway, these keywords are only being used for
definitions.

#### Pointer syntax
Beau is meant to be clean and readable, so a change I've implemented for
readability and style is changing pointer syntax. For example below we have the
line: `byte @ string: "Hello, World!";`. This defines a pointer "string" that
points to a byte (which happens to be the location of our string). The intent
is for it to be read like "I am referring to a byte at the value of x". I also
think the `@` operator is just cleaner looking and prominent in a way you want,
the `*` makes things messy in my opinion. I plan for `%` to be the new address-
of operator (replacing `&`).

#### Initialization syntax
Beau will make great use of the colon (`:`) for initializing values. It always
bothered me in other languages that the assignment operator (`=`) was also
utilized for initialization, to me these are two different things and should
be distinct. Initializers are not expressions. This is utilized for variable
initialization and function definition.

#### Default argument values
Functions can have default argument values defined as seen below in the "add"
function. Can be invoked as `add();` or `add(,2)`.

#### Keywords
Where appropriate some keywords will be shortened for more concise and sleek
code. At the moment the main example being `ret` for `return`.

#### Example

```Java
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
```

## Status

Mostly complete front end, working on a simple back end to get the compiler up
and running. At first I considered LLVM, but the overhead of this for such a
simple language is ridiculous so after some research I have decided to utilize
the [QBE Compiler Backend](https://c9x.me/compile/).

## Grammar

Check the EBNF grammar in `GRAMMAR`.

## License

BSD 3-Clause License, see `LICENSE`.