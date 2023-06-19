# sulfur
a programming language in c

to compile and run on windows:
    make win_all
to compile and run on linux:
    make linux_all
to compile and run on macos:
    it isn't entirely supported mayber use make linux_all

## built-in types
    ount : signed integer   
        ex : 7
    floap : signed floating point number    
        ex : 3.14
    boolean : can store only 2 value 1b(True) or 0b(False)  
        ex : 1b or 0b
    string : a string   
        ex : "hi" or 'hello'
    complexe : a complexe number composed of 2 floap ($i : imaginary unit)
        ex : 1+4*$i
    list : a non typed list 
        ex : list(3,4,"salut",1b,list(0,-1));
    nil_type : any variable type can be set to a nil_type
        ex : a=nil;


## Syntax instructions
### Setting a var:
    Name = Value ;
        ex:
            pi=3.141592;

### Changing value of var:
    Name = Value ;
        ex: 
            pi=3.0 ;

    changing value of var return its old value
        ex:
            a=0;
            b=1;
            a = (b = a); //now a=1 and b=0 


### Conditions:
    if ( condition ){
        instructions
    }
    elif ( second condition ){
        instructions
    }
    elif ( third condition ){
        instructions
    }      
    else{
        instructions
    }
        ex: if(pi>4){
                print("pi is greater than 4");
            }
            elif(pi=="hi"){
                print("pi is a string");
            }
            else{
                print("pi is pi");
            }

### for loop:
    if start_Value < end_Value then 
        for loop will execute the instruction in it while the var [name_of_var] <end_Value
        by first setting the var with name [name_of_var] to start_Value
        and after each iteration the value of [name_of_var] will be increased by 1

    if start_Value > end_Value then 
        for loop will execute the instruction in it while the var [name_of_var] >end_Value
        by first setting the var with name [name_of_var] to start_Value
        and after each iteration the value of [name_of_var] will be decreased by 1

    if start_Value == end_Value then
        nothing will be executed and the var [name_of_var] will be set to start_Value
    -----------------------------------------------
    for(name_of_var from start_Value to end_Value){
        Instructions
    }
        ex: for(i from 0 to 10){
            print(i);
        }

### while loop:
    while loop will execute the instructions inside it while the condition is true
    -----------------------------------------------
    while(condition){
        instructions
    }
        ex:while(pi>0){
            pi=pi-0.5;
            println(pi);
        }

### sections and goto:
    goto first search downward the section then it will search upward 
    -----------------------------------------------
    goto section_name

    section_name::
        ex: goto sec
            print("this will not be printed");
            sec::


### function definition:
    def func_name(type1 arg1 , type2 arg2,...): return_type1 | return_type2 :{
        instructions
    }
    
### anonymous function:
    {instructions}

    it can be used in expresion
    the instruction has to return a value else it will be considered nil
    it uses the same scope as before entering it

    ex:
        k=0;
        print(3 + {
            for(i from 0 to 10){
                k=k+i;
            }
            return k;
        });
        //here 48 will be printed


## keywords:
    if : used for conditions
    else : used for conditions
    elif : used for contions
    while : used for loop
    for : used for loop
    class : used to declare a class
    return : used to return a value in function 
        ex: return Value;
    def : used to declare a function
    from : used in for-loop to declare the starting value
    to : used in for-loop to declare the ending value
    goto : used to go to a section

## operators:
    + : addition
    * : multiplication
    - : substraction
    / : division
    ^ : power
    % : modulo
    \ : floor division
    | : logical or
    & : logical and 
    ! : logical not
    . : dot operator (used to acces member of class or library)
    < : less than
    > : greater than
    <= : less than or equal
    >= : greater than or equal
    == : equal 
    != : not equal
    

## importation:
    you can use the import function to import a file
    it takes as arguments the path to the file
    if the path is not absolute it will search in the interpreter libs
    you can acces Object from file with dot operator
    like this filename.func1()

    you can change the name of the file in the program
    if you import(filename,newname) you'll have to acces Objects like this
    newnamme.func1()

    if the newname is "" then you acces Object like if they were definined in the 
    same file so 
    import(filename,"")
    func1()


## acces modules
    to acces an element of a module use '.'
    ex:
        import("math");
        println(math.pi); // "3.14"

## acces lists
    to acces an element of a list use ':'
    ex:
        l = list(3, 4, 7);
        println(l:1); // "4"
    ex:
        a = 0;
        l = list(0, 1, 0b);
        l:a = 7 // l = 7, 1, 0b
        l:(a + 1) = -30 //l = 7, -30, 0b

## built-in functions:
    print(x,...) : print to stdout the value of each args separated with space
    println(x,...) : call print with the same arguments and print a new line 
    input() : read input from stdin until there is a new line return the input as a string
    time() : return the unix time in miliseconds as an ount
    import(File,As) : import the file File as As (if As == "" Objects from file will be in stored in memory)
    import(File) : import the file File as its name

    sleep(x) : poses the execution for x miliseconds
    bool(x) : return the boolean value of x
    ount(x) : return the ount value of x  
    list(x,...) : return a list composed of every argument passed if none the list is empty

## built-in vars:
    __path__ : the path of the executed .su file
    nil : a constant used like null


## the interpreter:
if you call it with the argument -m the memory will be shown at the end of execution