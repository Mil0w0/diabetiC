### WINDOWS ###

# HOW TO INSTALL SQL LITE

https://www.tutorialspoint.com/sqlite/sqlite_installation.htm
 * Download :   sqlite-dll-win64-x64-3400000.zip 
                sqlite-tools-win32-x86-3400000.zip
    at https://www.sqlite.org/download.html 

 * Unzip those files in C:\sqlite
 * Add this path in the $PATH environnment variable (Paramètres>Système>A propos> Paramètres avancés> Environnement de variables> PATH> Nouveau>OK)
 * open a shell and run "sqlite3" (-> should display version of sqlite)

# Useful commands in sqlite: 
sqlite >.databases  --display databases
sqlite >.quit       --to quit sqlite

# TO COMPILE WITH THE LIBRARY : 
https://www.sqlite.org/howtocompile.html

   * Download the Sqlite-amalgamation zip
   at https://www.sqlite.org/2022/sqlite-amalgamation-3400100.zip
   * Unzip it in your project.
   * Add the header in your program : #include "sqlite3.h".
   * Run gcc main.c sqlite3.c -o main.exe where main.c is your program to compile.
   * It should be all good :)

########


### LINUX ###


########