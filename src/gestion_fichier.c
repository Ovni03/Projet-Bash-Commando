/**
 *  @file   gestion_fichier.c
 *  @brief  regroupe tous les fonctions qui manipule les fichiers
 *  @author Valisoa
 *  @date   29 Août 2025
 *  @version    1.0.0
 *
 *      toutes les manipulartions de fichier comme les enregistrements et les affichages sont regroupés
 *      dans celle-ci ; ces fonctions sont cités dans gestion_fichier.h 
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../include/mission.h"
#include "../include/prompt.h"
#include "../include/gestion_fichier.h"

void cat_fichier(char *nom_fichier , bool num)
{
    FILE *ptr_Fic;
    char *ligne;
    int num_ligne;

    num_ligne = 0;
    ptr_Fic = NULL;
    ptr_Fic = fopen(nom_fichier, "r");

    ligne = calloc(LIGNE, sizeof(char));
    if(ptr_Fic == NULL)
    {
        return;
    }
    while( !(feof(ptr_Fic)) )
    {
        fgets(ligne, LIGNE, ptr_Fic);
        if(num_ligne != 0)
        {
            if(num)
            {
                printf("%d\t" , num_ligne);
            }
        }
        printf("%s", ligne);
        num_ligne++;
    }
    free(ligne);
    printf("\n");
}
int affiche_ligne(char *chemin , int num_ligne , char *emplacement)
{
    FILE *ptr_Fic;
    int nb_ligne , position , i;
    char caractere;
    char *ligne;
    bool limite;
    
    ptr_Fic = fopen(chemin , "r");
    if(ptr_Fic == NULL)
    {
        return(-1);
    }
    
    // Compter le nombre de ligne pour eviter les boucles infinies sur mon lecture debutant vers la fin
    nb_ligne = 0;
    ligne = calloc(LIGNE , sizeof(char));
    while( !(feof(ptr_Fic)) )
    {
        fgets(ligne , LIGNE , ptr_Fic);
        nb_ligne++;
    }
    rewind(ptr_Fic);
    limite = false;
    // Ouverture a la ligne desire
    position = 0;
    if(num_ligne <= 0)
    {
        num_ligne *= (-1);
        if( num_ligne >= nb_ligne )
        {
            num_ligne = 1;
            limite = true;
        }
        else if(num_ligne == 0)
        {
            num_ligne = nb_ligne;
        }
        else
        {
            num_ligne = nb_ligne - num_ligne;
        }
        while( !(feof(ptr_Fic)) && (position != num_ligne) )
        {
            fgets(ligne , LIGNE , ptr_Fic);
            position++;
        }
        i = 0;
        while((ligne[i] != '\n') && ((ligne[i] != '\0')) )
        {
            printf("%c" , ligne[i]);
            emplacement[i] = ligne[i];
            i++;
        }
        emplacement[i] = '\0';
    }
    else
    {
        while( !(feof(ptr_Fic)) && (position != num_ligne) )
        {
            fgets(ligne , LIGNE , ptr_Fic);
            position++;
        }
        i = 0;
        while((ligne[i] != '\n') && ((ligne[i] != '\0')) )
        {
            printf("%c" , ligne[i]);
            emplacement[i] = ligne[i];
            i++;
        }
        emplacement[i] = '\0';
    }
    fclose(ptr_Fic);
    free(ligne);

    if( limite )
    {
        return (2);
    }

    return (1);
}

void insert_fin(char *nom_fichier, char *chaine_caractere)
{
    FILE *ptr_Fic;

    ptr_Fic = NULL;
    ptr_Fic = fopen(nom_fichier, "a");
    if(ptr_Fic == NULL)
    {
        exit(-1);
    }
    fprintf(ptr_Fic, "%s", chaine_caractere);

    fclose(ptr_Fic);
}

void head_fichier(char *nom_fichier , int max)
{
    FILE *ptr_Fic;
    char *ligne;
    int num_ligne;

    num_ligne = 0;
    ptr_Fic = NULL;
    ptr_Fic = fopen(nom_fichier, "r");

    ligne = calloc(LIGNE, sizeof(char));
    if(ptr_Fic == NULL)
    {
        printf("%s n'existe pas  ; si non vous n'avez pas le droit de voir ce qu'il y a dedans\n", nom_fichier);
        return;
    }
    while( !(feof(ptr_Fic)) && (num_ligne <= max))
    {
        fgets(ligne, LIGNE, ptr_Fic);
        num_ligne++;
        printf("%s", ligne);
    }
    free(ligne);
}
void tail_fichier(char *nom_fichier , int min)
{
    FILE *ptr_Fic;
    char *ligne;
    long num_ligne , nb_ligne , pos_actuel;
    char caractere;

    ptr_Fic = NULL;
    ptr_Fic = fopen(nom_fichier, "r");

    ligne = calloc(LIGNE, sizeof(char));
    if(ptr_Fic == NULL)
    {
        printf("%s n'existe pas  ; si non vous n'avez pas le droit de voir ce qu'il y a dedans\n", nom_fichier);
        return;
    }
    nb_ligne = 0;
    pos_actuel = 0;
    while( !(feof(ptr_Fic)) )
    {
        caractere = fgetc(ptr_Fic);
        pos_actuel++;
        if(caractere == '\n')
        {
            nb_ligne++;
        }
    }
    fseek(ptr_Fic , -1 , SEEK_END);
    // Deplacer vers la ligne N° min
    num_ligne = 0;
    while(num_ligne != min)
    {
        if(pos_actuel == 0)
        {
            break;
        }
        caractere = fgetc(ptr_Fic);
        if(caractere == '\n')
        {
            num_ligne++;
        }
        pos_actuel--;
        fseek(ptr_Fic , -2 , SEEK_CUR);
    }
    if(pos_actuel != 0)
    {
        fgets(ligne, LIGNE, ptr_Fic);
    }
    while( !(feof(ptr_Fic)) )
    {
        fgets(ligne, LIGNE, ptr_Fic);
        printf("%s", ligne);
    }
    printf("\n");
    free(ligne);
}
int lire_histoire()
{
    FILE *hist;
    char ligne[100];
    hist = fopen("../data/histoire.txt" , "r");

    if(hist == NULL)
    {
        return (-1);
    }
    if(feof(hist))
    {
        return (-2);
    }
    else
    {
        while( feof(hist) == 0 )
        {
            fgets(ligne , 100 , hist);
            printw("%s" , ligne);
        }
    }

    fclose(hist);
    return (1);
}
void voir_indice(Mission *mission)
{
    int validite , retour , i;
    char rep;
    char reponse[7];

    strcat(reponse , "YyNnOo");
    printf("Voir de l'indice decremente votre score actuel !\n");
    validite = 0;
    do
    {
        printf("Veuillez valider l'action ? [Y/N]");
        retour = scanf("%c" , &rep);
        // vider les inpuretés pour eviter les erreurs du prochain scanf ou autre entree
        while(getchar() != '\n' );
        for(i=0 ; i<7 ; i++)
        {
            if(rep == reponse[i])
            {
                validite = 1;
            }
        }
    }
    while( ( validite != 1 ) || ( retour != 1) );
    
    if(rep == 'Y' || rep == 'y' || rep == 'o' || rep == 'O')
    {
        validite = decrement_score(1);

        if(validite < 0)
        {
            printf("Votre score est insuffisante !\n");
        }
        else
        {
            printf("%s\n" , mission->indice);
        }
    }
}

void qui_est_meilleur(int n_miss)
{
    FILE *record;
    char *ligne , *nom , *ndj;
    int reussite , num_mission , i;
    char tmp[10];
    double temps , min_trouve;

    record = fopen("../save/record.txt" , "r");
    min_trouve = 1000;
    if(record == NULL)
    {
        fprintf(stderr , "Record inexistant ! \n");
    }
    else
    {
        ligne = calloc(LIGNE , sizeof(char));
        nom = calloc(MOT , sizeof(char));
        ndj = calloc(MOT , sizeof(char));
        while( !(feof(record)) )
        {
            fgets(ligne , LIGNE , record);
            sscanf(ligne , "%d\t%d\t%s\t%s\n" , &num_mission , &reussite , tmp , nom );
            i = 0;
            while(tmp[i] != ',' && tmp[i] != '\0')
            {
                i++;
            }
            if(tmp[i] != '\0')
            {
                tmp[i] = '.';
            }
            temps = atof(tmp);
            if(num_mission == n_miss)
            {
                if(temps < min_trouve)
                {
                    min_trouve = temps;
                    strcpy(ndj , nom);
                }
            } 
        }

        //! traitement de donnée obtenus d'après la lecture du fichier de score
        if((ndj[0] != '\0') && (min_trouve != 1000))
        {
            printf("La meilleur pour la mission %d est %s \n" , n_miss , ndj);
        }
        else
        {
            printf("Score pas encore batu pour la mission n°%d .\n" , n_miss);
        }
        fclose(record);
        free(ligne);
        free(ndj);
        free(nom);
    }
}
int my_man(char *nom_commande)
{
    FILE *la_page;
    char *chaine_char;

    chaine_char = calloc(LIGNE , sizeof(char));

    sprintf(chaine_char , "../data/page_man/aide_%s.txt" , nom_commande);
    la_page = fopen(chaine_char  , "r");
    if(la_page == NULL)
    {
        printf("Aucune page d'aide pour %s\n" , nom_commande);
        return (-1);
    }
    
    while( !feof(la_page) )
    {
        fgets(chaine_char , LIGNE , la_page);
        printf("%s" , chaine_char);
    }

    free(chaine_char);
    fclose(la_page);
    return (1);
}
int histoire_s(char option)
{
    FILE *histoire;
    int taille , test , nb_ligne , signe;
    char *nom , *chemin ;
    char **ligne;

    // Si l'option est disponible ici
    if((option != 's') && (option != 'S'))
    {
        printf("Option non reconnues !\n");
        return (-2);
    }

    nom = getenv("USER");
    taille = strlen(nom) + 24;
    chemin = calloc(taille , sizeof(char));
    sprintf(chemin , "../save/%s/histoire.txt" , nom);
    
    histoire = fopen(chemin , "r");
    if(histoire == NULL)
    {
        return (-1);
    }
    
    test = feof(histoire);
    nb_ligne = 0 ;
    chemin = realloc(chemin , LIGNE*sizeof(char));
    while(test != 1)
    {
        fgets(chemin , LIGNE , histoire);
        nb_ligne++;
        test = feof(histoire);
    }
    rewind(histoire);
    ligne = calloc((nb_ligne+1) , sizeof(char *));
    test = 0;
    while(test != nb_ligne)
    {
        fgets(chemin , LIGNE , histoire);
        taille = strlen(chemin) + 1;
        ligne[test] = calloc(LIGNE , sizeof(char));
        taille = strlen(chemin);
        if(taille > 0)
        {
            if(chemin[taille-1] == '\n')
            {
                chemin[taille-1] = '\0';
            }
        }
        strcpy(ligne[test] , chemin);
        test++;
    }
    ligne[test] = NULL;
    // triage de char **ligne
    if(nb_ligne > 0)
    {
        test = 0;
        while(ligne[test+1] != NULL)
        {
            if(nb_ligne > 1)
            {
                taille = test ;
                while(ligne[taille+1] != NULL)
                {
                    signe = strcmp(ligne[taille] , ligne[test]);
                    if((signe < 0) && (option == 's'))
                    {
                        echanger(ligne[taille] , ligne[test]);
                    }
                    else if((signe > 0) && (option == 'S'))
                    {
                        echanger(ligne[taille] , ligne[test]);
                    }
                    taille++;
                }
            }
            test++;
        }
    }
    test = 0;
    taille = 1;
    while((ligne[test] != NULL) || (test < nb_ligne))
    {
        if(ligne[test][0] != '\0')
        {
            printf("%d\t%s\n" , taille , ligne[test]);
            taille++;
        }
        test++;
    }

    free(ligne);

    return (1);
}
void echanger(char *chaine1 , char *chaine2)
{
    char *tmp;

    tmp = calloc(LIGNE , sizeof(char));
    strcpy(tmp , chaine1);
    strcpy(chaine1 , chaine2);
    strcpy(chaine2 , tmp);

    free(tmp);
}
int my_history(bool effacer)
{
    FILE *histoire;
    char *nom , *emplacement;
    int taille ;

    // Vision du fichier d'historique de commande
    nom = getenv("USER");
    taille = 23;
    taille += strlen(nom);
    emplacement = calloc(taille , sizeof(char));
    sprintf(emplacement , "../save/%s/histoire.txt" , nom);
    histoire = fopen(emplacement , "r");
    if(histoire == NULL)
    {
        printf("Histoire : vide !\n");
        free(emplacement);
        return (-1);
    }
    else
    {
        fclose(histoire);
    }


    // vider le fichier de histoire
    if(effacer)
    {
        remove(emplacement);
    }
    else
    {
        cat_fichier(emplacement , true);
    }

    free(emplacement);

    return (1);
}