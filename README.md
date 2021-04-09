### Brainfuck interpreter(s)

The source files contain interpreters to two implementations
of the esoteric programming language called brainfuck. One uses
an abstract syntax tree (AST). The other simply iterates through the
source file, looping back to or past matching brackets if needed.
The AST implementation is seemingly faster, but requires parsing before
executing the program. I can't guarantee 100% accuracy, but the programs
I tested seem to work well enough.