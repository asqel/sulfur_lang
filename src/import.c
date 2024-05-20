/*

Keyword:
	import

libs found in ~/sulfur/libs
import(math)	 	| import math as math
import(math, mt) 	| import math as mt

lib found in ./ or ./libs/
import("math", mt)	| alias is need becausse file can have names that are not variable names
	relative or absolute path can be used
	relative path ar relative to the executed file
	if a path starts with // its root will be ~/sulfur/libs
		import("//math", mt) same as import("//")

*/