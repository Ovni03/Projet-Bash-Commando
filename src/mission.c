/**
 * @file mission.c
 * @brief : Fonction pour gérer les missions du jeu
 * @author : RAKOTOARIVONY Zo Mamitiana Olivier
 * @date : 03 déc 2025
 */

/************************************************************************************* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/sécure.h"
#include "../include/affichage.h"
#include "../include/gestion_fichier.h"
#include "../include/prompt.h"
#include "../include/mission.h"
#include "../include/menu.h"


/**
 * @fn     map_initialisation
 * @brief  Initialise le map dans le jeux
 * @param  mission : Structur mission
 * @param  racine : Structur fichier ( racine ) l'arboressence du jeu
 */
void map_initialisation(Mission *mission , fichier *racine)
{
   fichier *courant = NULL ;

   my_mkdir(racine , racine , "Poste_Commandement" , 1) ;
        courant=my_cd(racine,racine,"Poste_Commandement") ;
        my_mkdir(racine,courant,"cartes_stratégiques", 1);
        my_mkdir(racine,courant,"transmission", 1);
        my_mkdir(racine,courant,"journal_missions", 1);
    my_mkdir(racine , racine , "Quartier_Généraux" , 1) ;
        courant=my_cd(racine,racine,"Quartier_Généraux" ) ;
        my_mkdir(racine,courant,"bureau_du_général" , 1);
        my_mkdir(racine,courant,"salle_briefing" , 1);
        my_mkdir(racine,courant,"système_sécurité" , 1);
    my_mkdir(racine, racine , "Hangar", 1);
        courant=my_cd(racine,racine,"Hangar");
        my_mkdir(racine,courant,"mechas", 1);
        my_mkdir(racine,courant,"vehicules_blindes", 1);
        my_mkdir(racine,courant,"carburant", 1);
    my_mkdir(racine , racine , "Caserne", 1);
        courant=my_cd(racine,racine,"Caserne") ;
        my_mkdir(racine,courant,"dortoirs", 1);
        my_mkdir(racine,courant,"infirmerie", 1);
        my_mkdir(racine,courant,"armurerie", 1);
    my_mkdir(racine , racine , "Laboratoire", 1);
        courant=my_cd(racine,racine,"Laboratoire") ;
        my_mkdir(racine,courant,"prototypes", 1);
        my_mkdir(racine,courant,"biogenierie", 1);
        my_mkdir(racine,courant,"archives_secretes", 1);
    my_mkdir(racine , racine , "Zone_Entrainement", 1);
        courant=my_cd(racine,racine,"Zone_Entrainement") ;
        my_mkdir(racine,courant,"tir", 1);
        my_mkdir(racine,courant,"combats", 1);
    my_mkdir(racine , racine , "Bunker_Souterrain", 1);
        courant=my_cd(racine,racine,"Bunker_Souterrain") ;
        my_mkdir(racine,courant,"salle_serveurs", 1);
        my_mkdir(racine,courant,"chambre_de_contrôle", 1);
        my_mkdir(racine,courant,"zone_niveau", 1);
}

//--------------------------------------------------------------------------------------------------------------

/**
 * @fn     load_struct_mission
 * @brief  Charge la structure mission depuis un fichier
 * @param  mission : Structur mission
 */

void load_struct_mission(Mission *mission )
{
   FILE *save = NULL , *tiavina = NULL ;
   char chemin_level[30] = {""};
   char chemin_mission[30] = {""};

   save = fopen( "../save/temp.txt" , "r");   
      fscanf( save , "%s" , chemin_level );
      fscanf( save , "%s" , chemin_mission );
   fclose(save);

   save = fopen( chemin_mission , "rb") ;
      if ( save == NULL )
      {
         return  ;
      }
      else
      {
         fread( mission , sizeof(Mission) , NOMBRE_MISSION , save );
      }
   fclose( save );

}
//--------------------------------------------------------------------------------------------------------------

/**
 * @fn     sauvegarde_dernier_partie
 * @brief  Sauvegarde le rang du dernier mission términée
 * @param  mission : Structur mission
 * @param  rang_mission_actuel : Rang du mission actuelle
 */
void sauvegarde_dernier_partie( Mission *mission , int rang_mission_actuel )
{
   FILE *save = NULL ;
   char chemin_level[30] = {""};
   char chemin_mission[30] = {""};

   // Sauvegarde du dernier partie términer pour que le joueur puisse continuer la où il etait la derniere fois

   save = fopen( "../save/temp.txt" , "r");
   if( save == NULL ) // Si le fichier est introuvable
   {
      printf("Erreur : fichier temp.txt introuvable\n") ;
      exit(0) ;
   }
   fscanf( save , "%s" , chemin_level );
   fscanf( save , "%s" , chemin_mission );
   fclose(save);
   
   save = fopen( chemin_level , "w" );
   
   if( save == NULL ) // Si le fichier est introuvable
   {
      printf("Erreur : fichier level.txt introuvable\n");
   }
   else
   {
      fprintf(save , "%d", rang_mission_actuel) ;
      fclose(save) ;
   }

   // Sauvegarde du status mission dans le fichier pour la fonction charger_partie

   save = fopen( chemin_mission , "wb");

   if(save == NULL) // Si le fichier est introuvable
   {
      printf("Erreur : fichier level.txt introuvable\n");
   }
   else
   {
      fwrite(mission , sizeof(Mission) , NOMBRE_MISSION , save );
      fclose(save);
   }

}

//--------------------------------------------------------------------------------------------------------------

/**
 * @fn réinitialisation_mission
 * @brief Réinitialise toutes les missions
 * @param mission : Structur mission
 * 
 */
