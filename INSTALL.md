# Sulfur installation guide

### for now only linux and windows are supported (for macos you can try to use the linux installation)

## for pre built (linux and windows)
  - download from the releases sulfur for your os and place everything that is inside the compressed file into
  - the folder $HOME/sulfur/
  - now you should have the sulfur executable at $HOME/sulfur/sulfur
  - we put it here so if wwe want to use the vscode extension for sulfur it will find it here
  - now if you want to run a program jutse do in a terminal $HOME/sulfur/sulfur path-to-your-file

## for non pre built (linux and windows (here macOS try using linux)
  - you will need a few things :
    - the gcc compiler
    - the ability to run a makefile
  - first download the source code of sulfur on github
  - then open a terminal in the folder of the source code (there should be a main.c and a makefile)
  - then for linux do make linux_all and for windows do make win_all
  - now in the build folder you should have a sulfur executable (.exe on windows probably) and a libs folder
  -take these two and place them in $HOME/sulfur/
  - we place them ttheree so if you use the vscode extension it will find them
  - now you can use sulfur by doing $HOME/sulfur/sulfur path-to-your-file
  
