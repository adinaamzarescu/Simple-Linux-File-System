# Simple-Linux-File-System
## Copyright 2022 Adina Amzarescu 321CA & David Nedelcu 314CA

This is the third University Project for the Data Structures course.
It is a simulation of a simple linux file system.

Keywords: Tree data structure, list data structure, memory management.

___________________________________________________________________________________________

It has 9 commands:

```
* touch 
```
  This creates a new file in the current directory.
  You can find this function in _tree.c_.
  Before creating a file, the function check_existance()
  will check if there already is a file/directory with the same name.
  After the file is created, it will be added at the end of the list of files by
  changing the **_next_** parameter of the previous file.
  
  * Extra functions :
  
    * make_file_node()
  
      You can find it in _helpers.c_.
      In order to make a new file, the program will allocate memory for it
      and then it will initialize its parameters.
      The type parameter will be 0.
    
    * create_new_list()
    
      You can find it in _helpers.c_.
      If this is the first file in the current directory, the content of
      that directory will be initialised to a new list in which the created
      file will be the head. As more files are created, the content will be
      changed.

___________________________________________________________________________________________

```   
* mkdir
```

  You can find this function in _tree.c_.
  This creates a new directory in the current folder.
  Before creating a file, the function check_existance()
  will check if there already is a file/directory with the same name.
  After the file is created, it will be added at the end of the list of files by
  changing the **_next_** parameter of the previous file.
  
  * Extra functions :
  
      * make_folder_node()
  
        You can find it in _helpers.c_.
        In order to make a new file, the program will allocate memory for it
        and then it will initialize its parameters.
        The type parameter will be 1.
        
      * create_new_list()
    
        You can find it in _helpers.c_.
        If this is the first file in the current directory, the content of
        that directory will be initialised to a new list in which the created
        file will be the head. As more files are created, the content will be
        changed.


___________________________________________________________________________________________

```  
* ls
```

  You can find this function in _tree.c_.
  This function will list all the directories and then all the files. If the arg variable
  exists, it will list its content instead. The type variable checks if the given argument
  is a file or a directory. For each one of those cases there is a different function.

  
  * Extra functions: ls_file() ls_folder() ls_all()
  
    You can find those functions in _helpers.c_.
    ls_all is used to print the name of each node in the list.
    ls_file is used to print the name of the file followed by its content (file : content)
    ls_folder uses ls_all to print the content of the given directory.
    
___________________________________________________________________________________________

```
* rm
```

   You can find this function in _tree.c_.
   This removes a file. If no file can be found, it will print
   an error message, depending on the type of error.
   After removing the file from the list, it will free
   the memory that has been allocated for it.
   
___________________________________________________________________________________________

```
* rmdir
```

   You can find this function in _tree.c_.
   This removes a directory. If no directory can be found, it will print
   an error message, depending on the type of error.
   This function will work only if the given directory is empty.
   After removing the directory from the list, it will free
   the memory that has been allocated for it.
   
___________________________________________________________________________________________

```
* rmrec
```

   You can find this function in _tree.c_.
   This removes a directory or file. If the given name can't be found it will print
   an error message.
   If the given argument is a directory, it will remove its content recursively.
   The memory will be freed aftwerwards.
   
___________________________________________________________________________________________

```
* cd
```

   You can find this function in _tree.c_.
   This changes the current directory in the one with the given name. 
   If the path is ".." then the directory will become the parent of the current one. 
   The initial skel has been modified by adding the tree_cmd parameter. This is used in
   order to print the error messages only if the command is cd. We made those changes because
   for the tree function we will use the cd function and the error messages for that are different.
   In main cd the tree_cmd will be 0 and when it is called in the tree function tree_cmd will be 1.
   
   Extra function: find_folder()
   
   You can find it in _helpers.c_. 
   This function returns the directory if it exists and if it doesn't, it will return NULL.
   
___________________________________________________________________________________________

```
* tree
```

   You can find this function in _tree.c_.
   This function will show the content of the current directory in an
   arborescent form.
   If no argument is given, it will be applied to the current directory.
   If a path is given as an argument, the cd function will be used with
   the tree_cmd being 1. (see README.md cd function, line 133).
   
   After the function will print the content, it will also print the
   number of files and directories found.
   
   Extra function: tree_print()
   
   You can find it in _helpers.c_. 
   This is a recursive function so at first level will be 0. Then level
   will be incremented by 1 for every directory found inside. The spacing
   consists of 4 spaces. The number1 and number2 variables are used to count
   the number of files/directories.
   
___________________________________________________________________________________________

```
* pwd
```

   This function shows the path of the current directory, staring with root. The last
   directory will be printed in the end because it shouldn't end with the "/" character.
    
   * Extra function : print_path()
   
       You can find it in _helpers.c_. 
       Since it is a recursive function, print_path will print
       the name of the parent followed by "/". Every path will start with "root/" because
       this is the first directory of the system.
       
___________________________________________________________________________________________

## **Other functions**

```
* create_file_tree
```

  You can find it in _tree.c_. 
  This function creates the file tree that will be used for the entire project. 
  The name of the root will be "root" and its type will be automatically 1 (folder type).

___________________________________________________________________________________________

```
* free_tree
```

  This function frees the memory allocated for the tree. The free_folder function will be
  called with the root as the folder to be freed.
  
  Extra functions: free_file() free_folder() free_list()
  
  You can find those functions in _free_mem.c_. 
  The points of the first 2 functions are similar. If a file/directory with the given name
  is found then the memory will be freed. If it is the first file/directory in the current
  node's content then the content (which is a list) will be freed as well using the free_list
  function. If it is a folder then its content will be freed as well, recursively.
  
  Setting unused pointers to NULL is a defensive style, protecting against dangling pointer bugs.
  
___________________________________________________________________________________________
  
  ## Important mentions
  
  * The Makefile has been changed
  * The main function has been changed a little. In order to eliminate the warnings we created
    the my_strdup() function. You will find it in _main.c_.
  * The names of the functions and variables were changed in order to follow the snake_case
    naming convention.
    
___________________________________________________________________________________________

## Feedback :

A very interesting project with some difficulties. Valgrind and gdb were our friends especially
because the strings sent with strdup had to be freed and we weren't able to notice that right away.
Since there are a lot of deadlines going on we weren't able to implement the last 2 functions, but
the project wasn't as complex as the second one. We are not scared of the "Segmentation fault" message
anymore so thank you for this ;)

___________________________________________________________________________________________

## Resources:

1. [OCW](https://ocw.cs.pub.ro/courses/sd-ca)
2. [STACKOVERFLOW](https://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free)
3. [WIKIPEDIA](https://en.wikipedia.org/wiki/Tree_(data_structure))
  