void réinitialisation_mission(Mission mission[])
{
   int i = 0 ;
   FILE *save = NULL; // fichier de sauvegarde du dernière mission
   char chemin_level[30] = {""} ;
   char chemin_mission[30] = {""} ;
   char chemin_point[30] = {""} ;
   
   // Reinitialisation du structure mission 

   for( i = 0 ; i < NOMBRE_MISSION ; ++i )
   {
      if (i == 0)
      {
         mission[i].debloqué = 1 ;
         strcpy(mission[i].titre , "MISSION-1 : PWD") ;
      }
      else
      {
         mission[i].debloqué = 0 ;
      }
      
      mission[i].terminé = 0 ;
   }

   save = fopen( "../save/temp.txt" , "r");
   fscanf( save , "%s" , chemin_level );
   fscanf( save , "%s" , chemin_mission );
   fscanf( save , "%s" , chemin_point );
   fclose(save);

   // Sauvegarde du status mission dans le fichier pour la fonction charger_partie
   save = fopen( chemin_mission , "wb");
      if(!save) // Si le fichier est introuvable
      {
         printf("Erreur : fichier mission.bin introuvable\n");
         exit(0);
      }
      else
      {
         fwrite(mission , sizeof(Mission) , NOMBRE_MISSION , save );
      }
   fclose(save);
   
   // Reinitialiser le rang du mission à 0
   save = fopen( chemin_level , "w" );
      if(!save)
      {
         printf("Erreur : fichier level.txt introuvable\n");
         exit(0);
      }
      else
      {
         fprintf(save, "%d", 0);
      }
   fclose(save);
   
   // Reinitialisation du point à 0
   save = fopen( chemin_point , "w" );
      if(!save)
      {
         printf("Erreur : fichier level.txt introuvable\n");
      }
      else
      {
         fprintf(save, "%d", 0);
      }
   fclose(save);

}

//--------------------------------------------------------------------------------

/**
 * @fn verification_reussite
 * @brief Fonction verifiant si la mission est términée ou pas
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 * @param reuissite :   booléen indique les status du missions
 * @param num_mission : Numéro du mission actuel 
 */
void verification_reussite(Mission *mission , fichier *racine , bool reuissite , int num_mission )
{
   // Ecriture de la rapport apropos du mission
    //  rapport_mission(num_mission , reuissite) ;
   if(reuissite) // mission reussie ecrit félicitation
   { 
      // Changement statu mission 
      mission[num_mission - 1 ].terminé = 1 ;  // Mission actuel términé
      mission[num_mission ].debloqué = 1 ; // Mission suivant débloqué
      sauvegarde_dernier_partie( mission , num_mission + 1); // Sauvegarde du dernier rang
      increment_score(mission[num_mission].point);


      if(felicitation()) // Si le joueur souhaite continuer la partie
      {
         load_game(mission , num_mission + 1 , racine ) ; 
      }
      else // Si le joueur veut retourner au menu principal
      {
         menu_pricipale( mission , racine ) ;
      }
   }
   else  // Mission Faild
   {
      decrement_score(1);
      if (echec()) // Si le joueur veut recommencer le mission
      {
         load_game( mission , num_mission , racine ); 
      }
      else       // S'il veut retourner au menu principale
      {
         menu_pricipale( mission , racine );
      }   
   }
}

//--------------------------------------------------------------------------------
/**
 * @fn intro
 * @brief Fonction d'introduction du jeu
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void intro(Mission *mission , fichier *racine)
{
   setlocale(LC_ALL, "");
   affiche_fichier_tableau_curse_position_exacte("BIEVENUE SUR BASH COMMANDO" , "../data/intro.txt");  
  
   mission_1(mission , racine);
   
}

//--------------------------------------------------------------------------------
/**
 * @fn prompt_mission
 * @brief Affiche le prompt durant la patrie et analyse les commandes entrées par l'user
 * @param 1 : Structur racine contenant les arborescence du jeu
 * @param commande : La commande a vérifier durant la partie
 * @param titre : Le titre du mission actuelle
 * @param chemin : Chemin vers le fichier contenat le mission actuel
 * @param rep_courant : Repertoire courant du joueur dans la partie
 * @param mission_actuelle : Structur du mission actuelle
 */
bool prompt_mission(fichier *racine , char *commande , char *titre , char *chemin , char *rep_courant  , Mission *miss_actuelle , int rang_mission , int autoverifi)
{
   bool sortir = false , reussite  = false ;
   int nbMot , i ;
   clock_t debut , fin;
   double temps_seconde;
   char **ligne ;
   char *temps_ecoule ;
   fichier *tmp ;
   char  *ligne_complet = calloc( LIGNE , sizeof(char));

   tmp = racine ;
   ligne = calloc( LIGNE , sizeof(char *));
   temps_ecoule = calloc( LIGNE , sizeof(char));

   for(nbMot=0 ; nbMot<LIGNE ; nbMot++)// atao anaty tableau 2D ilay ligne amzay mora ny ahafantarana ny argument ex : ls -l --> ligne[0]="ls" ; ligne[1]="-l" 
   {
      ligne[nbMot] = calloc( MOT , sizeof(char));
   }
   
   debut = clock();
   while( !(sortir) ) // faire une prompt tant que la commande sortir est false
   {
      affiche_prompt(tmp); // affiche une prompt avec rep_actuel : fichier *tmp
      for(nbMot=0 ; nbMot<LIGNE ; nbMot++)// atao anaty tableau 2D ilay ligne amzay mora ny ahafantarana ny argument ex : ls -l --> ligne[0]="ls" ; ligne[1]="-l" 
      {
         ligne[nbMot][0] = '\0';            
      }

      nbMot = prend_ligne_command(ligne , LIGNE, tmp);
      // Recuperation du repertoire courant
      get_pwd( tmp , rep_courant );

      ligne_complet[0]='\0' ;
      if(strcmp(ligne[0] , commande ) == 0) // la mission est reussi si l'utilisateur ecrit la commande voulue
      {
         reussite = true ;
      }
      else
      {
         for(i=0 ; i<nbMot ; i++)
         {
            strcat(ligne_complet , ligne[i]);
            if(i != (nbMot-1))
            {
               strcat(ligne_complet , " ");
            }
         }
         if(strcmp(ligne_complet , commande) == 0)
         {
            reussite = true;
         }
      }
   
      if( strcmp(ligne[0] , "mission" ) == 0 )
      {
         affiche_fichier_tableau_curse_position_exacte( titre , chemin );
      }
      else if(strcmp(ligne[0] , "indice" ) == 0)
      {
         if(nbMot != 1)
         {
            printf("(EE : Pas d'argument autorisee !\n");
         }
         else
         {
            voir_indice(miss_actuelle) ;
         }
      }
      else if (strcmp(ligne[0] , "stat" ) == 0) // Si l'user veut checker le status de la mission
      {
         if(autoverifi == 1) // Si l'auto verification est activée
         {
            if(reussite == true )
            {
               printf("Mission terminé soldat !\nVous pouvez taper sur exit !\n");
            }
            else
            {
               printf("Mission non terminé soldat !\n");
            }
         }
         else
         {
            if( verification_mission(miss_actuelle , rang_mission , racine , rep_courant) == true )
            {
               printf("Mission terminé soldat !\nVous pouvez taper sur exit !\n");
            }
            else
            {
               printf("Mission non terminé soldat !\n");
            }
         }
      }
      else
      {
         tmp = execute(ligne , tmp , nbMot ); // return NULL si la ligne ecrit est "exit"
      }
      
      if(tmp == NULL )
      {
         break;
      }
   }
   fin = clock();

   temps_seconde = (double) (fin - debut);
   temps_seconde /= CLOCKS_PER_SEC;
   sprintf(temps_ecoule , "TEMPS_DERNIER_PROMPT=%lf" , temps_seconde);
   putenv(temps_ecoule);
   
   free(ligne_complet);

   return (reussite);
}

