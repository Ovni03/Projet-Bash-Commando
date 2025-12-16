/**
 *  @file   prompt.c
 *  @brief  ceci fait reference à la simulation de prompt
 *  @author Valisoa
 *  @brief
 *      Il est regroupe les  fonctions utiles  pour notre petite terminale ;
 *          + affiche le repertoire courant ; le nom d'utilisateur ; la machine
 *          + execute le commande decrit
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
/**
 *  @include    #include <ncurses.h>
 *  @brief  Quelques fonctions de Olivier utilise cette bibliotheques
 */
#include <ncurses.h>
/**
 *  @include    #include "../include/gestion_fichier.h"
 *  @brief  pour la gestion des fichiers ( adaptation des mes besoins )
 */
#include "../include/gestion_fichier.h"

/**
 *  @include    #include "../include/prompt.h"
 *  @brief  inclusion de tous les fonctions de ce fichier [ certains appelle les autres ]
 */
#include "../include/prompt.h"
#include "../include/affichage.h"

void affiche_prompt(fichier *repertoire_actuel)
{
    int point_actu;
    char *nom_joueur, *chemin_repertoire_actuel;

    nom_joueur = malloc(MOT * sizeof(char));
    chemin_repertoire_actuel = calloc(MOT , sizeof(char));

    point_actu = get_point();
    get_pwd( repertoire_actuel , chemin_repertoire_actuel );

    getNomJoueur(nom_joueur);
    printf("[%d]\033[1;32m%s\033[0m\033[1;34m@\033[0m\033[1;31mBash-Commando:\033[0m :%s $ ", point_actu , nom_joueur, chemin_repertoire_actuel);

    free(nom_joueur);
    free(chemin_repertoire_actuel);
}
void efface_cara(char *chaine, int *position)
{
    int point_actu, longueur, i;
    
    point_actu = *position;

    if ((*position) > 0)
    {
        // Reculer pour trouver le début du caractère UTF-8
        (*position)--;
        while (((*position) > 0) && ((chaine[(*position)] & 0xC0) == 0x80))
        {
            (*position)--;
        }

        longueur = point_actu - (*position);
        
        // Décaler les caractères dans la chaîne
        point_actu = strlen(chaine) - longueur;
        for (i = (*position); i < point_actu; i++)
        {
            chaine[i] = chaine[(i + longueur)];
        }
        chaine[point_actu] = '\0';
    }
}
int my_get_ligne(char *mobile, int taille_Max, fichier *repertoire_actuel)
{
    struct termios ancien, nouveau;
    char *chaine, *tmp ;
    char caractere;
    int i, position, j, test, droite, tabulation;

    // Initialisation
    mobile[0] = '\0';
    
    // Configuration du terminal en mode non canonique
    tcgetattr(STDIN_FILENO, &ancien);
    nouveau = ancien;
    nouveau.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &nouveau);

    
    // Initialisation des variables
    i = 0;
    position = -1;  // position dans l'historique (-1 = pas dans l'historique)
    tmp = calloc(LIGNE, sizeof(char));
    droite = 0;     // décalage du curseur vers la droite
    tabulation = 0; // compteur pour les tabulations successives
    
    while (true)
    {
        caractere = getchar();
        
        // Touche Entrée
        if (caractere == '\n' || caractere == '\r')
        {
            // Réinitialiser le compteur de tabulation à la fin de la ligne
            tabulation = 0;
            break;
        }
        
        // Vérifier la taille maximale
        if (i >= taille_Max - 1)
        {
            break;
        }

        // Touche Backspace/Suppr
        if(caractere == 127 || caractere == 8)
        {
            touche_suppr(&i , &droite , mobile);
            continue;
        }
        
        // Séquence escape (touches fléchées)
        if (caractere == 27)
        {
            // Lire le caractère suivant avec vérification
            caractere = getchar();
            if (caractere == '[')
            {
                caractere = getchar();
                switch (caractere)
                {
                    case 'A': // Flèche haut (historique précédent)
                    {
                        touches_haut_bas(&i , &position , &droite ,  mobile , tmp , 1);
                        // Réinitialiser le compteur de tabulation
                        tabulation = 0;
                        break;
                    }
                    case 'B': // Flèche bas (historique suivant)
                    {
                        touches_haut_bas(&i , &position , &droite ,  mobile , tmp , 0);
                        // Réinitialiser le compteur de tabulation
                        tabulation = 0;
                        break;
                    }
                    case 'C': // Flèche droite
                    {
                        touches_gauche_droite(&i , &droite , mobile , 0);
                        break;
                    }
                    case 'D': // Flèche gauche
                    {
                        touches_gauche_droite(&i , &droite , mobile , 1);
                        break;
                    }
                    default:
                    {
                        continue;
                    }
                }
            }
            else
            {
                // Si ce n'est pas une flèche, ignorer la séquence
                continue;
            }
        }
        else if ( caractere == '\t' ) // Tabulation
        {
            vide_ligne(droite , mobile);
            auto_completion(mobile, repertoire_actuel, &tabulation , &droite);
            i = strlen(mobile);
            for(test=0 ; test<droite ; test++)
            {
                i--;
                while ( (i > 0) && ((mobile[i] & 0xC0) == 0x80) )
                {
                    i--;
                }
                printf("\033[1D");
            }        
        }
        // Caractères imprimables
        else if (isprint(caractere))
        {
            // réinitialiser tabulation quand on tape autre chose qu'un espace
            if (caractere != ' ')
            {
                tabulation = 0;
            }
            // insertion de caractere et affchage du ligne
            affiche_commande(&i , caractere , mobile , droite);
        }
        test = verifier(mobile);
        if(test == 1)
        {
            printf("\n--> Arretez , ça me degoute !\n");
            i = 0;
            mobile[i] = '\0';
            affiche_prompt(repertoire_actuel);
        }
        else if(test == 2)
        {
            strcpy(mobile , "exit");
            break;
        }
        if (caractere == ' ')
        {
            tabulation = 0;
        }
    }
    printf("\n");

    // Restaurer les paramètres du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &ancien);

    free(tmp);
    return i;
}
int verifier(char *chaine)
{
    int i , longueur;

    longueur = strlen(chaine);
    if(longueur >= 10)
    {
        i = 0;
        while((i < (strlen(chaine) - 2) )&& (chaine[i] == chaine[i+1]))
        {
            i++;
        }
        if(i >= 10)
        {
            return (1);
        }
    }

    i = trouver_avant(chaine , "exit ");
    longueur = strlen("exit ");
    if( i == longueur )
    {
        return 2;
    }

    return (0);
}
void affiche_commande(int *indice , char caractere ,char *chaine ,int  droite)
{
    int i;
    
    vide_ligne(droite , chaine);
    inserer(caractere, (*indice), chaine);
    printf("%s" , chaine);
    (*indice) = strlen(chaine);
    for(i=0 ; i<droite ; i++)
    {
        (*indice)--;
        while ( ((*indice) > 0) && ((chaine[(*indice)] & 0xC0) == 0x80) )
        {
            (*indice)--;
        }
        printf("\033[1D");
    }
}
void touches_gauche_droite(int *indice , int *pos_droite , char *chaine , int gauche)
{
    switch(gauche)
    {
        case 0: // Flèche droite
        {
            if ((*indice) < strlen(chaine))
            {
                if ((*pos_droite) > 0)
                {
                    (*pos_droite)--;
                    printf("\033[1C");
                    (*indice)++;
                    while ( ( (*indice) < strlen(chaine) ) && ( (chaine[(*indice)] & 0xC0) == 0x80 ) )
                    {
                        (*indice)++;
                    }
                }
            }
            else
            {
                printf("\a"); // bip sonore
            }
            break;
        }
        case 1: // Flèche gauche
        {
            if ((*indice) > 0)
            {
                (*pos_droite)++;
                printf("\033[1D");
                (*indice)--;
                while (((*indice) > 0) && ((chaine[(*indice)] & 0xC0) == 0x80))
                {
                    (*indice)--;
                }
            }
            else
            {
                printf("\a");
            }
            break;
        }
    }
}
void touches_haut_bas(int *indice , int *position , int *droite , char *chaine , char *tmp , int haut)
{
    int test;
    char *chemin , *nom;

    // Préparation du chemin pour l'historique
    test = 23;
    nom = getenv("USER");
    if (nom != NULL)
    {
        test += strlen(nom);
        chemin = calloc(test , sizeof(char));
        sprintf(chemin, "../save/%s/histoire.txt", nom);
    }

    switch(haut)
    {
        case 1 :
        {
            if ((*position) == -1)
            {
                strcpy(tmp, chaine);
            }
            vide_ligne((*droite) , chaine);
            chaine[0] = '\0';
            
            (*position)++;
            test = affiche_ligne(chemin, (-(*position)), chaine);
            
            if (test != -1)
            {
                (*indice) = strlen(chaine);
                // Remettre à zéro le décalage du curseur quand on change de ligne
                (*droite) = 0;
                if (test == 2)  // limite atteinte
                {
                    (*position)--;
                }
            }
            break ;
        }
        case 0 :
        {
            if ((*position) == 0)
            {
                vide_ligne((*droite) , chaine);
                (*position) = -1;
                strcpy(chaine, tmp);
                printf("%s", chaine);
                (*indice) = strlen(chaine);
                (*droite) = 0;
            }
            else if ((*position) >= 1)
            {
                vide_ligne((*droite) , chaine);
                chaine[0] = '\0';
                (*position)--;
                test = affiche_ligne(chemin, (-(*position)), chaine);
                if (test != -1)
                {
                    (*indice) = strlen(chaine);
                    (*droite) = 0;
                }
            }
            break;
        }
    }
    if(nom != NULL)
    {
        free(chemin);
    }
}
void touche_suppr(int *position , int *pos_droite , char *chaine)
{
    int i;
    if ((*position) > 0)
    {
        vide_ligne((*pos_droite), chaine);
        efface_cara(chaine , position);
        printf("%s" , chaine);
        (*position) = strlen(chaine);
        for(i=0 ; i<(*pos_droite) ; i++)
        {
            (*position)--;
            while ( ((*position) > 0) && ((chaine[(*position)] & 0xC0) == 0x80) )
            {
                (*position)--;
            }
            printf("\033[1D");
        }
    }
}
void vide_ligne(int droite , char *chaine)
{
    int position;

    for(position=0 ; position<droite ; position++)
    {
        printf("\033[1C");
    }
    position = taille_de(chaine);
    while(position != 0)
    {
        printf("\b \b");
        position--;
        fflush(stdout);
    }
}
int taille_de(char *ligne)
{
    int i , longueur;

    longueur = 0;
    i = 0;
    if(ligne!= NULL)
    {
        while(ligne[i] != '\0')
        {
            if((ligne[i] & 0xC0) == 0xC0) // Lohana Accent sy ny tariny , utf-8 reny
            {
                longueur++;
            }
            else if((ligne[i] & 0x80 ) == 0) // rehefa ASCII
            {
                longueur++;
            }
            i++;
        }
    }

    return (longueur);
}
int auto_completion(char *chaine, fichier *repertoire_actuel, int *nb_Tab , int *pos_droite)
{
    int i, longueur , j , test , compteur_fichiers , nbMot , nb_commande , pos_curseur;
    fichier *fichier_pass , *racine , *tmp;
    char *brouillon  , *tempo , *chemin , *reste;

    // Liste des commandes disponibles
    char *liste_commandes[] = {
        "ls", "cd", "pwd", "cat", "echo", "mkdir", "rm", "cp", "mv",
        "rmdir", "tail", "chmod", "exit", "clear", "aide", "histoire",
        "liste_commande", "major", "mission", "head" , "touch", "favori", 
         "demos" , "nombre_ouverture" , NULL 
    };
    nb_commande = 23;

    fichier_pass = repertoire_actuel;
    racine = repertoire_actuel;
    // Recherche de fichier *racine
    while( racine->parent != NULL)
    {
        racine = racine->parent;
    }
    i = strlen(chaine);
    for(j=0 ; j<(*pos_droite) ; j++)
    {
        i--;
        while ( (i > 0) && ((chaine[i] & 0xC0) == 0x80) )
        {
            i--;
        }
    }
    reste = calloc(LIGNE , sizeof(char));
    test = i;
    j = 0;
    do
    {
        reste[j] = chaine[i];
        i++;
        j++;
        if(i > strlen(chaine))
        {
            break;
        }
    }
    while(chaine[i] != '\0');
    chaine[test] = '\0';
    longueur = strlen(chaine);
    nbMot = compter_mot(chaine);
    i = 0;
    j = 0;
    if (longueur == 0 || nbMot == 0)
    {
        strcat(chaine, "aide");
    }
    else
    {
        // compter le nombre de mot
        pos_curseur = nbMot;
        brouillon = calloc(LIGNE , sizeof(char));
        strcpy(brouillon , "aide");
        // trouver le nombre de caractere de brouillon au debut de chaine
        i = trouver_avant(chaine , brouillon);
        
        // appuyer tab avec aide aide suivie d'espacement 
        if( ( nbMot == 1 ) && ( i == 4 ) )
        {
            strcat(chaine , " aide");
        }
        else if( (nbMot == 2) && (i == 4)  )
        {
            longueur = strlen(chaine);
            if(longueur > 0)
            {
                if(chaine[longueur-1] != ' ')
                {
                    prend_mot_dans(2 , chaine , brouillon);
                    compteur_fichiers = 0;
                    tempo = calloc(MOT , sizeof(char));
                    for(i=0 ; i<nb_commande ; i++)
                    {
                        j = trouver_avant(liste_commandes[i] , brouillon);
                        if(j == strlen(brouillon))
                        {
                            compteur_fichiers++;
                            inserer_mot_dans(1 , tempo , liste_commandes[i]);
                        }
                    }
                    if(compteur_fichiers == 1)
                    {
                        inserer_mot_dans(2 , chaine , tempo);
                    }
                    else if( ((*nb_Tab) != 0 ) && (compteur_fichiers>0) )
                    {
                        printf("%s\n" , chaine);
                        for(i=0 ; i<nb_commande ; i++)
                        {
                            j = trouver_avant(liste_commandes[i] , brouillon);
                            if(j == strlen(brouillon))
                            {
                                compteur_fichiers++;
                                printf("%s\t" , liste_commandes[i]);
                            }
                        }
                        printf("\n");
                        affiche_prompt(repertoire_actuel);
                        
                    }
                    free(tempo);
                }
                else if((*nb_Tab) != 0)
                {
                    (*nb_Tab) = (*nb_Tab) % nb_commande; // Boucler si trop de tabulations
                    inserer_mot_dans(2 , chaine , liste_commandes[(*nb_Tab)]);
                }
            }
        }
        else if(nbMot == 1) 
        {
            longueur = strlen(chaine);
            if(longueur > 0)
            {
                if(chaine[longueur-1] != ' ')
                {
                    prend_mot_dans(1 , chaine , brouillon);
                    compteur_fichiers = 0;
                    tempo = calloc(MOT , sizeof(char));
                    for(i=0 ; i<nb_commande ; i++)
                    {
                        j = trouver_avant(liste_commandes[i] , brouillon);
                        if(j == strlen(brouillon))
                        {
                            compteur_fichiers++;
                            inserer_mot_dans(1 , tempo , liste_commandes[i]);
                        }
                    }
                    if(compteur_fichiers == 1)
                    {
                        inserer_mot_dans(1 , chaine , tempo);
                    }
                    else if( ((*nb_Tab) != 0 ) && (compteur_fichiers>0) )
                    {
                        printf("%s\n" , chaine);
                        for(i=0 ; i<nb_commande ; i++)
                        {
                            j = trouver_avant(liste_commandes[i] , brouillon);
                            if(j == strlen(brouillon))
                            {
                                compteur_fichiers++;
                                printf("%s\t" , liste_commandes[i]);
                            }
                        }
                        printf("\n");
                        affiche_prompt(repertoire_actuel);
                        
                    }
                    free(tempo);
                }
                else
                {
                    if((*nb_Tab) != 0)
                    {
                        printf("%s\n" , chaine);
                        tempo = calloc(2 , sizeof(char));
                        tempo[0] = '\0';
                        my_ls(racine , fichier_pass , tempo );
                        affiche_prompt(fichier_pass);
                        free(tempo);
                    }
                }
            }
        }
        else
        {
            longueur = strlen(chaine);
            if(chaine[longueur-1] != ' ')
            {
                prend_mot_dans(pos_curseur , chaine , brouillon);
                if(chaine[longueur-1] != '/')
                {   
                    tempo = strchr(brouillon , '/');
                    if(tempo == NULL)
                    {
                        // Compter les fichiers disponibles
                        compteur_fichiers = 0;
                        fichier_pass = fichier_pass->premierfils;
                        while (fichier_pass != NULL)
                        {
                            compteur_fichiers++;
                            fichier_pass = fichier_pass->frereSuivant;
                        }
                        
                        if (compteur_fichiers > 0)
                        {
                            compteur_fichiers = 0;
                            fichier_pass = repertoire_actuel->premierfils;
                            while(fichier_pass != NULL)
                            {
                                j = trouver_avant(fichier_pass->nom , brouillon);
                                if(j == strlen(brouillon))
                                {
                                    compteur_fichiers++;
                                    tmp = fichier_pass;
                                }
                                fichier_pass = fichier_pass->frereSuivant;
                            }
                            if(compteur_fichiers == 1)
                            {
                                longueur = strlen(tmp->nom) + 1;
                                tempo = calloc( longueur , sizeof(char));
                                strcat(tempo , tmp->nom);
                                if(tmp->estDossier == 1)
                                {
                                    strcat(tempo , "/");
                                }
                                inserer_mot_dans(pos_curseur , chaine , tempo);
                                free(tempo);
                            }
                            else if( ((*nb_Tab) != 0 ) && (compteur_fichiers>0) )
                            {
                                printf("%s\n" , chaine);
                                fichier_pass = repertoire_actuel->premierfils;
                                while(fichier_pass != NULL)
                                {
                                    j = trouver_avant(fichier_pass->nom , brouillon);
                                    if(j == strlen(brouillon))
                                    {
                                        if (fichier_pass->estDossier)
                                        {
                                            printf("\033[1;36m%s\033[0m  " , fichier_pass->nom);
                                        }
                                        else
                                        {
                                            printf("\033[1;37m%s\033[0m  " , fichier_pass->nom);
                                        }
                                        printf("\t");
                                    }
                                    fichier_pass = fichier_pass->frereSuivant;
                                }
                                printf("\n");
                                affiche_prompt(repertoire_actuel);
                            }
                        }
                    }
                    else
                    {
                        i = strlen(brouillon);
                        tempo = calloc(LIGNE , sizeof(char));
                        if(i > 0)
                        {
                            while(brouillon[i] != '/')
                            {
                                i--;
                            }
                            longueur = i;
                            j = i + 1;
                            i = 0;
                            while(brouillon[j] != '\0')
                            {
                                tempo[i] = brouillon[j];
                                i++;
                                j++;
                            }
                            tempo[i] = '\0';
                            brouillon[(longueur)] = '\0';
                            
                        }
                        chemin = calloc(strlen(brouillon) , sizeof(char));
                        inserer_mot_dans(1 , chemin , brouillon);
                        tmp = my_cd(racine , repertoire_actuel , chemin);
                        free(chemin);
                        if(tmp == NULL)
                        {
                            affiche_prompt(repertoire_actuel);
                        }
                        else
                        {
                            strcat(brouillon , "/");
                            // Compter les fichiers disponibles
                            compteur_fichiers = 0;
                            fichier_pass = tmp->premierfils;
                            while (fichier_pass != NULL)
                            {
                                compteur_fichiers++;
                                fichier_pass = fichier_pass->frereSuivant;
                            }
                            
                            if (compteur_fichiers > 0)
                            {
                                compteur_fichiers = 0;
                                fichier_pass = tmp->premierfils;
                                racine = tmp;
                                while(fichier_pass != NULL)
                                {
                                    j = trouver_avant(fichier_pass->nom , tempo);
                                    if(j == strlen(tempo))
                                    {
                                        compteur_fichiers++;
                                        tmp = fichier_pass;
                                    }
                                    fichier_pass = fichier_pass->frereSuivant;
                                }
                                if(compteur_fichiers == 1)
                                {
                                    strcat(brouillon , tmp->nom);

                                    if(tmp->estDossier == 1)
                                    {
                                        strcat(brouillon , "/");
                                    }
                                    inserer_mot_dans(pos_curseur , chaine , brouillon);
                                }
                                else if( ((*nb_Tab) != 0 ) && (compteur_fichiers>0) )
                                {
                                    printf("%s\n" , chaine);
                                    fichier_pass = racine->premierfils;
                                    while(fichier_pass != NULL)
                                    {
                                        j = trouver_avant(fichier_pass->nom , tempo);
                                        if(j == strlen(tempo))
                                        {
                                            if (fichier_pass->estDossier)
                                            {
                                                printf("\033[1;36m%s\033[0m  " , fichier_pass->nom);
                                            }
                                            else
                                            {
                                                printf("\033[1;37m%s\033[0m  " , fichier_pass->nom);
                                            }
                                            printf("\t");
                                        }
                                        fichier_pass = fichier_pass->frereSuivant;
                                    }
                                    printf("\n");
                                    affiche_prompt(repertoire_actuel);
                                }
                            }
                                                        
                        }
                        free(tempo);
                    }
                }
                else
                {
                    prend_mot_dans(pos_curseur , chaine , brouillon);
                    brouillon[(strlen(brouillon) - 1 )] = '\0';
                    chemin = calloc(strlen(brouillon) , sizeof(char));
                    inserer_mot_dans(1 , chemin , brouillon);
                    tmp = my_cd(racine , repertoire_actuel , chemin);
                    free(chemin);

                    if(tmp == NULL)
                    {
                        affiche_prompt(repertoire_actuel);
                    }
                    else
                    {
                        // Compter les fichiers disponibles
                        compteur_fichiers = 0;
                        fichier_pass = tmp->premierfils;
                        while (fichier_pass != NULL)
                        {
                            compteur_fichiers++;
                            fichier_pass = fichier_pass->frereSuivant;
                        }
                        if (compteur_fichiers > 0)
                        {
                            if(compteur_fichiers == 1)
                            {
                                strcat(brouillon , "/");
                                strcat(brouillon , tmp->premierfils->nom);
                                inserer_mot_dans(pos_curseur ,  chaine , brouillon);
                            }
                            else if((*nb_Tab) != 0 ) 
                            {
                                printf("%s\n" , chaine);
                                fichier_pass = tmp->premierfils;
                                while(fichier_pass != NULL)
                                {
                                    if (fichier_pass->estDossier)
                                    {
                                        printf("\033[1;36m%s\033[0m  " , fichier_pass->nom);
                                    }
                                    else
                                    {
                                        printf("\033[1;37m%s\033[0m  " , fichier_pass->nom);
                                    }
                                    printf("\t");
                                    
                                    fichier_pass = fichier_pass->frereSuivant;
                                }
                                printf("\n");
                                affiche_prompt(repertoire_actuel);
                            }
                        }
                        
                    }
                }

            }
        }
        free(brouillon);
    }
    
    (*nb_Tab)++;
    strcat(chaine , reste);
    free(reste);
    longueur = taille_de(chaine);
    printf("%s" , chaine);
    i = strlen(chaine);
   
    return longueur;
}
void inserer_mot_dans(int position , char *chaine , char *insertion)
{
    int i , j , longueur , test;
    char *tmp;

    test = compter_mot(chaine);
    if(test == 0)
    {
        position = 1;
    }
    else if(test < position)
    {
        position = test + 1;
    }
    j = 0;
    if(position == 1)
    {
        while(chaine[j] == ' ')
        {
            j++;
        }
    }    
    // se deplacer au debut du {postition} ieme mot 
    for(i=0 ; i<(position-1) ; i++)
    {
        while(chaine[j] == ' ')
        {
            j++;
        }
        while(chaine[j] != ' ')
        {
            j++;
        }
        while(chaine[j] == ' ')
        {
            j++;
        }
    }
    i = j;
    while((chaine[j] != ' ') && (chaine[j] != '\0'))
    {
        j++;
    }

    test = strlen(chaine) - j + 1;
    tmp = calloc(test , sizeof(char));

    test = strlen(chaine);
    longueur = i;
    i = 0;
    while(j != test)
    {
        tmp[i] = chaine[j];
        i++;
        j++;
    }
    tmp[i] = '\0';
    chaine[longueur] = '\0';
    if(longueur > 0)
    {
        if(chaine[longueur-1] != ' ')
        {
            strcat(chaine , " ");
        }
    }

    strcat(chaine , insertion);
    strcat(chaine , tmp);

    free(tmp);
}
int trouver_avant(char *chaine , char *recherche)
{
    int i , j;

    i = 0;
    j = 0;
    while(chaine[j] == ' ')
    {
        j++;
    }

    while( ( recherche[i] == chaine[j] ) && ( i < strlen(recherche) ) && ( j < strlen(chaine) ) )
    {
        i++;
        j++;
    }

    return (i);
}
int compter_mot(char *chaine)
{
    int i , nbMot;

    i = 0;
    nbMot = 0;
    while(chaine[i] == ' ')
    {
        i++;
    }
    while( chaine[i] != '\0' )
    {
        if(chaine[i] == ' ')
        {
            nbMot++;
            while(chaine[i] == ' ')
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }
    if(chaine[i-1] != ' ')
    {
        nbMot++;
    }

    return (nbMot);
}
int prend_mot_dans(int position , char *chaine , char *emplacement)
{
    int i , j;

    j = 0;
    if(position == 1)
    {
        while(chaine[j] == ' ')
        {
            j++;
        }
    }
    // se deplacer au debut du {postition} ieme mot 
    for(i=0 ; i<(position-1) ; i++)
    {
        while(chaine[j] == ' ')
        {
            j++;
        }
        while(chaine[j] != ' ')
        {
            j++;
        }
        while(chaine[j] == ' ')
        {
            j++;
        }
    }
    
    //vider brouillon
    i = 0;
    while(strlen(emplacement) != 0)
    {
        i = strlen(emplacement) - 1 ;
        emplacement[i] = '\0';
    }
    while((chaine[j] != '\0') && (chaine[j] != ' '))
    {
        emplacement[i] = chaine[j];
        j++;
        i++;
    }
    // effacer les espaces apres le mot 
    if(i > 1)
    {
        if(emplacement[i-2] == ' ')
        {
            i = i - 2;
            emplacement[i] = '\0';
            if(i != 0)
            {
                i--;
                while(emplacement[i] == ' ')
                {
                    emplacement[i] = '\0';
                    if(i != 0)
                    {
                        i--;
                    }
                }
            }
        }
    }

    return (0);
}
void inserer(char caractere , int position , char *chaine)
{
    int longueur , j;

    longueur = strlen(chaine);
    if(longueur == 0)
    {
        chaine[0] = caractere;
        chaine[1] = '\0';
    }
    else if(position <= longueur)
    {
        j = (longueur+1) ;
        while( (j != position) && (j != 0) )
        {
            chaine[j] = chaine[j-1];
            j--;
        }
        chaine[position] = caractere;
    }
}
int prend_ligne_command(char **emplacement, int taille , fichier *repertoire_actuel)
{
    int i , j , debut , fin , nbElement , pos , caractere ;
    char *ligne , *chemin , *nom;
    FILE *histoire;
    
    // Preparer pour l' enregistrement de la ligne de commande
    ligne = calloc(taille, sizeof(char));

    i = 23;
    nom = getenv("USER");
    i += strlen(nom);
    chemin = calloc(i , sizeof(char));
    sprintf(chemin , "../save/%s/histoire.txt" , nom);
    histoire = fopen(chemin , "a+");
    
    free(chemin);

    //   recuperation de brute de la ligne de notre prompt
    my_get_ligne(ligne , taille , repertoire_actuel);


    nbElement = 0;
    // reparation de la ligne pour eviter les erreurs avec les espaces
    debut = 0;
    while(ligne[debut] == ' ' || ligne[debut] == '\t')
    {
        debut++;
    }
    
    i = debut;
    j = 0;
    while(ligne[i] != '\0')
    {
        if(ligne[i] == ' ' || ligne[i] == '\t' )
        {
            emplacement[nbElement][j] = '\0';
            nbElement++;
            j = 0;
            while(ligne[i] == ' ' || ligne[i] == '\t')
            {
                i++;
            }
        }
        else
        {
            emplacement[nbElement][j] = ligne[i];
            i++;
            j++;
        }
    }
    emplacement[nbElement][j] = '\0';

    free(ligne);
    
    if(emplacement[nbElement][0] != '\0')
    {
        nbElement = nbElement + 1;
    }
    // Enregistrement de la ligne ecrite
    if(histoire != NULL)
    {
        fprintf(histoire , "\n" );
        for(i=0 ; i<nbElement ; i++)
        {
            fprintf(histoire , "%s " , emplacement[i]);
        }
        fclose(histoire);
    }

    if(emplacement[i][j-1] == '\n' )
    {
        emplacement[nbElement][j-1] = '\0';
    }

    return (nbElement);
}
int getNomJoueur(char *emplacement)
{
    char *tmp;

    tmp = calloc(MOT, sizeof(char));
    tmp = getenv("USER");

    if(tmp == NULL)
    {
        return (-1);
    }
    strcpy(emplacement, tmp);

    //free(tmp);

    return (0);
}
int get_point()
{
    int lec_point;
    FILE *ptr_Fic , *save ;
    char chemin_point[30] ;
    char *tmpFic;

    save = fopen( "../save/temp.txt" , "r");

    if(save == NULL)
    {
        printf("impossible d'ouvrir save !\n");
    }
    else
    {
        tmpFic = calloc(100 , sizeof(char));
        fgets(tmpFic , 100 , save);
        fgets(tmpFic , 100 , save);
        free(tmpFic);

        fscanf( save , "%s" , chemin_point );
        fclose(save);
    }

    ptr_Fic = fopen( chemin_point , "r");
    lec_point = 0;

    if( (ptr_Fic != NULL) && !(feof(ptr_Fic)) )
    {
        fscanf(ptr_Fic , "%d" , &lec_point);
    }
    else
    {
        lec_point = 0;
        ptr_Fic = fopen( chemin_point , "w+");
        fprintf(ptr_Fic , "0" );
    }

    if(ptr_Fic != NULL)
    {
        fclose(ptr_Fic);
    }
    return (lec_point);
}
void increment_score(int actuPTS)
{
    int ancien , ecrit;
    FILE *ptr_Fic , *save ;
    char *tmpFic;
    char ligne[10];

    char chemin_point[30] = {""};

    save = fopen( "../save/temp.txt" , "r");

    if(save == NULL)
    {
        printf("impossible d'ouvrir save !\n");
    }
    else
    {
        tmpFic = calloc(100 , sizeof(char));
        fgets(tmpFic , 100 , save);
        fgets(tmpFic , 100 , save);
        free(tmpFic);

        fscanf( save , "%s" , chemin_point );
        fclose(save);
    }
    ancien = get_point();
    ecrit = actuPTS;
    ptr_Fic = fopen( chemin_point , "w+");

    if(ptr_Fic == NULL)
    {
        printf("Score NON sauvegardee !\n");
    }
    else if(ancien == 0)
    {
        fprintf(ptr_Fic , "%d" , ecrit);
    }
    else if( !(feof(ptr_Fic)) )
    {
        ecrit = ecrit + ancien ;
        fprintf(ptr_Fic , "%d" , ecrit);
    }
    else
    {
        fprintf(ptr_Fic , "%d" , ecrit);
    }

    fclose(ptr_Fic);
}
int decrement_score(int penalite)
{
    int ancien , ecrit;
    FILE *ptr_Fic , *save ;
    char *tmpFic;
    char ligne[10];

    char chemin_point[30] = {""};

    save = fopen( "../save/temp.txt" , "r");

    if(save == NULL)
    {
        printf("Impossible d'ouvrir save !\n");
    }
    else
    {
        tmpFic = calloc(100 , sizeof(char));
        fgets(tmpFic , 100 , save);
        fgets(tmpFic , 100 , save);
        free(tmpFic);

        fscanf( save , "%s" , chemin_point );
        fclose(save);
    }
    
    ancien = get_point();
    ptr_Fic = fopen( chemin_point , "w+");

    if(ptr_Fic == NULL)
    {
        return (-1);
    }
    else if(ancien == 0 || (feof(ptr_Fic)))
    {
        return (-1);
    }
    else
    {
        ecrit = ecrit - 1;
        fprintf(ptr_Fic , "%d" , (ancien-1) );
    }

    fclose(ptr_Fic);
    return (1);
}
void get_pwd(fichier *adresse, char *emplacement)
{
    char *tmp;
    int i , j , test , taille , position;
    fichier *tmpFic;

    tmp = calloc(LIGNE, sizeof(char));
    strcat(tmp, "//");
    tmpFic = adresse;
    do
    {
        strcat(tmp, tmpFic->nom);
        test = strcmp(tmpFic->nom , "/");
        strcat(tmp, "/");
        tmpFic = tmpFic->parent;
    }
    while(tmpFic != NULL);


    strcat(emplacement, "/");
    taille = strlen(tmp);
    j = 1;
    position = taille - 4;
    while(position != 0)
    {
        i = position;
        while(tmp[i] != '/')
        {
            i--;
        }
        position = i - 1;
        i = i + 1;
        while(tmp[i] != '/')
        {
            emplacement[j] = tmp[i];
            i++;
            j++;
        }
        emplacement[j] = '/';
        j++;
    }
    emplacement[j] = '\0';

    free(tmp);
}
int commande_option(ligne_commande* ligne , char **liste , int nb_mot)
{
    int i , taille , nbMot , nb_option , nb_argument;

    taille = strlen(liste[0]) + 1;
    ligne->commande = calloc( taille , sizeof(char));
    strcpy(ligne->commande , liste[0]);
    nb_option = 0;
    nb_argument = 0;
    for(i=1 ; i<nb_mot ; i++)
    {
        if(liste[i][0] == '-')
        {
            nb_option++;
        }
        else
        {
            nb_argument++;
        }
    }
    ligne->option = calloc(nb_option+1 , sizeof(char*));
    ligne->argument = calloc(nb_argument+1 , sizeof(char*));
    nb_option = 0;
    nb_argument = 0;
    for(i=1 ; i<nb_mot ; i++)
    {
        if(liste[i][0] == '-')
        {
            ligne->option[nb_option] = calloc(strlen(liste[i])+1 , sizeof(char));
            strcpy(ligne->option[nb_option] , liste[i]);
            nb_option++;
        }
        else
        {
            ligne->argument[nb_argument] = calloc(strlen(liste[i])+1 , sizeof(char));
            strcpy(ligne->argument[nb_argument] , liste[i]);
            nb_argument++;
        }
    }
    ligne->option[nb_option] = NULL;
    ligne->argument[nb_argument] = NULL;

    return(0);
}
fichier *execute(char **commande , fichier *repertoire_actuel , int nbMot )
{
    int test , i , nb_commande;
    fichier *racine , *tmp;
    char *tempo , *chemin , *par_option;
    ligne_commande ligne;

    // Liste des commandes disponibles
    char *liste_commandes[] = {
        "ls", "cd", "pwd", "cat", "echo", "mkdir", "rm", "cp", "mv",
        "rmdir", "tail", "chmod", "exit", "clear", "aide", "histoire",
        "liste_commande", "major", "mission", "head" , "touch" , "favori", "nombre_ouverture", "demos" , NULL 
    };
    nb_commande = 24;


    commande_option(&ligne , commande , nbMot);
    racine = repertoire_actuel;
    // Recherche de fichier *racine
    while( racine->parent != NULL)
    {
        racine = racine->parent;
    }
    
    test = strcmp(commande[0], SORTIR);
    if(test == 0)
    {
        return (NULL);
    }
    
    for(i=0 ; ( (liste_commandes[i] != NULL) && (i < nb_commande) ) ; i++)
    {
        test = strcmp(liste_commandes[i] , ligne.commande);
        if(test == 0)
        {
            break;
        }
    }
    nb_commande = i;
    switch(nb_commande)
    {

        case 5:
        {
            nbMot = 0;
            while(ligne.argument[nbMot] != NULL)
            {
                nbMot++;
            }
            if( nbMot == 0 )
            {
                printf("(EE) : mkdir a besoin d'argument !\n");
            }
            else
            {
                nbMot = 0;
                while(ligne.argument[nbMot] != NULL)
                {
                    my_mkdir(racine , repertoire_actuel , ligne.argument[nbMot] , 0);
                    nbMot++;
                }
            }
            break;
        }
        case 7:
        {
            nbMot = 0;
            while(ligne.argument[nbMot] != NULL)
            {
                if(nbMot == 0)
                {
                    nbMot = 1;
                }
                nbMot++;
            }
            if(nbMot == 2)
            {
                my_cp( racine , repertoire_actuel , ligne.argument[0] , ligne.argument[1]);
            }
            else if(nbMot > 2)
            {
                nb_commande = 0;
                while(ligne.argument[(nb_commande)] != NULL)
                {
                    nb_commande++;
                }
                for(i=0 ; i < (nb_commande-1) ; i++)
                {
                    my_cp( racine , repertoire_actuel , ligne.argument[i] , ligne.argument[(nb_commande-1)]);
                }
            }
            else
            {
                printf("\t(EE) : cp besoin d'arguments source et destination !\n\tSaisissez << aide cp >> pour plus d'informations.\n");
                return (repertoire_actuel);
            }
            break;
        }
        case 11:
        {
            nbMot = 0;
            while(ligne.argument[nbMot] != NULL)
            {
                nbMot++;
            }
            if(nbMot == 2)
            {
                my_chmod(racine, repertoire_actuel, ligne.argument[0], ligne.argument[1]) ;
            }
            else
            {
                printf("Apprennez a utilisez chmod !\n");
            }
            break;
        }
        case 22:
        {
            afficher_nbr_ouverture(racine, 0) ;
            break;
        }
        case 21:
        {
            affichage(racine) ;
            break;
        }
        case 0:
        {
            if(ligne.option[0] != NULL)
            {
                i = 0;
                test = 0;
                while(ligne.option[i] != NULL)
                {
                    if(strcmp(ligne.option[i] , "-a") == 0)
                    {
                        if(test == 3)
                        {
                            test = 2;
                        }
                        else if(test == 0)
                        {
                            test = 1;
                        }
                    }
                    else if(strcmp(ligne.option[i] , "-al") == 0 || strcmp(ligne.option[i] , "-la") == 0 )
                    {
                        test = 2;
                    }
                    else if (strcmp(ligne.option[i] , "-l") == 0)
                    {
                        if(test == 1)
                        {
                            test = 2;
                        }
                        else if(test == 0)
                        {
                            test = 3;
                        }
                    }
                    else
                    {
                        printf("%s argument non encore connue !\n" , ligne.option[i]);
                    }
                    i++;
                }
                if(ligne.argument[0] != NULL)
                {
                    i = 0;
                    while(ligne.argument[i] != NULL)
                    {
                        switch(test)
                        {
                            case 1:
                            {
                                my_ls_a(racine , repertoire_actuel , ligne.argument[i]);
                                break;
                            }
                            case 2:
                            {
                                my_ls_la(racine , repertoire_actuel ,ligne.argument[i]);
                                break;
                            }
                            case 3:
                            {
                                my_ls_l(racine , repertoire_actuel , ligne.argument[i]);
                                break;
                            }
                            default :
                            {
                                break;
                            }
                        }
                        i++;
                    }
                }
                else
                {
                    switch(test)
                    {
                        case 1:
                        {
                            my_ls_a(racine , repertoire_actuel , ".");
                            break;
                        }
                        case 2:
                        {
                            my_ls_la(racine , repertoire_actuel , ".");
                            break;
                        }
                        case 3:
                        {
                            my_ls_l(racine , repertoire_actuel , ".");
                            break;
                        }
                        default :
                        {
                            break;
                        }
                    }
                }
                
            }
            else
            {
                if(nbMot == 1)
                {
                    my_ls(racine , repertoire_actuel , commande[1] );
                }
                else
                {
                    i = 0;
                    if(ligne.argument[i] != NULL)
                    {
                        while(ligne.argument[i] != NULL)
                        {
                            my_ls(racine, repertoire_actuel , ligne.argument[i]);
                            i++;
                        }
                    }
                }
            }
            break;
        }
        case 20: 
        {
            if( ligne.argument[0] == NULL)
            {
                printf("(EE) : touch a besoin d'argument !\n");
            }
            else
            {
                i = 0;
                while(ligne.argument[i] != NULL)
                {
                    my_touch( racine , repertoire_actuel , ligne.argument[i],0);
                    i++;
                }
            }
            break;
        }
        case 16: 
        {
            if( nbMot != 1)
            {
                printf("Oui liste_commande ignore ton argument !\n");
            }
            cat_fichier("../data/page_man/mes_commandes.txt" , false);
            break;
        }
        case 1:
        {
            i = 0;
            while(ligne.argument[i] != NULL)
            {
                i++;
            }
            if(ligne.option[0] != NULL)
            {
                printf("Option injuste ! \n");
            }
            else if(i > 1)
            {
                printf("Votre destination est indefini !\n");
            }
            else if(ligne.argument[0] != NULL)
            {
                tmp = my_cd(racine , repertoire_actuel , ligne.argument[0]);
                if(tmp != NULL)
                {
                    repertoire_actuel = tmp;
                }
            }
            else
            {
                tmp = my_cd(racine , repertoire_actuel , ligne.argument[0]);
                if(tmp != NULL)
                {
                    repertoire_actuel = tmp;
                }
            }
            break;
        } 
        case 9: 
        {
            if( ligne.argument[0] == NULL)
            {
                printf("(EE) : rmdir a besoin d'argument !\n");
            }
            else
            {
                i = 0;
                while(ligne.argument[i] != NULL)
                {
                    my_rmdir(racine , repertoire_actuel , ligne.argument[i]);
                    i++;
                }
            }
            break;
        }
        case 15: 
        {
            if(nbMot == 1)
            {
                my_history(false);
            }
            else if(ligne.argument[0] != NULL)
            {
                printf("(EE) : commande histoire mal compris !\n\n   Utilisation : histoire [-c]\n\n");
            }
            else
            {
                i = 0;
                while(ligne.option[i] != NULL)
                {
                    if(strcmp(ligne.option[i], "-c") == 0)
                    {
                        my_history(true);
                    }
                    else if((strcmp(ligne.option[i], "-") == 0))
                    {
                        printf("%s : quelle drole option !\n" , ligne.option[i]);
                    }
                    else
                    {
                        par_option = ligne.option[i]+1;
                        while(*par_option != '\0')
                        {
                            histoire_s(*par_option);
                            par_option++;
                        }
                    }
                    i++;
                }
            }
            break;
        }
        case 6: 
        {
            if( nbMot == 1)
            {
                printf("(EE) : rm a besoin d'argument !\n");
            }
            else
            {
                i = 0;
                while(ligne.argument[i] != NULL)
                {
                    my_rm(racine , repertoire_actuel , ligne.argument[i]);
                    i++;
                }
            }
            break;
        }
        case 2:
        {
            if ( nbMot == 1 )
            {
                my_pwd(repertoire_actuel);
            }
            else if(ligne.argument[0] != NULL)
            {
                printf("Argument non autorisee !\n");
            }
            else
            {
                if(ligne.option[0] != NULL)
                {
                    if( (strcmp(commande[1] , "-L") == 0 ) || (strcmp(commande[1] , "-L") == 0 ) 
                    || (strcmp(commande[1] , "-LP") == 0 ) || ( strcmp(commande[1] , "-PL") == 0 ))
                    {
                        my_pwd(repertoire_actuel);
                    }
                    else
                    {
                        printf("(EE) : Argument Invalide !\n");
                    }
                }
            } 
            break;
        }
        case 13:
        {
            if ( nbMot == 1)
            {
                screen_clear();          
            }
            else
            {
                printf("(EE) : clear ne veut pas d'argument !\n");
            }
            break;
        }
        case 3:
        {
            if( nbMot == 1)
            {
                printf("(EE) : cat a besoin d'argument pour le moment !\n");
            }
            else if(ligne.option[0] == NULL)
            {
                i = 0;
                while(ligne.argument[i] != NULL)
                {
                    if(chercher(repertoire_actuel , ligne.argument[i]) != NULL)
                    {
                        chemin =  calloc(LIGNE , sizeof(char));
                        sprintf(chemin , "../data/cat/%s" , ligne.argument[i]);
                        cat_fichier(chemin , false);
                        free(chemin);
                    }
                    else
                    {
                        printf("Erreur : %s n'existe pas \n", ligne.argument[i]) ;
                    }
                    i++;
                }
            }
            else
            {
                i = 0;
                while(ligne.option[i] != NULL)
                {
                    if(strcmp(ligne.option[i] , "-n") ==0)
                    {
                        i = 0;
                        while(ligne.argument[i] != NULL)
                        {
                            if(chercher(repertoire_actuel , ligne.argument[i]) != NULL)
                            {
                                chemin =  calloc(LIGNE , sizeof(char));
                                sprintf(chemin , "../data/cat/%s" , ligne.argument[i]);
                                cat_fichier(chemin , true);
                                free(chemin);
                            }
                            else
                            {
                                printf("Erreur : %s n'existe pas \n", ligne.argument[i]) ;
                            }
                        }
                    }
                    else
                    {
                        printf("%s : option inconnue !\n" , ligne.option[i]);   
                    }
                    i++;
                }
            }
            break;
        }
        case 19:
        {
            if( (nbMot == 1) || (ligne.argument[0] == NULL))
            {
                printf("(EE) : head a besoin de fichier comme argument !\n");
            }
            else if(ligne.option[0] == NULL)
            {
                for(i=0 ; ligne.argument[i] != NULL ; i++)
                {
                    head_fichier(ligne.argument[i] , 10);
                }
            }
            else
            {
                nb_commande = 0 ;
                for(i=0 ; ligne.option[i] != NULL ; i++)
                {
                    if(strcmp(ligne.option[i] , "-n") == 0)
                    {
                        nb_commande = atoi(ligne.argument[0]);
                    }
                    else
                    {
                        printf("%s : option non reconnue !\n", ligne.option[i]);
                    }
                }
                if(nb_commande != 0)
                {
                    test = 1;
                    while(ligne.argument[test] != NULL)
                    {
                        head_fichier(ligne.argument[test] , i);
                        test++;
                    }
                }
            }
            break;
        }
        case 10:
        {
            if( (nbMot == 1) || (ligne.argument[0] == NULL))
            {
                printf("(EE) : tail a besoin de fichier comme argument !\n");
            }
            else if(ligne.option[0] == NULL)
            {
                for(i=0 ; ligne.argument[i] != NULL ; i++)
                {
                    tail_fichier(ligne.argument[i] , 10);
                }
            }
            else
            {
                nb_commande = 0 ;
                for(i=0 ; ligne.option[i] != NULL ; i++)
                {
                    if(strcmp(ligne.option[i] , "-n") == 0)
                    {
                        nb_commande = atoi(ligne.argument[0]);
                    }
                    else
                    {
                        printf("%s : option non reconnue !\n", ligne.option[i]);
                    }
                }
                if(nb_commande != 0)
                {
                    test = 1;
                    while(ligne.argument[test] != NULL)
                    {
                        tail_fichier(ligne.argument[test] , i);
                        test++;
                    }
                }
            }
            break;
        }
        case 4:
        {
            my_echo( commande );
            break;
        }
        case 8: 
        {
            nbMot = 0;
            while(ligne.argument[nbMot] != NULL)
            {
                if(nbMot == 0)
                {
                    nbMot = 1;
                }
                nbMot++;
            }
            if(nbMot == 2)
            {
                my_mv( racine , repertoire_actuel , ligne.argument[0] , ligne.argument[1]);
            }
            else if(nbMot > 2)
            {
                nb_commande = 0;
                while(ligne.argument[(nb_commande)] != NULL)
                {
                    nb_commande++;
                }
                for(i=0 ; i < (nb_commande-1) ; i++)
                {
                    my_mv( racine , repertoire_actuel , ligne.argument[i] , ligne.argument[(nb_commande-1)]);
                }
            }
            else
            {
                printf("Vous avez besoin d'apprendre mv !\n-------------> Tapez aide mv \n");
            }
            break;
        }
        case 23:
        {
            tempo = calloc(LIGNE , sizeof(char));
            for(i=1 ; i<nbMot ; i++)
            {
                strcat(tempo , commande[i]);
                strcat(tempo , " ");
            }
            system(tempo);
            free(tempo);
            break;
        }
        case 17:
        {
            if(nbMot == 1)
            {
                printf("Eh ! le numero de la mission doit etre en argument !\n");
            }
            else if(ligne.option[0] != NULL)
            {
                printf("Pas d'option accepter !\n");
            }
            else
            {
                for(i=0 ; ligne.argument[i] != NULL ; i++)
                {
                    test = atoi(ligne.argument[i]);
                    qui_est_meilleur(test);
                }
            }
            break;
        }
        case 14:
        {
            if(nbMot == 1)
            {
                my_man("aide");
            }
            else if( ligne.option[0] != NULL)
            {
                printf(" Option non accepter !\n\n");
            }
            else
            {
                for(i=0 ; ligne.argument[i] != NULL ; i++)
                {
                    my_man(ligne.argument[i]);
                }
            }
            break;
        }
        default:
        {
            if(nbMot >= 1)
            {
                printf("(EE) %s : Commande introuvable !\n" , ligne.commande) ;
            }
            break;
        }
    }
    

    return (repertoire_actuel);
}
int rapport_mission(int num_mission , bool reussie)
{
    int val_mission;
    double valeur_temps;
    FILE *mission_registre;
    char *temps , *nom_joueur;

    if(reussie)
    {
        val_mission = 1;
    }
    else
    {
        val_mission = 0;
    }

    mission_registre = fopen("../save/record.txt" , "a+");
    if(mission_registre == NULL)
    {
        return (-1);
    }
    else
    {
        nom_joueur = getenv("USER");
        temps = getenv("TEMPS_DERNIER_PROMPT");
        if(nom_joueur == NULL || temps == NULL)
        {
            return (-2);
        }
        else
        {
            sscanf(temps , "%lf" , &valeur_temps);
            fprintf(mission_registre , "%d\t%d\t%lf\t%s\n" , num_mission , val_mission , valeur_temps , nom_joueur );
        }
    }

    fclose(mission_registre);
    return (1);
}
