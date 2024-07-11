# Sulfur Bytecode file format 

the file is composed of 7 / 8 parts
- magic number
- address of the first instruction
- header
- requested variable left
- requested variable right
- string constants
- code
- extra data (optional)

magic number

	`.SU.` without the \0 at the end 
	stands on 4 bytes

address of the first instruction

	it is the same as the address of the code
	the adress 0 is considered to be the start of the file
	stands on 8 bytes little endian

## header

the heaer is composed of parts
- file type
- date time
- the address of the requested variables left
- the address of the requested variables right
- the address of the string constants
- the adress of the extra data

file type

	describe the type of the file for now only executable exists
	0 : executable
	stands on 1 byte

date time

	a null terminated string representing the time when it was compiled
	it is of the format JJ/MM/YYYY|hh/mm/ss
	if the time is not available use "FAKE_TIME"

the address of the requested variables left

	it is the addres of the section containing the requested variables left
	the adress 0 is considered to be the start of the file
	stands on 8 bytes little endian

the address of the requested variables right

	it is the addres of the section containing the requested variables right
	the adress 0 is considered to be the start of the file
	stands on 8 bytes little endian

the address of the string constants

	it is the addres of the section containing the string constants
	the adress 0 is considered to be the start of the file
	stands on 8 bytes little endian

the address of the extra data

	it is the addres of the section containing the extra data
	if it is 0 then there is no extra data
	the adress 0 is considered to be the start of the file
	stands on 8 bytes little endian

## requested variable left
it contains
- its len aka the number of variable left
- the variables left

its len stands on 8 bytes
the variables are stored as null terminated strings \
they represent the variable of the program that was  \
on the left side of the dot operator \
or not on a dot operator

example

	a.b = c

	requested variable left len = 2
	- a
	- c

## requested variable right
it contains
- its len aka the number of variable right
- the variables right

its len stands on 8 bytes \
the variables are stored as null terminated strings \
they represent the variable of the program that was  \
on the right side of the dot operator

example

	a.b = c

	requested variable left len = 1
	- b

## the string constants
it contains
- its len aka the number of string constants
- the strings

its len stands on 8 bytes \
the strings are null terminated they are the strings \
that the programm use during the execution


## code
contains the actual bytecode code
and is ended with a `push_nil` `ret` even if the code ends with its own ret \
the code is considered to be inside an anonymous function \
for now the return value of the code has no effect 

## the extra data
it contains
- its len aka the number of keys
- the pairs key/values

its len stands on 8 bytes
the pairs are composoed of 3 parts
- the key a unique null terminated string
- the length of the data in bytes (stands on 8 bytes)
- the data

it is a key value format where keys are null terminated \
values can be anything \
strings have to be different one another 
see [extra data common formats](extra_data/common_format.md)

