/**********************************************************
 * Datoteka: hbshell.c
 * Autor: Hrvoje Babaja
 **********************************************************/

#include "defs.h"
#include "functions.h"

/* definicija main funkcije
 */
int main(int argc, char **argv, char **envp)
{
	initialization();
	displayShell();
	
	while(1)
	{
		input = getchar();
		switch (input)
		{
			case '\n':	// u slucaju da korisnik unese 'enter' opet ispisemo radni direktorij
				displayShell();
				break;
			default:
				getUserInput();
				processUserCommand();
				break;
		}
	}
	
	printf("\n");
	return 0;
}


/* ispis uvodne poruke
 * upravljanje raznim signalima
 */
void initialization()
{
	
	signal(SIGINT, SIG_IGN);   
	signal(SIGQUIT,SIG_IGN);
	signal(SIGSTOP,SIG_IGN);	// ignorira signale ctrl+c, ctrl+d, ctrl+z 
	signal(SIGCHLD, &childSignalHandler);
	printf("\x1b[2J\x1b[H");	// brise ekran, sigurnije od system("clear") 
	
	printf("\n\tUNIX shell\n");
	printf("\tAutor: Hrvoje Babaja\n");
	printf("\tPomoc pri koristenju - upisite 'help'\n");
	
	currentDir = (char*) calloc(1024, sizeof(char));	// alokacija memorije za spremanje trenutnog direktorija 
}


/* prikaz trenutnog radnog direktorija
 */
void displayShell()
{
	printf("%s : ",getcwd(currentDir,1024));	// getcwd sprema putanju trenutnog direktorija u currentDir 
}


/* promjena direktorija u slucaju naredbe cd + direktorij
 */ 
void changeDir()
{
	if (arguments[1] == NULL)	// ako nije napisano ime direktorija poslije 'cd' 
	{
		chdir(getenv("HOME"));
	}
	else
	{
		if (chdir(arguments[1]) == -1)	// ako ne uspije promjena trenutnog direktorija ispise poruku
		{
			printf(" %s: ne postoji taj direktorij\n", arguments[1]);
		}
	}
}


/* funkcija za slanje signala procesu
 */
void killProcess(int processID)
{
	kill(processID, SIGKILL);
}


/* ispis osnovnih funkcija shell-a i primjeri njihova koristenja
 */
void displayHelp()
{
	printf("\n\nNaredba                       Primjer koristenja");
	printf("\n\ncd: promjena direktorija      cd [ime_direktorija]");
	printf("\nkill: prekidanje procesa      kill [broj_procesa]");
	printf("\n>: preusmjeravanje izlaza     > [ime_datoteke] [naredba] [arg1] [argN]");
	printf("\n<: preusmjeravanje ulaza      < [ime_datoteke] [naredba] [arg1] [argN]");
	printf("\n|: pipelining                 [naredba] [arg1] [argN] | [naredba]");
	printf("\n&: izvodenje u pozadini       [naredba] [arg1] [argN] &");
	printf("\n& i > : kombinacija           > [ime datoteke] [naredba] [arg1] [argN] &");
	printf("\nhelp: prikaz pomoci");
	printf("\nexit: izlaz iz shell-a\n\n");
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
