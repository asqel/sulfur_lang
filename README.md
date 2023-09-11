# Sulfur programming language documentation

## Introduction

Sulfur is a programming language written in C that supports Windows, Linux, and partially macOS. This documentation provides an overview of the language's features, syntax, and built-in functions.

## Getting started
you need to have gcc installed in order To compile and run the interpreter:
- On Windows : `make win_all`
- On Linux   : `make linux_all`
- On macOS   : It's not fully supported; consider using `make linux_all`
- For profanOS : `make profan` and follow the instructions

it will generate a file called sulfur and run it without argument
## Built-in types

- `ount`     : Signed integer (e.g., `7` or `0x7` for hex notation)
- `floap`    : Signed floating-point number (e.g., `3.14`)
- `boolean`  : Boolean values (`1b` for True, `0b` for False)
- `string`   : String values (`"hi"` or `'hello'`)
- `complexe` : Complex numbers with imaginary unit `$i` (e.g., `1 + 4 * $i`)
- `list`     : Untyped lists (e.g., `list(3, 4, "salut", 1b, list(0, -1))`)
- `nil` : Any variable can be set to `nil` (e.g., `a = nil;`)

## Instructions / statements syntax 

### Setting / changing the value of a variable
`Var_name` = *Expression*; \

#### example :
```
pi = 3.14;
```

### conditons
if ( *Expression* ) { //first condition \
    &emsp;**Instructions**

} elif ( *Expression* ) { // secondary condition \
    &emsp;**Instructions**

} elif ( *Expression* ) { // third condition \
    &emsp;**Instructions**

} else { \
    &emsp;**Instructions**

}

#### example :
```
if (pi > 4) {
    print("pi is greater than 4");
}

elif (pi == "hi") {
    print("pi is a string");
}

else {
    print("pi is pi");
}
```

### for loop
for (`name_of_var` from **Expression** to **Expression**) {
    Instructions
}

#### example :
```
for (i from 0 to 10) {
    println(i);
}
```
#### technical explanations
    if start_Value < end_Value then 
        for loop will execute the instruction in it while the var `name_of_var` < end_Value
        by first setting the var with name `name_of_var` to start_Value
        and after each iteration the value of `name_of_var` will be increased by 1

    if start_Value > end_Value then 
        for loop will execute the instruction in it while the var `name_of_var` > end_Value
        by first setting the var with name `name_of_var` to start_Value
        and after each iteration the value of `name_of_var` will be decreased by 1

    if start_Value == end_Value then
        nothing will be executed and the var `name_of_var` will be set to start_Value

### while loop
while(**Expression**){ \ // while condition
    instructions \
}
#### example :
```
i = 0;
while (i < 10) {
    println(i);
    i += 1;
}
```

### goto / sections 
goto section_name // go to that section

section_name:: // section declaration

#### example :
```
goto a
println("this will not be printed");
a::
```
#### techanical explanation :
the goto statement fisrt search the section downward
then if it does not find it will search upward

multiple sections can have the same name

a section can have the same name as a variable and that will cause no issues

### anonymous function 

{**Instructions**}

#### example :
```
pi = 3;
println({
    if (pi > 3){
        return "greater";
    } else {
        return "lower or equal";
    }
});
```
#### techanical explanation :
the anonymous function scope is the same as before entering the anonymous function

## Keywords
- `if`      : Used for conditional statements.
- `else`    : Used in conditional statements.
- `elif`    : Used in conditional statements.
- `while`   : Used for loop constructs.
- `for`     : Used for loop constructs.
- `class`   : Used to declare a class (not impleted yet).
- `return`  : Used to return a value from a function.
- `Example` : return Value;
- `def`     : Used to declare a function (not implemented yet).
- `from`    : Used in for-loops to declare the starting value.
- `to`      : Used in for-loops to declare the ending value.
- `goto`    : Used to jump to a labeled section.

