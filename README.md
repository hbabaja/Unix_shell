Unix_shell
==========

Izvorni kod ljuske je podijeljen u tri datoteke:  
1.	defs.h – ova datoteka sadrži deklaracije svih funkcija, deklaraciju svih globalnih varijabli (argumenti, korisnikov unos itd.), biblioteke ljuske te macro-e koje ćemo koristiti.  
2.	function.h – ovdje su napisane definicije većine funkcija.  
3.	hbshell.c – ovo je glavno tijelo ljuske, sastoji se od main funkcije i definicija nekih funkcija (displayShell, displayHelp, changeDir ...).  

Ova UNIX ljuska ima slijedeće funkcionalnosti:  
1.	Parsiranje korisnikovog unosa  
2.	Prikaz radnog direktorija u svakom trenutku  
3.	Promjena radnog direktorija naredbom 'cd'  
4.	Izvršavanje naredbi  
5.	Pokretanje naredbe u pozadini s '&'  
6.	Presumjeravanje izlaza/ulaza naredbe u datoteku pomoću operatora '>' i '<'  
7.	Ulančavanje naredbi sa operatorom '|'  
8.	Prekidanje procesa naredbom 'kill'  
9.	Prikaz pomoći naredbom 'help'