//------------------------------------------------------------------------------------------------

/**
 * @fn mission_1
 * @brief Afficher le répertoire courant
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_1( Mission *mission , fichier *racine ) // Afficher le répertoire courant
{

   bool reussite = false ;
   char *courant = calloc(MOT , sizeof(char)) ;
   
   mission[1].point = 0 ;
  
   // affiche_fichier_tableau_curse("MISSION 1", "../data/mission_1.txt");
      affiche_fichier_tableau_curse_position_exacte( "MISSION 1" , "../data/pwd.txt" );

   reussite = prompt_mission(racine , "pwd" , "MISSION 1" , "../data/pwd.txt" , courant , (mission+1) , 1 , 1 ) ;

   if(reussite == true )
   {
      affiche_fichier_tableau_curse_position_exacte( "MISSION 1" , "../data/cours_1.txt" );
   }
   
   verification_reussite( mission , racine , reussite , 1 );
}

//--------------------------------------------------------------------------------

/**
 * @fn mission_2
 * @brief Lister les fichiers et dossiers dans le répertoire courant
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_2(Mission *mission, fichier *racine)
{
    bool reussite = false;
    char *courant = calloc(MOT, sizeof(char));
    mission[2].point = 1;

    strcpy(mission[2].titre, "MISSION-2 : LS");
    affiche_fichier_tableau_curse_position_exacte("MISSION 2", "../data/mission_2.txt");

    // -------- FILS 1 : Chrono --------
    pid_t pid_chrono = fork();
    if(pid_chrono == 0)
    {
         int max_sec = 5;
         for(int i = 0; i < max_sec*10; ++i)
         {
            usleep(100000);
         }

         exit(1); // code 1 = temps écoulé
    }

    // -------- FILS 2 : Mission --------
    pid_t pid_mission = fork();
    if(pid_mission == 0)
    {
        // exécute prompt_mission dans un sous-fils
            reussite = prompt_mission(racine, "ls", "MISSION 2", "../data/mission_2.txt",courant, (mission+2), 2, 1) ;
            exit(reussite ? 0 : 2 ) ;
    }

    // -------- PÈRE : supervise fils chrono et fils mission --------
    int status;
    pid_t fini;

    while(1)
    {
        // vérifie si le fils mission a terminé
        fini = waitpid(pid_mission, &status, WNOHANG);
        if(fini == pid_mission)
        {
            reussite = (WIFEXITED(status) && WEXITSTATUS(status) == 0);
            // tuer le chrono si la mission a fini
               kill(pid_chrono, SIGKILL);
               break;
        }

        // vérifie si le chrono a fini
        fini = waitpid(pid_chrono, &status, WNOHANG);
        if(fini == pid_chrono)
        {
            // temps écoulé → tuer le fils mission
            kill(pid_mission, SIGKILL);
            affiche_fichier_tableau_curse_position_exacte("" , "../data/vide");
            reussite = false ;
            break;
        }

        usleep(5000);
    }

    // nettoyage des fils
    waitpid(pid_chrono, NULL, 0);
    waitpid(pid_mission, NULL, 0);

    // -------- étape suivante si la mission réussie --------
    if(reussite)
    {
        reussite = true;
        affiche_fichier_tableau_curse_position_exacte("MISSION 2", "../data/mission_2_1.txt");
        reussite = prompt_mission(racine, "ls Quartier_Généraux/", "MISSION 2","../data/mission_2_1.txt", courant, (mission+2), 2, 1);
    }

    verification_reussite(mission, racine, reussite, 2);
}

// --------------------------------------------------------------------------------------------------------------

/**
 * @fn verification_mission
 * @brief Fonction verifiant si la mission est términée ou pas
 * @param mission : Structur mission
 * @param rang_mission : Numéro du mission actuelle
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 * @param courant : Repertoire courant du joueur dans la partie
 */
