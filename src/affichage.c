#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <termio.h>
#include "../include/arborescence.h"


#define ESPACE 7 // Espace du texte dans la fonction echec() et felicitation()

// Clear the termirnal srceen
void screen_clear()
{
    system("clear");
}

/**
 * Affiche les lignes d'une phrase succesivement
 * @param a : Le nom du fichier à afficher
 */
void affichage_ligne(char *nom_fichier)
{
	FILE *fichier = NULL ;
	char texte[1000]="";

	fichier = fopen( nom_fichier , "r");
        if(fichier == NULL)
        {
                printf("!!! Erreur d'ouverture fichier !!!\n");
        }

	while( fgets(texte , sizeof(texte) , fichier) )
	{
		printf("%s",texte);
	//	fflush(stdout);
		usleep(10000) ;
	}

	fclose(fichier);
}

//  Affiche le fichier
void affiche_fichier(char *nom_fichier ) 
{
    
    screen_clear();
    affichage_ligne(nom_fichier );
}

/**
 * @fn echec
 * @brief Affiche une messege félicitation et recupère l'avis du joueur s'il souhaite passé au mission suivant ou pas 
 * @return Retourne un bool
 */
bool felicitation()
{
   screen_clear();

    struct winsize terminal ; // Structure contenant la taille du terminale
    int touche , choix ;
    int ligne = 1 ;

    ioctl( STDOUT_FILENO , TIOCGWINSZ , &terminal ); // Récupre la taille du términale
 
    initscr(); // Début ncurses

    curs_set(0); // Désactiver le curseur
    cbreak();   
    noecho();    // N'affiche pas les touche tapé par le joueur
    keypad(stdscr , TRUE ); // Activer les touche de direction du clavier

    start_color();      
    init_pair(1 , COLOR_MAGENTA , -1 );  // ID = 1 texte rouge sur fond noir
    init_pair(2 , COLOR_GREEN , -1 );    // texte en vert 

    mvprintw( terminal.ws_row/2 + 2 , (terminal.ws_col-strlen("MISSION TERMINÉE ! BRAVO SOLDAT !"))/2 , "==>");

        while(1)
        {
            attron(COLOR_PAIR(1) | A_BOLD );
                mvprintw( terminal.ws_row/2  , (terminal.ws_col-strlen("MISSION TERMINEE ! BRAVO SOLDAT !"))/2 , "MISSION TERMINEE ! BRAVO SOLDAT !");
            attroff(COLOR_PAIR(1) | A_BOLD);
            attron( COLOR_PAIR(2) | A_BOLD );
                mvprintw( terminal.ws_row/2 + 2 , (terminal.ws_col)/2 - ESPACE, "CONTINUER");
                mvprintw( terminal.ws_row/2 + 3 , (terminal.ws_col)/2 - ESPACE, "MENU PRINCIPALE");
            attroff( COLOR_PAIR(2) | A_BOLD);

            refresh();
            
            touche = getch(); // Récupère une touche
            
            if(touche == KEY_UP && ligne > 1 )
            {
                clear();
                mvprintw( terminal.ws_row/2 + 2 , (terminal.ws_col-strlen("MISSION TERMINÉE ! BRAVO SOLDAT !"))/2 , "==>");
                ligne--;
            }
            else if(touche == KEY_DOWN && ligne < 2)
            {
                clear();
                mvprintw( terminal.ws_row/2 + 3 , (terminal.ws_col-strlen("MISSION TERMINÉE ! BRAVO SOLDAT !"))/2 , "==>");
                ligne++;
            }
            else if( touche == '\n' && ligne == 1 )
            {
                clear();
                endwin();
                return true;
            }
            else if( touche == '\n' && ligne == 2)
            {
                clear();
                endwin();
                return false;
            }
        }
}
/**
 * @fn echec
 * @brief Affiche une messege d'echec et recupère l'avis du joueur s'il souhaite recommencé ou pas 
 * @return Retourne un bool
 */
