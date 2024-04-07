# Sulfur programming language documentation

# Introduction

Sulfur is a programming language written in C that supports Windows, Linux, and partially macOS. This documentation provides an overview of the language's features, syntax, and built-in functions.

you can use the [sulfur_code](https://marketplace.visualstudio.com/items?itemName=asqel.sulfur-color) vscode extension to have coloration and some autocompletion

for installation see [the installation guide](INSTALL.md)

## Getting started
you need to have gcc installed in order To compile and run the interpreter:
- On Windows : `make win_all`
- On Linux   : `make linux_all`
- On macOS   : It's not fully supported; consider using `make linux_all`
- For profanOS : `make profan` and follow the instructions

it will generate a file called sulfur and run it without argument

### Identifier
Sequence of character that can contain any alphanumerical character and cannot start with a number,
it can also contain some special character such as `_` but not if the character is an opeartor
    ```
    ```

## Built-in types

- `ount`     : Signed integer (e.g., `7` or `0x7` for hex notation)
- `floap`    : Signed floating-point number (e.g., `3.14`)
- `boolean`  : Boolean values (`1b` for True, `0b` for False)
- `string`   : String values (`"hi"` or `'hello'`)
- `complexe` : Complex numbers with imaginary unit `$i` (e.g., `1 + 4 * $i`)
- `list`     : Untyped lists (e.g., `list(3, 4, "salut", 1b, list(0, -1))`)
- `nil` : Any variable can be set to `nil` (e.g., `a = nil;`)

# Instructions / statements syntax 


## Setting / changing the value of a variable
a variable name must be a valide [identifier](#identifier)

```js
var_name = *Expression*;
```

### example :
```js
pi = 3.14;
```


## conditons
conditions are used to control the flow of instructions

```js
if ( *Expression* ) { //first condition 
    **Instructions**

} elif ( *Expression* ) { // secondary condition 
    **Instructions**

} elif ( *Expression* ) { // third condition 
    **Instructions**

} else {
    **Instructions**

}
```

### example :
```php
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

## for loop
for loops are used to perform instructions multiple times with an iterator that will take value according to the values of start and end expressions
```js
for (name_of_var from *Expression** to *Expression*) {
    Instructions
}
```

### example :
```js
for (i from 0 to 10) {
    println(i);
}
```


### technical explanations
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

## while loop
while loops are used to performe instructions until a condtion is unmet

```js
while(**Expression**){  // while condition
    instructions 
}
```

## example :
```js
i = 0;
while (i < 10) {
    println(i);
    i += 1;
}
```

## include
> !! this section is in work in progress !!
include "path to file"

include will juste copy and paste the code of the file into the current file after lexing   
you can put '//' in front of the file for a relative path

DONT DO CIRCULAR INCLUDE (when 'a' include 'b' and 'b' include 'a')

### example :
```
include "/my_libs/hello.su"
include "//relative_file.su"
```

## goto / sections 
gotos are used to control the flow of instruction

```C
goto section_name
```

```js
section_name:: // section declaration
```

### example :
```js
goto a
println("this will not be printed");
a::
```
### techanical explanation :
the goto statement fisrt search the section downward
then if it does not find it will search upward

multiple sections can have the same name

a section can have the same name as a variable and that will cause no issues

## anonymous function 
anonymous functions are block of instruction that can return a value and can be used in expressions

```js
{**Instructions**}
```

### example :
```js
pi = 3;
println({
    if (pi > 3){
        return "greater";
    } else {
        return "lower or equal";
    }
});
```
### techanical explanation :
the anonymous function scope is the same as before entering the anonymous function every variable declared within the anonymous function will also be declared in the said scope

# Keywords
- `if`      : Used for conditional statements.
- `else`    : Used in conditional statements.
- `elif`    : Used in conditional statements.
- `while`   : Used for loop constructs.
- `for`     : Used for loop constructs.
- `class`   : Used to declare a class (not impleted yet).
- `return`  : Used to return a value from a function.
- `def`     : Used to declare a function.
- `from`    : Used in for-loops to declare the starting value.
- `to`      : Used in for-loops to declare the ending value.
- `goto`    : Used to jump to a labeled section.
- `proceed` : goto next iteration of the loop.
- `stop`    : break out of a loop earlier.
- `include` : include every token of a file depending on the including mode

## Operatos
### Arithmetic operators
- `+`  : addition
- `*`  : multiplication
- `-`  : substraction
- `/`  : division
- `**` : power
- `%`  : modulo
- `\`  : floor division
### Bitwise operators
- `>>` : right shift bit
- `<<` : left shift bit
- `|`  : bitwise AND
- `&`  : bitwise OR
- `~`  : bitwise NOT
- `^`  : bitwise XOR
### Logical operators
- `&&` : logical AND
- `||` : logical OR
- `!`  : logical NOT
- `^^` : logical XOR
### acces members operators
- `.`  : dot used to acces module members or methods
- `:`  : colon used to acces list members or string characters
### comparison operators
- `==` : equal
- `!=` : not equal
- `>=` : greater or equal
- `<=` : less or equal
- `>`  : greater
- `<`  : less
## other operators
- `$` : unpack an object into arguments in function call

# imports
import function is used to import modules from files
```js
import('file name','alias'); 
```
or 
```js
import('file name'); 
```

`'file name'` is the name of the file tha will be loaded it can also be one of the std libs
`'alias'` will be the name used to access its members

### example :
```js
import("math","mt");
println(mt.pi);
```

# accessing list / string
a list is mutable  \
a string is immutable \
mutable means its members can be changed \
immutable means its members can't be changed

```js
l : index
```
```js
s : index
```

if index is -1 the length will be returned

### example :
```js
l = list(3,4);
println("fist element", l:0);
println("length", l : -1);
```

# defining a function
> work in progress

commas are optionals in function declaration
if last param has $ it will be a packed param
use arg:index to acces elements, it can be emptyu
```js
def func_name(arg1, arg2) {
    instructions
}
```

```js
def func_name(arg1, $arg2) {
    instructions
}
```



# the interpreter
if it is run without a path to a file the shell will be launched \
available flags :
```
-m, --show-mem      show memory after execution
-p, --show-parse    show parse tree
-l, --show-lexe     show tokens after lexing
-h, --help          show help message and exit
-v, --version       show sulfur version and exit
```

# Makefile

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


## [Built-in functions](doc/built-in/functions/intro.md)

## [Built-in variables](doc/built-in/vars/intro.md)