bool verification_mission(Mission *mission , int rang_mission , fichier *racine , char* courant)
{
   fichier *repertoire_courant = NULL ;
   fichier *repertoire_courant_1 = NULL ;
   char chemin[] ={"Caserne/amurerie"};

   switch (rang_mission)
   {
   case 3 :
      // Verification si l'user est dans camp

      if( strcmp(courant , "/AK-07/Zone_Entrainement/combats/") == 0 )
      {
         return true ;
      }
      else
      {
         return false ;
      }     
   
   break;
   case 4 :
      // Verification si l'user est dans camp

      // On se deplace d'abord dans le repertoire "/AK-07/Caserne/dortoirs"
      repertoire_courant = my_cd( racine , racine , "Caserne");
      repertoire_courant = my_cd( racine , repertoire_courant , "dortoirs");

      // On cherche dans le reperptoire si l'user à bien crée "tente_1"
      if (chercher( repertoire_courant , "tente_1") != NULL )
      {
         return true ;
      }
      else
      {
         return false ;
      }

   break;
   case 5 :

   // On se deplace d'abord dans le repertoire "/AK-07/Caserne/dortoirs"
      repertoire_courant = my_cd( racine , racine , "Caserne");
      repertoire_courant = my_cd( racine , repertoire_courant , "dortoirs");
   // On cherche dans le reperptoire si l'user à bien supprimer la tente_2 et tente_3
      if ( (chercher( repertoire_courant , "tente_2") == NULL ) && (chercher( repertoire_courant , "tente_3") == NULL ) )
      {
         return true ;
      }
      else
      {
         return false ;
      }

   break;
   case 6 :

      // On cherche dans le reperptoire  "/AK-07/Caserne/armurerie" si l'user a bien fabriquer "pistolet"
         repertoire_courant = my_cd( racine , racine , "Caserne");
         repertoire_courant = my_cd( racine , repertoire_courant , "armurerie");
      if ( (chercher( repertoire_courant , "pistolet") != NULL ) ) // Si pistolet trouvé
      {
         return true ;
      }
      else
      {
         return false ;
      }

   break;
   case 7 :

      repertoire_courant = my_cd( racine , racine , "Zone_Entrainement");
      // On cherche dans le reperptoire si l'user à bien supprimer la tente_2 et tente_3
         if ( (chercher( repertoire_courant , "fichier_indesirable") == NULL ) )
         {
            return true ;
         }
         else
         {
            return false ;
         }

   break;
   case 8 :
   
      // Deplacement dans les repertoire a vérifier [Hangar et Zone_Entrainement]
         repertoire_courant = my_cd(racine , racine , "Hangar" ) ;
         repertoire_courant_1 = my_cd(racine , racine , "Zone_Entrainement" ) ;

      // On cherche dans le reperptoire si l'user à bien déplacer la caisse dans le Hangar vers la Zone_Entrainement
         if( chercher( repertoire_courant , "caisse") != NULL && chercher( repertoire_courant_1 , "caisse") != NULL )
         {
            return true ;
         }
         else
         {
            return false ;
         }

   break ;
   case 9 :
      // Deplacement dans les repertoire a vérifier [Hangar et Zone_Entrainement]
         repertoire_courant = my_cd(racine , racine , "Hangar" ) ;
         repertoire_courant_1 = my_cd(racine , racine , "Zone_Entrainement" ) ;

      // On cherche dans le reperptoire si l'user à bien déplacer la caisse dans le Hangar vers la Zone_Entrainement
         if( chercher( repertoire_courant , "caisse") == NULL && chercher( repertoire_courant_1 , "caisse") != NULL )
         {
            return true ;
         }
         else
         {
            return false ;
         }
   break ;
   case 141 :

         // Verification si l'user est dans le Coffre Fort
         if( strcmp(courant , "/AK-07/Zone_Entrainement/Coffre_Fort/") == 0 )
         {
            return true ;
         }
         else
         {
            return false ;
         }

   break;
   case 14 :

      // Deplacement dans les repertoire a vérifier [ /AK-07/Laboratoire/archives_secretes/ && /AK-07/Quartier_Généraux/bureau_du_général/ ]
         repertoire_courant = my_cd(racine , racine , "Laboratoire" ) ;
         repertoire_courant = chercher(repertoire_courant , "archives_secretes");
         repertoire_courant_1 = my_cd(racine , racine , "Quartier_Généraux" ) ;
         repertoire_courant_1 = my_cd(racine , repertoire_courant_1 , "bureau_du_général");

      // On cherche dans le reperptoire si le repertoire est bien protégé, le joueur n'est plus dans le l'archives_secretes et le fichier démandé est dans le bureau du géneral
         if( strcmp( repertoire_courant->perm , "---" ) == 0 && chercher( repertoire_courant_1 , "X4-77-DELTA" ) != NULL && strcmp(courant , "/AK-07/Laboratoire/archives_secretes/" ) != 0 )
         {
            return true ;
         }
         else
         {
            return false ;
         }
   default:
      break;
   }
}
//--------------------------------------------------------------------------------
/**
 * @fn mission_3
 * @brief se déplacer dans un dossier
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
*/
void mission_3(Mission *mission , fichier *racine ) // se déplacer dans un dossier
{
   bool reussite = false ;
   char *courant = calloc(MOT , sizeof(char)) ;

   strcpy(mission[3].titre , "MISSION-3 : CD") ;
   strcpy(mission[3].indice , "tu dois simplement entrer le dossier combats qui est dans la zone d'entrainement") ;
   mission[3].point = 1;

   affiche_fichier_tableau_curse_position_exacte("MISSION 3", "../data/mission_3.txt");    
   prompt_mission(racine , "ls" , "MISSION 3" , "../data/mission_3.txt" , courant , (mission+3) , 3 , 0);

   reussite = verification_mission( mission , 3 , racine , courant) ;

   verification_reussite( mission , racine , reussite , 3 );
}

//--------------------------------------------------------------------------------------------------------------

