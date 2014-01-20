/**********************************************************
 * Datoteka: functions.h
 * Autor: Hrvoje Babaja
 **********************************************************/


/* uzima znakove koje korisnik unosi i pravi niz stringova
 */
void getUserInput()
{
	initializeCommand();	
															// citaj i spremaj korisnikov unos 
	while ( (input != '\n') && (tempChars < MAX_LENGHT) )	// dok ne unese enter ili vise 														
	{														// od MAX_LENGHT znakova    
		tempLine[tempChars++] = input;								   
		input = getchar();
	}
	
	tempLine[tempChars] = 0x00;
	
	makeCommand();		
}


/* prazni niz stringova koji drzi naredbe
 * jer *arguments[] bude popunjen od proslih
 * unosa korisnika 
 */
void initializeCommand()
{
	while (nArguments != 0)
	{
		arguments[nArguments] = NULL;
		nArguments--;
	}
	tempChars = 0;
}


/* iz niza char-ova pravi niz stringova (makne razmake) 
 */
void makeCommand()
{
	char *tempPointer;
	
	tempPointer = strtok(tempLine, " ");	// razdijeli string u tokene         
											// vraca pointer na prvi byte tokena 
	while (tempPointer != NULL)
	{
		arguments[nArguments] = tempPointer;
		tempPointer = strtok(NULL, " ");
		nArguments++;
	}
}


/* provjerava niz stringova za naredbe i funkcije koje su
 * ugradene u shell, po potrebi ih salje na izvrsavanje u executeCommand
 */
int processUserCommand()
{
	if (strcmp("exit", arguments[0]) == 0)	// provjera za naredbu izlaza 
	{
		exit(EXIT_SUCCESS);
	}
	
	/* u slucaju da je prvi string "cd" poziva funckiju changeDir
	 */
	if (strcmp("cd", arguments[0]) == 0)	
	{
		changeDir();						
		displayShell();
		return 1;
	}
	
	/* u slucaju da je prvi string "help" poziva funkciju displayHelp 
	 */
	if (strcmp("help", arguments[0]) == 0)	//
	{
		displayHelp();
		displayShell();
		return 1;
	}
	
	/* ako je prvi string kill i sljedeci string nije NULL poziva se funkcija killProcess
	 * koja terminira proces sa unesenim process id-om
	 */
	if (strcmp("kill", arguments[0]) == 0)
	{
		if (arguments[1] == NULL)
		{
			printf("koristenje: kill [broj procesa]\n");
			return 0;
		}
		
		killProcess(atoi(arguments[1]));
		return 1;
	}
	
	/* ako je prvi string znak ">" i poslije njega ima jos argumenata
	 * preusmjerava se izlaz naredbe na datoteku poslije ">", u slucaju da je 
	 * zadnji string jednak "&" sve to se izvrsava u pozadini
	 */
	if ( (strcmp(">", arguments[0]) == 0) && (arguments[1] != NULL) )
	{
		if (strcmp("&", arguments[nArguments-1]) == 0)
		{
			arguments[nArguments-1] = NULL;
			nArguments--;
			executeCommand(arguments+2, arguments[1], STDOUT, BACKGROUND);
			return 1;
		}
		
		executeCommand(arguments+2, arguments[1], STDOUT, FOREGROUND);
		return 1;
	}
	
	/* ako je prvi string znak "<" i poslije njega ima jos argumenata
	 * preusmjerava se ulaz naredbe na datoteku poslije "<", u slucaju da je 
	 * zadnji string jednak "&" sve to se izvrsava u pozadini
	 */
	if ( (strcmp("<", arguments[0]) == 0) && (arguments[1] != NULL) )
	{
		if (strcmp("&", arguments[nArguments-1]) == 0)
		{
			arguments[nArguments-1] = NULL;
			nArguments--;
			executeCommand(arguments+2, arguments[1], STDIN, BACKGROUND);
			return 1;
		}
		
		executeCommand(arguments+2, arguments[1], STDIN, FOREGROUND);
		return 1;
	}
	
	/* ako je broj argumenata u nizu jednak 3, provjerava se je li srednji argument
	 * "|". ako je, poziva se funkcija pipe(2)
	 * vise detalja u funckiji pipe
	 */
	if (nArguments == 3)
	{
		if (strcmp("|", arguments[1]) == 0)
		{
			pipeline(2);
			return 1;
		}
	}
	
	/* ako je broj argumenata u nizu veci od 3, provjerava se je li na predzadnjem mjestu "|"
	 * u slucaju da je poziva se funckija pipe(broj_argumenata-1)
	 * vise detalja u funckiji pipe
	 */
	if (nArguments > 3)
	{
		if(strcmp("|", arguments[nArguments-2]) == 0)
		{
			pipeline(nArguments-1);
			return 1;
		}
	}
	
	/* ako je zadnji argument "&" naredba se izvrsava u pozadini
	 */
	if (strcmp("&", arguments[nArguments-1]) == 0)
	{
		arguments[nArguments-1] = NULL;
		nArguments--;
		executeCommand(arguments, "STANDARD", 0, BACKGROUND);
		return 1;
	}
	
	/* ako se do sada nije izaslo iz ove funkcije preko "return 1;"
	 * izvrsi naredbu normalno
	 */
	executeCommand(arguments, "STANDARD", 0, FOREGROUND);
	return 1;
}


/* funkcija za preusmjeravanj izlaza jedne naredbe na ulaz druge naredbe
 */
