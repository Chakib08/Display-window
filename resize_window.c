#include <sys/ioctl.h>
#include <signal.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <curses.h>
#include <sys/time.h>

/************************************************************************/
/* pour compiler ce code tapez dans un terminal la commande suivante:	*/
/* gcc -o fenetre curses_exemple.c -lncurses				*/
/************************************************************************/


int x, y, xmax, ymax;
int nombreSecondes = 0; //initialisation  du nombre des secondes à 0
char str1[500];
void print_milieu(WINDOW *win, int debuty, int debutx, int largeur, char *texte);

/*Fonction du timer permettant d'afficher le nombre des secondes en utilisant la fonction settimer()*/
void timer_real (int signum)
{

/* incrementation du nombre des secondes ( avec la configuration des parametres du timer le nombre des secondes va s'incrementer chaque 1 seconde*/
 sprintf(str1,"nombre de secondes écoulées : %d",nombreSecondes++); 

}


/* fonction qui va permettre de tracer le cadre et qui sera dependant du terminal car cette fonction sera appellé a chaque interruption avec SINGWITCH */
void reSizeBorder()
{		
endwin();
clear();
refresh();
start_color();
init_pair(1, COLOR_RED, COLOR_BLACK);
attron(COLOR_PAIR(1));
getmaxyx(stdscr, ymax, xmax);
box(stdscr, ACS_VLINE, ACS_HLINE);
getyx(stdscr, y, x);
	
refresh();


}


int main (void) 
{
struct sigaction sar; //systeme qui va nous servir à démarrer notre timer à la réception du signal
struct itimerval timer; 
struct sigaction rez;//systeme qui va nous servir à ajuster notre cadre à chaque redimensionnement du terminal


  //configurer resizeBorder() comme routine d'interruption du signal SIGWINTCH
   memset (&rez, 0, sizeof (rez));
  rez.sa_handler = &reSizeBorder;
  sigaction (SIGWINCH, &rez, NULL);


  /* Configure timer_real comme routine d'interruption du signal SIGALRM.  */
  memset (&sar, 0, sizeof (sar));
  sar.sa_handler = &timer_real;
  sigaction (SIGALRM, &sar, NULL);

  /* Configure les timers pour une durée initiale de 1 microseconde pour obtenir le nombre de seconde écoulées sans retard...  */
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 1;
  /* ... et 1 seconde pour les suivantes.  */
  timer.it_interval.tv_sec = 1;
  timer.it_interval.tv_usec = 0;
  


  /* Démarre un timer reel. Il compte le temps écoulé pour le process courant et affiche le nombre de secondes écoulées chaque seconde  */
  setitimer (ITIMER_REAL, &timer, NULL);

// Routine d'interruption pour ajuster le cadre à chaque redimensionnement du terminal
  signal(SIGWINCH, &reSizeBorder);


volatile unsigned int i,j=0;

char str[256]="nombre de secondes écoulées = 0";

	if((initscr()) == NULL) 
	{
			perror("initscr");
			exit(EXIT_FAILURE);
	}
	if(has_colors() == FALSE)
	{	endwin();
		printf("Votre terminal ne prend pas en charge les couleurs\n");
		exit(1);
	}
	start_color();			/* Demarre le mode couleur 			*/
	init_pair(1, COLOR_RED, COLOR_BLACK);


	getmaxyx(stdscr, ymax, xmax);
	attron(COLOR_PAIR(1));
	
	
	mvaddstr(ymax/2, xmax/2, str);
	
	getyx(stdscr, y, x);
	
	refresh();
	
	box(stdscr, ACS_VLINE, ACS_HLINE);// mettre une bordure autour de la fenêtre
	refresh();
        
        //Pas de retour en monochrome pour laisser la bordure et le nombre de seconde en rouge
	//attroff(COLOR_PAIR(1)); // retour en monochrome pour les affichages suivants
	

	/* Boucle principale  */ 
	while (1)
	{
		usleep(2000000);
		
                getmaxyx(stdscr, ymax, xmax);
		print_milieu(stdscr,ymax/2, 0, xmax,str1);
               
	}
	return(EXIT_SUCCESS); // jamais executé
}

void print_milieu(WINDOW *win, int debuty, int debutx, int largeur, char *texte)
{	int longueur, x, y,temp;
	
	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	mvwprintw(win, 0, 0, "y=%d;x=%d",y,x);
	if(debutx != 0)
		x = debutx;
	if(debuty != 0)
		y = debuty;
	if(largeur == 0)
		largeur = x;
	longueur = strlen(texte);
	temp = (largeur - longueur)/ 2;
	x = debutx + temp;
	move(y,0);
	clrtoeol();
	refresh();
	mvwprintw(win, y, x, "%s", texte);
	touchwin(stdscr); // marque toute la fenêtre comme modifiée (oblige refresh à tout redessiner)
	refresh();
}

