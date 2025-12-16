#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/arborescence.h"
#include "../include/prompt.h"
#include "../include/mission.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <ncurses.h>

#define LARGEUR 70 // Largeur du tableau curses
/**
 * @fn soddierExiste
 * @brief Vérifie si un fichier est un dossier ou pas 
 * @param 1 : Chemin vers le fichier à vérifier
 */
int dossierExiste(const char *chemin)
{
    struct  stat info ;

    if (stat(chemin , &info) != 0 ) // Si le dossier n'existe pas
    {
            return 0;
    } 

    // S_ISDIR vérifie que c'est bien un dossier
    return S_ISDIR(info.st_mode) ;
}

/**
 * @fn loging
 * @brief : Fonction qui récupère le loging du joueur et accorde le sauvegarde qui lui est attribué s'il a déja jouer si non crée un nouveau dossier de sauvegarde pour l'user
 */

void loging()
{
    char *loging = NULL ;
    FILE *fichier_temporaire = NULL ;
    char *chemin = calloc(50 , sizeof(char));

    fichier_temporaire = fopen("../save/temp.txt" , "w+");

    if ( !fichier_temporaire )
    {
        printf("Erreur : Fichier temporaire introuvable\n");
        exit(0);
    }
   
    loging = calloc(50 , sizeof(char)); 
    // Erreur de recuperation du loging
    if( getNomJoueur(loging) == -1)
    {
        printf("Erreur : Impossible de récupérer le loging !!\n");
        exit(0);
    }
    
    fprintf(fichier_temporaire , "../save/%s", loging);
    rewind(fichier_temporaire);
    fscanf( fichier_temporaire , "%s" , chemin );

    // Verification si le joueure possede deja un sauvegarde si no créer
    if ( dossierExiste(chemin) == 0 )
    {
        if ( mkdir(chemin , 0755 ) != 0)
        {
            printf("Erreur : Impossible de crée le dossier.\n");
        }
    }

    // ecriture du chemin de level.txt dans temp.txt
    rewind(fichier_temporaire);
    fprintf(fichier_temporaire , "%s/level.txt\n", chemin );

    // Ecriture du chemin de mission.bin dans temp.txt
    fprintf(fichier_temporaire , "%s/mission.bin\n", chemin );

    // Ecriture du chemin de point
    fprintf(fichier_temporaire , "%s/pTSci", chemin );

    free(loging);
    fclose(fichier_temporaire);

}

//-----------------------------------------------------------------------------------------------------------------------

/**
 * @fn load_game
 * @brief Charger la mission choisie par l'user a travers une tableau de fonction
 * @param 1 : struct mission
 * @param 2 : Rang du mission que l'user souhaite joué
 * @param 3 : struct fichier 
 */
void load_game(Mission *mission , int rang_du_dernier_mission , fichier *racine)
{
    // Création du tableau de pointeurs pointant sur les fonctions mission
    void (*fonctions_mission[])(Mission * , fichier *) = { intro , mission_1 , mission_2 , mission_3  , mission_4 , mission_5 , mission_6 , mission_7 , mission_8 , mission_9 ,  mission_10 
          ,  mission_11 ,  mission_12 ,  mission_13 , mission_14 };

    // Appel du mission du dérnier mission ; 
    fonctions_mission[rang_du_dernier_mission]( mission , racine );
}

//-----------------------------------------------------------------------------------------------------------------------

/**
 * @fn charge_partie
 * @brief Affiche tout les partie débloqué laisse l'user choisir la partie qu'il veut rejouer
 * @param 1 : struct mission
 */