## Operatos
### Arithmetic operators
- `+` : addition
- `*` : multiplication
- `-` : substraction
- `/` : division
- `^` : power
- `%` : modulo
- `\` : floor division
### Logical operators
- `&` : logical AND
- `|` : logical OR
- `!` : logical NOT
### acces members operators
- `.` : dot used to acces module members or methods
- `:` : colon used to acces list members or string characters
### comparison operators
- `==` : equal
- `!=` : not equal
- `>=` : greater or equal
- `<=` : less or equal
- `>`  : greater
- `<`  : less

## imports
import(`'file name'`,`'alias'`); \
or \
import(`'file name'`);

`'file_name'` is the name of the file tha will be loaded it can also be one of the std libs
`'alias'` will be the name used to access its members

#### example :
```
import("math","mt");
println(mt.pi);
```

## accessing list / string
a list is mutable  \
a string is immutable \
mutable means its members can be changed \
immutable means its members can't be changed

`l : index`
`s : index`

if index is less than 0 the length will be returned

#### example :
```
l = list(3,4);
println("fist element", l:0);
println("length", l : -1);
```
## the interpreter
if it is run without a path to a file the shell will be launched \
available flags :
```
-m, --show-mem      show memory after execution
-p, --show-parse    show parse tree
-l, --show-lexe     show tokens after lexing
-h, --help          show help message and exit
-v, --version       show sulfur version and exit
```

## Makefile

- `win` : Compile for Windows
- `linux` : Compile for Linux
- `profan` : Compile for profanOS
- `run` : Start the interpreter
- `stdlibs_win` : Compile the standard libraries for Windows
- `stdlibs_linux` : Compile the standard libraries for Linux
- `win_all` : Compile for Windows and start the interpreter
- `linux_all` : Compile for Linux and start the interpreter
- `linux_one_file` : Compile for Linux without separate libraries
- `help` : Show Makefile details

Files can also be downloaded from [stable releases](https://github.com/asqel/sulfur_lang/releases) (windows and linux)
and from the automated [repository](https://github.com/elydre/sulfur_lang/releases/tag/latest) (linux, one-file and profanOS)


## Built-in Functions
- `print(...)` : Print values to stdout, separated by spaces.
- `println(...)` : Print values to stdout with a newline character.
- `input()` : Read input from stdin until a newline and return it as a string.
- `time()` : Return the Unix time in milliseconds as an ount.
- `import(File, As)` : Import a file as As.
- `sleep(x)` : Pause execution for x milliseconds.
- `bool(x)` : Return the boolean value of x.
- `ount(x)` : Return the ount value of x.
- `floap(x)` : Return the floap value of x.
- `list(...)` : Return a list composed of its arguments.
- `type(x)` : Return the type of x as a string.
- `__set_precision(x)` : Set the precision of floap when printing.
- `__get_precision()` : Return the precision of floap when printing.
- `var_exists(x)` : Check if the variable x exists.
- `chr(...)` : Return a string composed of the values in the arguments.
- `asc_val(x)` : Return the ASCII value of the first character in x.
- `method(x)` : Return the methods of x as a string.
- `pop(x)` : Remove the last element of x (x must be a list).
- `rand()` : Return a random floap between 0 and 1.
- `__print_memory__()` : Print the memory.
- `__set_flush__(x)` : Control whether the interpreter should fflush(stdout) when printing.

## Built-in Variables

- `__path__` : The path of the executed .su file.
- `__interpreter_path__` : The path of the interpreter.
- `__dir_path__` : The directory path of the interpreter.
- `__os__` : The name of the OS, or "UNKNOWN" if not known.
- `__version__` : The version of the interpreter.
- `__base_precision__` : The base precision for printing floap.
- `nil` : A constant used as null.
- `_` : A variable guaranteed to exist, set to nil by default.
- `__` : A variable guaranteed to exist, set to nil by default.
- `___` : A variable guaranteed to exist, set to nil by default.