/**
 * @fn mission_4
 * @brief création des dossiers
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_4(Mission *mission , fichier *racine ) // création des dossiers
{
   bool reussite = false ;
   fichier *repertoire_courant = NULL;
   char *courant = calloc(MOT , sizeof(char)) ;

   strcpy(mission[4].titre , "MISSION-4 : mkdir") ;
   mission[4].point = 2;

   affiche_fichier_tableau_curse_position_exacte("MISSION 4", "../data/mission_4.txt");    
   prompt_mission(racine , "mkdir" , "MISSION 4" , "../data/mission_4.txt" , courant , (mission+4) , 4 , 0 );

   reussite = verification_mission( mission , 4 , racine , courant);

   if (reussite == true)
   {
      affiche_fichier_tableau_curse_position_exacte("MISSION ACCOMPLIE", "../data/felicitation_4.txt");
   }
   
   verification_reussite( mission , racine , reussite , 4 );

}

//--------------------------------------------------------------------------------------------------------------
/**
 * @fn mission_5
 * @brief suppression des dossiers
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_5(Mission *mission , fichier *racine )   // Supprimer des dossiers
{
   bool reussite = false ;
   fichier *repertoire_courant = NULL;
   int max_sec = 60 ;
   char *courant = calloc(MOT , sizeof(char)) ;
   // pipe pour recevoir le resultat 
      int fd[2];
      pipe(fd);
   int status;
   pid_t fini;

   strcpy(mission[5].titre , "MISSION-5 : rmdir") ;
   mission[5].point = 2;
   
   // On se deplace d'abord dans le repertoire "/AK-07/Caserne/dortoirs"
      repertoire_courant = my_cd( racine , racine , "Caserne");
      repertoire_courant = my_cd( racine , repertoire_courant , "dortoirs");
      // Creation du reperotoire tente_2 et tente_3
      my_mkdir(racine , repertoire_courant , "tente_2", 0);
      my_mkdir(racine , repertoire_courant , "tente_3", 0);
   
   screen_clear();

   // Affichage de la mission
      affiche_fichier_tableau_curse_position_exacte("MISSION 5", "../data/mission_5.txt");    
      affiche_fichier_tableau_curse_position_exacte("MISSION 5 - TEMPS LIMITE : 60 secondes", "../data/vide");


   // -------- FILS 1 : Chrono --------
      pid_t pid_chrono = fork();
      if(pid_chrono == 0)
      {
         for(int i = 0; i < max_sec*10; ++i)
         {
            usleep(100000);
         }

         exit(1); // code 1 = temps écoulé
      }

   // -------- FILS 2 : Mission --------
      pid_t pid_mission = fork();
      if(pid_mission == 0)
      {
         close(fd[0]); // fermer lecture

         // exécute prompt_mission dans un sous-fils
            prompt_mission(racine , "rmdir" , "MISSION 5" , "../data/mission_5.txt" , courant , (mission+5) , 5 , 0 );
            reussite = verification_mission( mission , 5 , racine , courant );
         
         // Envoie le resultat au père
            write( fd[1] , &reussite , sizeof(bool) );
            close(fd[1]); // fermer écriture

            exit(0);
      }

   // -------- PÈRE : supervise fils chrono et fils mission --------
   

      while(1)
      {
         // vérifie si le fils mission a terminé
         fini = waitpid(pid_mission, &status, WNOHANG);
         if(fini == pid_mission)
         {
            // tuer le chrono si la mission a fini
               kill(pid_chrono, SIGKILL);
               reussite = true ;
               break;
         }

         // vérifie si le chrono a fini
         fini = waitpid(pid_chrono, &status, WNOHANG);

         if(fini == pid_chrono)
         {
            // temps écoulé → tuer le fils mission
            kill(pid_mission, SIGKILL);
            affiche_fichier_tableau_curse_position_exacte("TEMPS ÉCOULÉ !" , "../data/vide");
            reussite = false ;
            break;
         }

         usleep(5000);
      }

   close(fd[1]); // fermer écriture
   // Récupération du resultat du fils mission
      read( fd[0] , &reussite , sizeof(bool) );
      close(fd[0]); // fermer lecture

   // nettoyage des fils
      waitpid(pid_chrono, NULL, 0);
      waitpid(pid_mission, NULL, 0);

   verification_reussite( mission , racine , reussite , 5 );
}

//--------------------------------------------------------------------------------------------------------------
/**
 * @fn mission_6
 * @brief création d'un fichier
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_6(Mission *mission , fichier *racine ) // cree un fichier pistolet
{
   bool reussite = false ;
   fichier *repertoire_courant = NULL ;
   char *courant = calloc(MOT , sizeof(char)) ;

   strcpy(mission[6].titre , "MISSION-6 : touch") ;
   mission[6].point = 2;
   affiche_fichier_tableau_curse_position_exacte("MISSION 6", "../data/mission_6.txt");    
   

   // lancement du prompt 
      prompt_mission(racine , "rm" , "MISSION 6" , "../data/mission_6.txt" , courant , (mission+6) , 6 , 0 );

   reussite = verification_mission( mission , 6 , racine , courant);
   verification_reussite( mission , racine , reussite , 6 );
}
//--------------------------------------------------------------------------------------------------------------
/**
 * @fn mission_7
 * @brief suppression d'un fichier
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_7(Mission *mission , fichier *racine )
{
   bool reussite = false ;
   fichier *repertoire_courant = NULL;
   char *courant;
   courant = calloc(MOT , sizeof(char)) ;

   // Preparation de la terrain de jeu
   repertoire_courant = my_cd( racine , racine , "Zone_Entrainement");
   my_touch( racine , repertoire_courant , "fichier_indesirable" ,0);
   
   strcpy(mission[7].titre , "MISSION-7 : rm") ;
   mission[7].point = 2;
   affiche_fichier_tableau_curse_position_exacte("MISSION 7", "../data/mission_7.txt");    

   screen_clear();
   // lancement du prompt 
      prompt_mission(racine , "rm" , "MISSION 7" , "../data/mission_7.txt" , courant , (mission+7) , 7 , 0 );

   reussite = verification_mission(mission , 7 , racine , courant) ;
   if( reussite == true )
   {   
         affiche_fichier_tableau_curse_position_exacte("MISSION ACCOMPLIE", "../data/felicitation_7.txt");
   }

   verification_reussite( mission , racine , reussite , 7 );
   free(courant);
}
//--------------------------------------------------------------------------------------------------------------
/**
 * @fn mission_8
 * @brief Copier avec cp
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_8(Mission *mission , fichier *racine ) // Copier avec cp 
{
   bool reussite = false ;
   fichier *repertoire_courant = NULL;
   char *courant = calloc(4096 , sizeof(char)) ;

   char tente_1[] = "Caserne/dortoirs/tente_1";
   char tente_2[] = "Caserne/dortoirs/tente_2";
   char tente_3[] = "Caserne/dortoirs/tente_3";

   strcpy(mission[8].titre , "MISSION-8 : cp") ;
   mission[8].point = 2;

   affiche_fichier_tableau_curse_position_exacte("MISSION 8", "../data/mission_8.txt");   
   
   // Déplacement dans le repertoire du partie 
      repertoire_courant = my_cd(racine , racine , "Hangar" ) ;
      my_touch(racine , repertoire_courant , "caisse" , 0 );
   
   screen_clear();

   // lancement du prompt 
      prompt_mission(racine , "cp" , "MISSION 8" , "../data/mission_8.txt" , courant , (mission+8) , 8 , 0);

   reussite = verification_mission(mission , 8 , racine , courant);
   free(courant);
   verification_reussite( mission , racine , reussite , 8 );
}

//--------------------------------------------------------------------------------------------------------------

/**
 * @fn mission_9
 * @brief Déplacer avec mv
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_9(Mission *mission , fichier *racine ) // Déplacer avec mv
{
   bool reussite = false ;
   fichier *repertoire_courant = NULL;
   char *courant;
   courant = calloc(MOT , sizeof(char)) ;

   strcpy(mission[9].titre , "MISSION-9 : echo") ;
   mission[9].point = 2;

   affiche_fichier_tableau_curse_position_exacte("MISSION 9", "../data/mission_9.txt");    
   
   // Déplacement dans le repertoire du partie 
      repertoire_courant = my_cd(racine , racine , "Hangar" ) ;
      my_touch(racine , repertoire_courant , "caisse" , 0 );

   screen_clear();
      // lancement du prompt 
   prompt_mission(racine , "mv" , "MISSION 9" , "../data/mission_9.txt" , courant , (mission+9) , 9 , 0 );

   reussite = verification_mission(mission , 9 , racine , courant );
   verification_reussite( mission , racine , reussite , 9 );

   free(courant);
}

//--------------------------------------------------------------------------------------------------------------
/**
 * @fn mission_10
 * @brief Afficher du texte avec echo
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_10(Mission *mission , fichier *racine )
{
   bool reussite = false ;
   fichier *repertoire_courant = NULL;
   char *courant;
   courant = calloc(MOT , sizeof(char)) ;

   strcpy(mission[10].titre , "MISSION-10 : echo") ;
   mission[10].point = 2;

   affiche_fichier_tableau_curse_position_exacte("MISSION 10", "../data/mission_10.txt");    
   

   // Deplacement dans la terrain de jeu
   repertoire_courant = my_cd( racine , racine , "Zone_Entrainement");

   // lancement du prompt 
   reussite = prompt_mission(repertoire_courant , "echo" , "MISSION 10" , "../data/mission_10.txt" , courant , (mission+10) , 10 , 1 );

   // On verfie s'il a au moins taper echo
   if ( reussite )
   {
      affiche_fichier_tableau_curse_position_exacte("MISSION ACCOMPLIE", "../data/felicitation_10.txt");
   }

   verification_reussite( mission , racine , reussite , 10 );

   free(courant);
}

/*****************************************************************************************/
/**
 * @fn mission_11
 * @brief Afficher le contenu d'un fichier avec cat
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_11(Mission *mission , fichier *racine)
{
   bool reussite ;
   fichier *repertoire_courant ;
   char *courant;

   reussite = false;
   courant = calloc( MOT , sizeof(char)) ;
   repertoire_courant = NULL;

   strcpy(mission[11].titre , "MISSION-11 : cat") ;
   mission[11].point = 3;

   affiche_fichier_tableau_curse_position_exacte("MISSION 11", "../data/mission_11.txt");    
   

   // Preparation : terrain de jeu
   repertoire_courant = my_cd( racine , racine , "Laboratoire");
   repertoire_courant = my_cd( racine , repertoire_courant , "archives_secretes");
   my_touch(racine , repertoire_courant , "convers.crpt" , 0);

   screen_clear();
   // lancement du prompt 
   reussite = prompt_mission(racine , "cat convers.crpt" , "MISSION 11" , "../data/mission_11.txt" , courant , (mission+11) , 11 , 1 );

   // On verfie s'il a fait le mission correspondant
   if ( reussite )
   {
      affiche_fichier_tableau_curse_position_exacte("MISSION ACCOMPLIE", "../data/felicitation_11.txt");
   }

   verification_reussite( mission , racine , reussite , 11 );

   free(courant);
}

/*****************************************************************************************/
/**
 * @fn mission_12
 * @brief Afficher le début d'un fichier avec head
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_12(Mission *mission , fichier *racine)
{
   bool reussite ;
   fichier *repertoire_courant ;
   char *courant;

   reussite = false ;
   courant = calloc(MOT , sizeof(char)) ;
   repertoire_courant = NULL ;

   strcpy(mission[12].titre , "MISSION-12 : head") ;
   mission[12].point = 3;

   affiche_fichier_tableau_curse_position_exacte("MISSION 12", "../data/mission_12.txt");    
   

   // Preparation : terrain de jeu
   repertoire_courant = my_cd( racine , racine , "Laboratoire");
   repertoire_courant = my_cd( racine , repertoire_courant , "archives_secretes");
   my_touch(racine , repertoire_courant , "convers.crpt" , 0);

   screen_clear();
   // lancement du prompt 
   reussite = prompt_mission(racine , "head -n 5 convers.crpt" , "MISSION 12" , "../data/mission_12.txt" , courant , (mission+12) , 12 , 1 );

   // On verfie s'il a fait le mission correspondant
   if ( reussite )
   {
      affiche_fichier_tableau_curse_position_exacte("MISSION ACCOMPLIE", "../data/felicitation_12.txt");
   }

   verification_reussite( mission , racine , reussite , 12 );

   free(courant);
}

/*****************************************************************************************/
/**
 * @fn mission_13
 * @brief Afficher la fin d'un fichier avec tail
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_13(Mission *mission , fichier *racine)
{
   bool reussite ;
   fichier *repertoire_courant ;
   char *courant;

   reussite = false;
   courant = calloc(MOT , sizeof(char)) ;
   repertoire_courant = NULL;

   strcpy(mission[13].titre , "MISSION-13 : tail") ;
   mission[13].point = 3;

   affiche_fichier_tableau_curse_position_exacte("MISSION 13", "../data/mission_13.txt");    
   

   // Preparation : terrain de jeu
      repertoire_courant = my_cd( racine , racine , "Laboratoire");
      repertoire_courant = my_cd( racine , repertoire_courant , "archives_secretes");
      my_touch(racine , repertoire_courant , "convers.crpt" , 0);

   screen_clear();

   // lancement du prompt 
      reussite = prompt_mission(racine , "tail -n 5 convers.crpt" , "MISSION 13" , "../data/mission_13.txt" , courant , (mission+13) , 13 , 1 );

   // On verfie s'il a fait le mission correspondant
   if ( reussite )
   {
      affiche_fichier_tableau_curse_position_exacte("MISSION ACCOMPLIE", "../data/felicitation_13.txt");
   }

   verification_reussite( mission , racine , reussite , 13 );

   free(courant);
}

/*****************************************************************************************/
void entrainement_14_1(Mission *mission , fichier *racine)
{
   fichier *repertoire_courant = NULL , *repertoire_courant1 = NULL ;
   char *courant = calloc(MOT , sizeof(char)) ;
   bool reuissite = false ;

   // Preparation du terrain d'entraînement
      repertoire_courant = my_cd( racine , racine , "Zone_Entrainement") ;
      my_mkdir(racine , repertoire_courant , "Coffre_Fort" , 1 ) ;
      repertoire_courant1 = my_cd( racine , repertoire_courant , "Coffre_Fort") ;
      my_touch(racine , repertoire_courant1 , "Gilet" , 0) ; 
      my_touch(racine , repertoire_courant1 , "Munitions" , 0) ;
      my_touch(racine , repertoire_courant1 , "AK-47" , 0) ;
      strcpy(repertoire_courant1->perm , "---") ;
   
   // Lancement du jeu
      affiche_fichier_tableau_curse_position_exacte("ENTRAÎNEMENT", "../data/test_14.txt");
      prompt_mission( repertoire_courant , "chmod" , "MISSION 14" , "../data/test_14.txt" , courant , (mission+14) , 141 , 0 ) ;

   // Vérification du statuts mission   
      reuissite = verification_mission(mission , 141 , racine , courant );
      if(reuissite)
      {
         affiche_fichier_tableau_curse_position_exacte("Entraînement réussi", "../data/felicitation_14_1.txt");
      }

      free(courant);
}

