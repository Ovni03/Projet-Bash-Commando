
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <locale.h> // Pour les accents (setlocale)
#include "../include/arborescence.h"
#include "../include/affichage.h"
#include "../include/mission.h"
#include "../include/menu.h"
#include "../include/prompt.h"


int main()
{

    Mission *mission = NULL ;
    fichier *racine = NULL ; 

    setlocale(LC_ALL, "");
    racine = creer(NULL , "AK-07" , 1 , 1 ); // création du racine
    mission = malloc( NOMBRE_MISSION * sizeof(Mission) ); // Allocation du structure mission 

    loging();
    load_struct_mission(mission) ;  // Chargement du structure mission
    map_initialisation( mission , racine ) ; // Construction du map du jeu

    menu_pricipale(mission , racine );

    free(mission); // liberation du mémoire alloué au struct mission

    return 0 ;

} 