void pipeline(int f)
{
	char line[MAXLINE];
    FILE *fp_in, *fp_out;
    char argument1[50], argument2[50];
    int i;
	
	/* ako je f == 2, to znaci da svaka naredba ima samo jedan argument
	 * oni se nalaze na prvom i zadnjem mjestu
	 */
	if(f==2)
	{
	    strcat(argument1, arguments[0]);
	    strcpy(argument2, arguments[2]);
	}
	
	/* ako f != 2, to znaci da prva naredba ima vise argumenata, ovdje se uzimaju ti argumenti
	 * i stavljaju u string argument1
	 */
    else
	{
	   for(i=0; i<f-1; i++)
	   {
		   strcat(argument1, arguments[i]);
		   strcat(argument1, " ");
	   }
	   strcpy(argument2, arguments[nArguments-1]);
	}
	
    if ((fp_in = popen(argument1, "r")) == NULL)
	{
        printf("Ne moze se otvoriti %s", argument1);
	}
	
    if ((fp_out = popen(argument2, "w")) == NULL)
    {
		printf("Ne moze se otvoriti %s", argument2);
	}

    while (fgets(line, MAXLINE, fp_in) != NULL) 
	{
        if (fputs(line, fp_out) == EOF)
		{
            printf("fputs greska na pipe-u");
		}
    }
	
    if (ferror(fp_in))
	{
        printf("fgets greska");
	}
	
    if (pclose(fp_out) == -1)
    {
		printf(" ");
	}

}

/* pokrece naredbu, prima argumente, pointer na ime datoteke na koju se preusmjerava
 * izlaz ili ulaz, nacin izvrsavanj(background ako je na kraju naredbe '&', inace foreground)
 */
void executeCommand(char *cmd[], char *filePointer, int std , int mode)
{
	pid_t pid;
	int cmdStd;		// sluzi za otvaranje datoteke za ulaz ili izlaz
	fflush(stdout);
	
	pid = fork();
	
	switch (pid)
	{
		case -1:
			printf("Fork failure\n");
			fflush(stdout);
			exit(EXIT_FAILURE);
			break;
			
		case 0:
			fflush(stdout);
			signal(SIGINT, SIG_DFL);	// omogucuje signale poput ctrl+c   
			signal(SIGQUIT, SIG_DFL);
			signal(SIGSTOP, SIG_DFL);
			signal(SIGCHLD, &childSignalHandler);
			
			/* zamjena standardnog ulaza sa datotekom koja se nalazi na koju pokazuje filePointer
			 */
			if (std == STDIN) 
			{
                cmdStd = open(filePointer, O_RDONLY);
                dup2(cmdStd, STDIN_FILENO);
                if(cmdStd != STDIN_FILENO)
					close(cmdStd);
			}
			
			/* zamjena standardnog izlaza sa datotekom koja se nalazi na koju pokazuje filePointer
			 */
			if (std == STDOUT) 
			{
                cmdStd = open(filePointer,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(cmdStd, STDOUT_FILENO);
                if (cmdStd != STDOUT_FILENO)
					close(cmdStd);
			}
			
			/* ako se naredba izvrsava u pozadini ispisi process id, da korisnik moze prekinuti tu
			 * naredbu sa naredbom kill
			 */
			if (mode == BACKGROUND)
			{
				fprintf(stderr,"BG Job pid = %d started\n",getpid());
				displayShell();
				fflush(stdout);
			}
			
			/* pokretanje naredbe
			 */
			if (execvp(*cmd, cmd) == -1)
			{
                printf("%s : greska pri pokretanju\n",cmd[0]);
                fflush(stdout);
                exit(0);
			}
			
            exit(EXIT_SUCCESS);
            break;
            
		default:
			/* ako se naredba ne izvrsava u pozadini, makne se handler za child signal
			 * i ne ceka se na zavrsetak, nakon prikaza shella vraca se child signal handler
			 */
			if (mode == FOREGROUND)
			{
				signal(SIGCHLD, NULL);
				waitpid(pid, NULL, 0);
				displayShell();
				signal(SIGCHLD, &childSignalHandler);
			}
			else
			{
				signal(SIGCHLD, &childSignalHandler);
			}
			
			break;
	}
	
	
}

/* handler za SIGCHLD
 */
void childSignalHandler(int p)
{
	pid_t pid;
	int term_status;
	pid = waitpid(WAIT_ANY, &term_status, WUNTRACED | WNOHANG);   // ceka na bilo koji pid
	
	if (pid > 0)
	{
		if (WIFEXITED(term_status))		// pozadinska naredba normalno zavrsena
		{
			fprintf(stderr,"BG Job pid = %d done\n", pid);
			displayShell();
			fflush(stdout);
			return;
		}
		else if (WIFSIGNALED(term_status))	  // pozadinska naredba naglo prekinuta sa "kill"
		{
			fprintf(stderr,"BG Job pid = %d killed\n", pid);
			displayShell();
			fflush(stdout);
			return;
		}
	}
}



/* prikaz svih procesa koji se izvrsavaju u pozadini i njihov broj koji mozemo
 * koristit sa funkcijom 'kill'
 */
/*void processDisplay()
{
	process* temp_list = process_list;
	
	if (temp_list == NULL)
	{
		printf("\nNema procesa u pozadini\n");
	}
	else
	{
		printf("\n %20s %7s","Ime","broj procesa");
		while (temp_list != NULL)
		{
			printf("\n %20s %7d",temp_list->name,temp_list->number);
			temp_list=temp_list->next;
		}
		printf("\n");
	}
}*/ //nije implementirano jos