void prepa_14(fichier * racine)
{
   fichier *repertoire_courant = NULL ;

   // Preparation : terrain de jeu
      repertoire_courant = my_cd( racine , racine , "Laboratoire") ;
      repertoire_courant = my_cd( racine , repertoire_courant , "archives_secretes") ;
         my_touch(racine , repertoire_courant , "R+8" , 0) ; 
         my_touch(racine , repertoire_courant , ".Satan_2" , 0) ; 
         my_touch(racine , repertoire_courant , "enigma" , 0) ; 
         my_touch(racine , repertoire_courant , "sigma" , 0) ; 
         my_touch(racine , repertoire_courant , "Q-2" , 0) ; 
         my_touch(racine , repertoire_courant , "Section_14" , 0) ;
         my_touch(racine , repertoire_courant , ".Manhattan" , 0) ;
         my_touch(racine , repertoire_courant , "Blame" , 0) ;
         my_touch(racine , repertoire_courant , ".Abdul" , 0) ; 
         my_touch(racine , repertoire_courant , "Falcon" , 0) ;
         my_touch(racine , repertoire_courant , "Eclipse" , 0) ;
         my_touch(racine , repertoire_courant , "Shadow" , 0) ;
         my_touch(racine , repertoire_courant , ".Vortex" , 0) ;
         my_touch(racine , repertoire_courant , ".Ironclad" , 0) ;
         my_touch(racine , repertoire_courant , "Phantom" , 0) ;
         my_touch(racine , repertoire_courant , "Aurora" , 0) ;
         my_touch(racine , repertoire_courant , ".Titan" , 0) ;
      strcpy( repertoire_courant->perm , "---" ) ;
}
int demande_avis()
{
   int choix ;
   char texte[]={"Souhaites-tu commencer par l'entraînement ou pas soldat ??[Y/N] :"} ;
      
      screen_clear();

      initscr();              // initialise ncurses
      clear();
      noecho();               // ne pas afficher les touches tapées
      cbreak();               // entrée sans mise en tampon
      curs_set(0);            // cache le curseur

      
         mvprintw( LINES /2 , (COLS -strlen(texte)) /2 , "%s" , texte ) ;
         
         do
         {
            choix = getch();
         } while (choix != 'Y' && choix != 'y' && choix != 'N' && choix && 'n') ;

         if (choix == 'Y' || choix == 'y' )
         {
            endwin(); // restaure le terminal
            clear();      
            return 1 ;
         }
         else if (choix == 'N' || choix == 'n' )
         {
            endwin(); // restaure le terminal
            clear();
            return 0 ;
         }
}
/**
 * @fn mission_14
 * @brief Changer les permissions d'un fichier ou d'un dossier avec chmod
 * @param mission : Structur mission
 * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
 */
