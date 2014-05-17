PL0\_Interpreter
===============

A compiler and p-code virtual machine for the educational programming language [PL0](http://en.wikipedia.org/wiki/PL/0).

The p-code vm is based on [this](http://en.wikipedia.org/wiki/P-code_machine#Example_machine), with slight modifications.

Support for basic console I/O is added:

1. to read input from console and store it in variable `x`, use:
```pascal
    ?x
```

2. to write the value of variable `x` to console, use:
```pascal
    !x
```

The program is *not* adequately tested, though it works correctly on the test cases in this repo.

Licence
===============
WTFPL
