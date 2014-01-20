/**********************************************************
 * Datoteka: defs.h
 * Autor: Hrvoje Babaja
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

#define MAX_LENGHT 70
#define MAXLINE 4096

/* zamjena s ciljem lakseg uocavanja u kodu, a da se ne mora koraju koristiti stringovi
 * za ove nazive
 */
#define STDIN 1
#define STDOUT 2
#define BACKGROUND 3
#define FOREGROUND 4

/* globalne varijable 
 */
static char* currentDir;
static char input = '\0';
static char tempLine[MAX_LENGHT];	// ovdje se sprema korisnikov unos 
static int tempChars = 0;	// broj unesenih znakova
static char *arguments[10];	// argumenti naredbe korisnika 
static int nArguments = 0;	// broj argumenata iz naredbe 


/* deklaracije funkcija 
 */
void initialization();

void displayShell();

void getUserInput();

void initializeCommand();

void makeCommand();

int processUserCommand();

void changeDir();

void killProcess(int processID);

void executeCommand(char *cmd[], char *filePointer, int std , int mode);

void pipeline(int f);

void displayHelp();

void childSignalHandler(int p);