int charger_partie(Mission *mission)
{
    int i = 0 , nb_ligne , j = 0 , nbMot = 0 , MAX = 0 ,
    touch = 0 , hauteur = 0 , largeur = 0 , posix = 0 , posy = 0 , choix  = 0;
    char TAB[NOMBRE_MISSION][50] ;

    FILE *fichier=fopen( "../data/selection_partie.txt" , "r" );

        while( i < NOMBRE_MISSION && mission[i].debloqué == 1 && fgets(TAB[i] , 50 , fichier) )
        {
            nbMot = strlen(TAB[i]);

            if ( TAB[i][nbMot-1]== '\n') 
            {
                TAB[i][nbMot-1] = '\0';
            }

            if (nbMot > MAX )
            {
                MAX = nbMot ;
            }

            i++ ;

            nb_ligne = i ;
        }

    fclose(fichier);

    // Pour ajouter un peu de marge au tableau
        hauteur = i + 6 ;
        largeur = MAX ;
    // Pour trouver le centre du terminal
        posix = (COLS - LARGEUR) / 2 ;
        posy = (LINES - i) / 2   ;
        
    // Création du tableau win
        WINDOW *win = newwin(hauteur , LARGEUR , posy , posix ) ;

    // Config curses
        keypad(win , true );
        curs_set(0);
        noecho();
  
    while(1)
    {
            // Effacer la fenêtre (pas l'écran principal)
        werase(win);
        wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');

        // Affichage Selection partie
        wattron(win, COLOR_RED | A_BOLD);
        mvwprintw(win, 1, (LARGEUR - strlen("SELECTION PARTIE")) / 2, "SELECTION PARTIE");
        wattroff(win, COLOR_RED | A_BOLD);

        // Affichage des missions débloquées
        for (j = 0 ; j < nb_ligne ; j++)
        {
            if(j == choix)
            {
                wattron(win, COLOR_GREEN | A_BOLD);
                  mvwprintw(win, j + 3, 2 , "==> %s", TAB[j]);
                wattroff(win, COLOR_GREEN | A_BOLD);
            }
            else
            {
                mvwprintw(win, j + 3, 6, "%s", TAB[j]);
            }
        }

        // Instructions
        mvwprintw(win, hauteur - 2 , 2, "Utilisez les flèches et appuyez sur ENTRÉE ou q ");

        wrefresh(win);  // Rafraîchir la fenêtre

        touch = wgetch(win);

        if (touch == KEY_UP && choix > 0)
        {
            choix-- ;
        }
        else if(touch == KEY_DOWN && choix < nb_ligne - 1)
        {
            choix++ ;
        }
        else if( touch == KEY_UP && choix == 0 )
        {
            choix = nb_ligne - 1 ;
        }
        else if( touch == KEY_DOWN && choix == nb_ligne - 1 )
        {
            choix = 0 ;
        }
        else if (touch == '\n' || touch == KEY_ENTER)
        {
            delwin(win);
            clear();
            refresh();
            return choix ; // Retourne le rang de la mission choisie            
        }
        else if (touch == 27 || touch == 'q')  // ESC ou 'q' pour quitter
        {
            delwin(win);
            clear();
            refresh();
            return -1; // Indiquer que l'utilisateur a annulé            
        }
    }
    
}

//--------------------------------------------------------------------------------
/**
 * @fn quit_game
 * @brief Fonction qui libere le struct mission avant de quitter le jeu
 * @param 1 : Struct mission
 */
void quit_game( Mission *mission ) 
{
    printf("\033[1;31m");
    affiche_fichier("../ascii/quit.txt");
    printf("\033[0m");    
    
    free(mission); // liberation du mémoire alloué au struct mission
    exit (0) ;
}


void ptit_prompt(char *nom)
{

    char *user = getenv("USER");

    if( strcmp( "USER" , nom ) == 0)
    {
        printf("\n\t==> \033[1;33m%s\033[0m\033[1;34m@\033[0m\033[1;31mBash-Commando:\033[0m ", user);
    }
    else
    {
        printf("\n\t==> \033[1;33m%s\033[0m\033[1;34m@\033[0m\033[1;31mBash-Commando:\033[0m ", nom);
    }
}

/**
 * @fn menu_principale
 * @brief Fonction qui regle le menu principale du jeu (affichage , selection partie et continuer partie)
 * @param 1 : Struct mission
 * @param 2 : Struct fichier
 */
void menu_pricipale(Mission *mission , fichier *racine )
{
    int choix = 0 ;
    int compteur = 0 ;
    FILE *save = NULL ;
    int rang_du_dernier_mission = 0 ;
    char chemin_level[30] = {""};
    char chemin_mission[30] = {""};

    while(1)
    {
        do
        {
            if (compteur == 0)
            {
             affiche_fichier("../ascii/menu.txt" );   
            }
            else
            {
                printf("\t%d : Commande introuvable\n", choix);
            }

            ptit_prompt("USER");
            choix=lireEntier();
            compteur ++ ;
        }while( choix <= 0 || choix > 4 );
        
        compteur = 0 ; // Réinitialisation du compteur a zero
        switch (choix)
        {
        case 1 :
            // Regarde la dérniere mission jouer par l'utilisateur
            save = fopen( "../save/temp.txt" , "r");
            fscanf( save , "%s" , chemin_level );
            fscanf( save , "%s" , chemin_mission );
            fclose(save);
            save = fopen( chemin_level , "r" ) ;

            if( save == NULL )
            {
                // Si le fichier n'existe pas , on commence une nouvelle partie
                load_game( mission , 0 , racine);
            }
            else
            {
                fscanf( save , "%d" , &rang_du_dernier_mission ) ;
                fclose( save ) ;   
            }
            load_game( mission , rang_du_dernier_mission , racine);
        
        break;
        case 2 :
            screen_clear();
            endwin();
            initscr();
            clear();
            refresh();
            choix = charger_partie(mission);
                if (choix != -1 )
                {
                    load_game(mission , choix+1 , racine );
                }
            endwin();    
        break;
        case 3 :
            réinitialisation_mission(mission);
        break;
        case 4 :
            quit_game( mission );
            return ;
        break;
        default:
            printf("Option invalide !!!\n\n");
        }
    }       
} 
