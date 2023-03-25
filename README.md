# sulfur
a programming language in c


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
        ex : ount a=nil;


## Syntax instructions
### Setting a var:
    Type Name = Value ;
        ex: floap pi=3.141592;

### Changing value of var:
    Name = Value ;
        ex: pi=3.0 ;

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

    section_name:
        ex: goto sec
            print("this will not be printed");
            sec:


### function definition:
    def func_name(type1 arg1 , type2 arg2,...): return_type1 | return_type2 :{
        instructions
    }
    

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
    swap : used to swap to value
        ex: ount a=3;
            ount b=4;
            swap a b;//now b=3 and a =4
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
    

## built-in functions:
    print(x,...) : print to stdout the value of each args separated with space
    println(x,...) : call print with the same arguments and print a new line 
    input() : read input from stdin until there is a new line return the input as a string
    time() : return the unix time in miliseconds as an ount

    bool(x) : return the boolean value of x
    ount(x) : return the ount value of x  
    list(x,...) : return a list composed of every argument passed if none the list is empty

## built-in vars:
    __path__ : the path of the executed .su file
    nil : a constant used like null