void mission_14(Mission *mission , fichier *racine) // Chmod
{
   bool reussite = false ;
   char *courant ;
   int max_sec = 90 ;

   courant = calloc(MOT , sizeof(char)) ;

   strcpy(mission[14].titre , "MISSION-14 : chmod");
   mission[14].point = 3;
   // pipe pour recevoir le resultat 
      int fd[2];
      pipe(fd);


   // Demande si le joueur veut recommencer au cours au passer juste au mission
      if( mission[13].terminé == 1 )
      {
            if(demande_avis() == 1 ) // Si l'user veux de tout recommencer
            {
               // Lancement du première entrainement
                  affiche_fichier_tableau_curse_position_exacte("Chmod", "../data/cours_14.txt");
                  entrainement_14_1(mission , racine ) ;

               // Preparation : terrain de jeu
                  prepa_14(racine);
                  screen_clear() ;

               // lancement du vraie mission 14 . 
                  affiche_fichier_tableau_curse_position_exacte("Chmod", "../data/mission_14.txt");
                  affiche_fichier_tableau_curse_position_exacte("MISSION 14 - TEMPS LIMITE : 90 secondes", "../data/vide");
            
            // -------- FILS 1 : Chrono --------
            pid_t pid_chrono = fork();
            if(pid_chrono == 0)
            {
               for(int i = 0; i < max_sec*10; ++i)
               {
                  usleep(100000);
               }

               exit(1); // code 1 = temps écoulé
            }

            // -------- FILS 2 : Mission --------
            pid_t pid_mission = fork();
            if(pid_mission == 0)
            {
               // fermeture du pide lecture
                  close(fd[0]);
               
                  // exécute prompt_mission dans un sous-fils
                  prompt_mission(racine , "chmod" , "MISSION 14" , "../data/mission_14.txt" , courant , (mission+14) , 14 , 0 );
                  reussite = verification_mission(mission , 14 , racine , courant) ;

               // Envoie le resultat au père
                  write( fd[1] , &reussite , sizeof(bool) );
                  close( fd[1] ) ;
                  exit(0) ;
            }

            // -------- PÈRE : supervise fils chrono et fils mission --------
            int status;
            pid_t fini;

            while(1)
            {
               // vérifie si le fils mission a terminé
               fini = waitpid(pid_mission, &status, WNOHANG);
               if(fini == pid_mission)
               {
                  // tuer le chrono si la mission a fini
                     kill(pid_chrono, SIGKILL);
                     break;
               }

               // vérifie si le chrono a fini
               fini = waitpid(pid_chrono, &status, WNOHANG);
               if(fini == pid_chrono)
               {
                  // temps écoulé → tuer le fils mission
                  kill(pid_mission, SIGKILL);
                  affiche_fichier_tableau_curse_position_exacte("TEMPS ÉCOULÉ !" , "../data/vide");
                  reussite = false ;
                  break;
               }

               usleep(5000);
            }

            // nettoyage des fils
            waitpid(pid_chrono, NULL, 0);
            waitpid(pid_mission, NULL, 0);
            }
            else
            {
               // Preparation : terrain de jeu
                  prepa_14(racine);
                  screen_clear() ;

               // lancement du vraie mission 14 . 
                  affiche_fichier_tableau_curse_position_exacte("Chmod", "../data/mission_14.txt");
                  prompt_mission(racine , "chmod" , "MISSION 14" , "../data/mission_14.txt" , courant , (mission+14) , 14 , 0 );

            }
      }
      else // Si la mission n'etait pas encore términé avant 
      {
         // Lancement du première entrainement
            affiche_fichier_tableau_curse_position_exacte("Chmod", "../data/cours_14.txt");
         //   entrainement_14_1(mission , racine ) ;

         // Preparation : terrain de jeu
            prepa_14(racine);
            screen_clear() ;

         // lancement du vraie mission 14 . 
            affiche_fichier_tableau_curse_position_exacte("MISSION 14", "../data/mission_14.txt");
            affiche_fichier_tableau_curse_position_exacte("MISSION 14 - TEMPS LIMITE : 90 secondes" , "../data/vide");


            pid_t pid_chrono = fork();
            if(pid_chrono == 0)
            {
               for(int i = 0; i < max_sec*10; ++i)
               {
                  usleep(100000);
               }

               exit(1); // code 1 = temps écoulé
            }

            // -------- FILS 2 : Mission --------
            pid_t pid_mission = fork();
            if(pid_mission == 0)
            {
               // fermeture du pide lecture
                  close(fd[0]);

               // exécute prompt_mission dans un sous-fils
                  prompt_mission(racine , "chmod" , "MISSION 14" , "../data/mission_14.txt" , courant , (mission+14) , 14 , 0 );
                  reussite = verification_mission(mission , 14 , racine , courant) ;

               // Envoie le resultat au père
                  write( fd[1] , &reussite , sizeof(bool) );
                  close(fd[1]) ;
                  exit(0) ;
            }

            // -------- PÈRE : supervise fils chrono et fils mission --------
            int status;
            pid_t fini;

            while(1)
            {
               // vérifie si le fils mission a terminé
               fini = waitpid(pid_mission, &status, WNOHANG);
               if(fini == pid_mission)
               {
                  // tuer le chrono si la mission a fini
                     kill(pid_chrono, SIGKILL);
                     break;
               }

               // vérifie si le chrono a fini
               fini = waitpid(pid_chrono, &status, WNOHANG);
               if(fini == pid_chrono)
               {
                  // temps écoulé → tuer le fils mission
                  kill(pid_mission, SIGKILL);
                  affiche_fichier_tableau_curse_position_exacte("TEMPS ÉCOULÉ !" , "../data/vide");
                  reussite = false ;
                  break;
               }

               usleep(5000);
            }

            // nettoyage des fils
               waitpid(pid_chrono, NULL, 0);
               waitpid(pid_mission, NULL, 0);
      }
   // Reception du resultat du fils mission
      close(fd[1]) ; // fermer écriture
      read( fd[0] , &reussite , sizeof(bool) );
      close(fd[0]) ;

   // Vérification de la réussite de la mission   
      verification_reussite( mission , racine , reussite , 14 );

   free(courant);
} 