bool echec()
{
    screen_clear();

    struct winsize terminal ; // Structure contenant la taille du terminale
    int touche , choix ;
    int ligne = 1 ;

    ioctl( STDOUT_FILENO , TIOCGWINSZ , &terminal );
 
    initscr(); // Début ncurses

    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr , TRUE );

    start_color();
    init_pair(1 , COLOR_MAGENTA , -1 );  // ID = 1 texte rouge sur fond noir
    init_pair(2 , COLOR_GREEN , -1 );    // texte en vert 

    mvprintw( terminal.ws_row/2 + 2 , (terminal.ws_col-strlen("MISSION ECHOUÉE ! RETENTEZ VOTRE CHANCE SOLDAT !"))/2 , "==>");

        while(1)
        {
            attron(COLOR_PAIR(1) | A_BOLD );
                mvprintw( terminal.ws_row/2  , (terminal.ws_col-strlen("MISSION ECHOUÉE ! RETENTEZ VOTRE CHANCE SOLDAT !"))/2 , "MISSION ECHOUEE ! RETENTEZ VOTRE CHANCE SOLDAT !");
            attroff(COLOR_PAIR(1) | A_BOLD);
            attron( COLOR_PAIR(2) | A_BOLD );
                mvprintw( terminal.ws_row/2 + 2 , (terminal.ws_col)/2 - ESPACE, "RECOMMENCER");
                mvprintw( terminal.ws_row/2 + 3 , (terminal.ws_col)/2 - ESPACE, "MENU PRINCIPALE");
            attroff( COLOR_PAIR(2) | A_BOLD);

            refresh();
            
            touche = getch();
                    
            if(touche == KEY_UP && ligne > 1 )
            {
                clear();
                mvprintw( terminal.ws_row/2 + 2 , (terminal.ws_col-strlen("MISSION ECHOUÉE ! RETENTEZ VOTRE CHANCE SOLDAT !"))/2 , "==>");
                ligne--;
            }
            else if(touche == KEY_DOWN && ligne < 2)
            {
                clear();
                mvprintw( terminal.ws_row/2 + 3 , (terminal.ws_col-strlen("MISSION ECHOUÉE ! RETENTEZ VOTRE CHANCE SOLDAT !"))/2 , "==>");
                ligne++;
            }
            else if( touche == '\n' && ligne == 1 )
            {
                clear();
                endwin();
                return true;
            }
            else if( touche == '\n' && ligne == 2)
            {
                clear();
                endwin();
                return false;
            }
        }
}


void afficher_tableau_avec_titre_position_exacte(const char *titre, const char *nom_fichier)
{
    FILE *fichier = fopen(nom_fichier, "r");
    char lignes[300][300];
    int nb = 0;
    int max_len = 0;
    int hauteur , largeur , starty , startx , x_centre , y_centre , longueur_ligne ;

    if (!fichier) 
    {
        endwin();
        printf("Erreur : impossible d'ouvrir %s\n", nom_fichier);
        return;
    }

    while (fgets(lignes[nb], sizeof(lignes[nb]), fichier) && nb < 300)
    {
        longueur_ligne = strlen(lignes[nb]);

        if (lignes[nb][longueur_ligne-1] == '\n')
            lignes[nb][longueur_ligne-1] = '\0';

        longueur_ligne = strlen(lignes[nb]);
        if (longueur_ligne > max_len)
        {
            max_len = longueur_ligne;
        }
        nb++;
    }
    fclose(fichier);

    hauteur = nb + 4;
    largeur = max_len + 4;

    if(hauteur > LINES)
    {
        hauteur = LINES - 2 ;
    }
    if(largeur > COLS)
    {
        largeur = COLS - 2;
    }

    y_centre = (LINES - hauteur) / 2;
    x_centre = (COLS - largeur) / 2;


    // Tableaus ascii
    WINDOW *win = newwin(hauteur, largeur, y_centre, x_centre );
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');

    // Titre (seul élément centré)
    wattron(win, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(win, 1, (largeur - strlen(titre)) / 2, "%s", titre);
    wattroff(win, COLOR_PAIR(5) | A_BOLD);

    // Texte NON CENTRÉ (respect exact des positions)
    for (int i = 0; i < nb; i++)
    {
        // Si c'est la dernierer ligne 
        if ( i == nb-1 )
        {
            // Dernière ligne clignotante + rouge
            wattron(win, COLOR_PAIR(5) | A_BOLD | A_BLINK);
            mvwprintw(win, i+2 , 2 , "%s", lignes[i]);
            wattroff(win, COLOR_PAIR(5) | A_BOLD | A_BLINK);
        }
        else 
        {            
            // Colore les majuscules en vert
            if ( lignes[i][0] >= 'A' && lignes[i][0] <= 'Z' )
            {
                wattron(win, COLOR_PAIR(4) | A_BOLD );
                mvwprintw(win, i+2, 2 , "%s", lignes[i]);
                wattroff(win, COLOR_PAIR(4) | A_BOLD );
            }
            else
            {
                mvwprintw(win, i+2, 2 , "%s", lignes[i]);
            }
        }
    
    }

    // ───── Rafraîchissement et pause
    refresh();
    wrefresh(win);

    while( getch() != '\n' )
    {
        getch();
    }
    
    delwin(win);
}

/**
 * @fn affiche_fichier_tableau_curse_position_exacte
 * @brief Avec un texte dans un tableau tel que est dans le fichier 
 * @param 1 : Le titre du tableau à afficher de couleur Rouge
 * @param 2 : chemin vers le fichier à afficher
*/
void affiche_fichier_tableau_curse_position_exacte(const char *titre , const char *nom_fichier)
{
    screen_clear();

    initscr();              // initialise ncurses
    clear();
    noecho();               // ne pas afficher les touches tapées
    cbreak();               // entrée sans mise en tampon
    curs_set(0);            // cache le curseur

    start_color();          // active les couleurs
    use_default_colors();   // garde fond terminal
    init_pair(1, COLOR_YELLOW, -1); // titre
    init_pair(4, COLOR_CYAN, -1);   // texte
    init_pair(5, COLOR_RED, -1);    // message qui clignote
    
    afficher_tableau_avec_titre_position_exacte(titre, nom_fichier);

   endwin(); // restaure le terminal
   clear();
}
