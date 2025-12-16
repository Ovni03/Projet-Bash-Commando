/**
 *  @file arborescence.c
 *  @brief fonction pour manipuler les arborescences
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../include/arborescence.h"
#include "../include/prompt.h"


/** 
* @fn creer
* @arg parent: parent de dossier ou fichier à creer, 
       nom: nom de dossier ou fichier
       estDossier: pour avoir connaitre si il est dossier ou pas
* @brief creer un dossier ou un fichier
*/
fichier* creer(fichier *parent, char *nom, int estDossier, int statut)
{
    fichier* nouveau = (fichier*)malloc(sizeof(fichier)) ;
    strcpy(nouveau->nom, nom) ;
    nouveau->estDossier = estDossier ;
    nouveau->parent = parent ;
    nouveau->estProtege = statut ;
    strcpy(nouveau->perm, "rwx") ;  
    nouveau->premierfils = NULL ;
    nouveau->frereSuivant = NULL ;
    nouveau->nombre_ouverture = 0 ;
    return nouveau ;
}


/** 
* @fn ajouter_enfant
* @arg nom: nom de dossier ou fichier à ajouter 
       parent: on ajoute le dossier ou fichier dans parent
* @brief ajouter un dossier ou un fichier dans un dossier
*/
void ajouter_enfant(fichier *parent, char *nom, int nombre, int statut) 
{
    if(!parent->estDossier)     //  On ne peut rien y ajouter si il n'est pas un dossier 
    {
        printf("Erreur: %s n'est pas un dossier.\n", parent->nom) ;
        return ;
    }

    fichier *enfant = creer(parent, nom, nombre,statut) ;   
    if(parent->premierfils == NULL)   //Si il n'y pas déjà un premier fils dans le dossier  
    {
        parent->premierfils = enfant ;
    }
    else
    {
        fichier *temp = parent->premierfils ;   
        while(temp->frereSuivant != NULL)     //Parcourir dans les frères suivants si il y a déjà des contenus dans le dossier
        {
            temp = temp->frereSuivant ;
        }
        temp->frereSuivant = enfant ;
    }
}


/** 
* @fn my_touch
* @arg parent: le lieu pour ajouter un fichier
       nom: nom de fichier
* @brief creer un fichier dans un dossier 
*/
void my_touch(fichier *racine, fichier *courant, char *nom,int statut) 
{
    char *dir , *dernier ;
    char *mot ;
    mot = malloc(strlen(nom) *sizeof(char)) ;
    int compt = 0 ;
    fichier* temoin ;
    fichier *parent ;
    strcpy(mot, nom) ;
    dir = strtok(mot, "/") ;    //Vérifier si le nom de dossier est un répértoire absolu (exemple:/.../.../..../) ou un seul mot         
    while(dir != NULL)   //Parcourir le répértoire absolu
    {
        compt++ ;
        dernier = dir ;    
        dir = strtok(NULL, "/") ;
    }
    if (compt == 1)   //Si on ne voit pas un répértoire absolu mais un seul mot
    {
        temoin = chercher(courant, dernier) ;    //Vérifier si existe déjà
        if(temoin == NULL)
        {
            if(courant->perm[1] != 'w' || courant->perm[2] != 'x')
            {
                printf("Permission non accordée\n") ;
                return ;
            }
            ajouter_enfant(courant, dernier, 0, statut) ;
        }
        else
        {
            printf("Erreur: le fichier existe déjà\n") ;
        }
        return ;
    }
    if (nom[0] == '/')      //Si le répértoire absolu se commence par un "/" , alors on va parcourir depuis la racine 
    {
        parent = racine; 
    }
    else    //Sinon parcourir depuis le répértoire courant
    {
        parent = courant ;
    }
    if (compt > 1)     //Si le nom de dossier est un répértoire absolu (exemple:/.../.../..../)  
    {
        dir = strtok(nom, "/") ;
        if(strcmp(dir,"..") == 0)
        {
            strcpy(dir,courant->parent->nom) ;
        }
        if(strcmp(dir,".") == 0)
        {
            strcpy(dir,courant->nom) ;
        }
        while(dir != NULL)     //Parcourir la chaine 
        {
            if (strcmp(dir, dernier) == 0)
            {
                break ;
            }

            if(dir != dernier)
            {
                parent = chercher(parent, dir) ;    //Chercher les dossiers un par un si ils existent
            }

            if(parent == NULL)     //Si on ne trouve pas, on envoie un message d'erreur: destination introuvable
            {
                printf("Erreur: %s : aucun dossier de ce nom\n", dir) ;
                return  ;
            }
            
            dir = strtok(NULL, "/") ;
        }
    }

    temoin = chercher(parent, dernier) ;    //Vérifier si existe déjà
    if(temoin == NULL)
    {
        if(parent->perm[1] != 'w' || parent->perm[2] != 'x')
        {
            printf("Permission non accordée\n") ;
            return ;
        }
        ajouter_enfant(parent, dernier, 0,statut) ;
    }
    else
    {
        printf("Erreur: le fichier existe déjà\n") ;
    }

}


/** 
* @fn afficher
* @arg home: le dossier où on fait l'affichage
       prfnd: profondeur de l'affichage
* @brief afficher recursivement les contenus d'un dossier 
*/
void afficher(fichier* home, int prfnd)
{
    if(home == NULL)
    {
        return ;
    }
    for(int i = 0; i<prfnd; i++)
    {
        printf("  ") ;
    }
      printf("%s%s\n", home->nom, home->estDossier ? "/" : "") ;
      afficher(home->premierfils, prfnd + 1) ;   //On appelle recursivement la fonction lui-même
      afficher(home->frereSuivant, prfnd) ; 
}



/** 
* @fn my_ls
* @arg répértoire: le dossier où on fait l'affichage
* @brief afficher seulement les fils de ce dossier s'ils le sont pas cachés
*/
void my_ls(fichier* racine, fichier *courant,char *nom)
{
    fichier *repertoire ;
    if(nom[0]=='\0')
    {
        repertoire = courant ;
    }
    else if(strcmp(nom, "/") == 0)
    {
        repertoire = racine ;
    }
    else
    {
            if (strcmp(nom, "..") == 0)      //Pour remonter dans son parent
            {
                if (courant->parent) 
                {
                    repertoire = courant->parent ;  //dans son parent
                }
                else
                {
                    repertoire = courant; // reste à la racine
                }
            }

            else if (strcmp(nom, ".") == 0)   //Pour ne pas faire un déplacement
            {
                repertoire = courant ;
            }
            else 
            {
                char *dir ;
                fichier *tmp , *dernier;
                if (nom[0] == '/')      //Si le nom se commence par un "/", donc parcourir depuis la racine   
                {
                    tmp = racine; 
                }
                else    //Sinon, parcourir depuis le répértoire courant 
                {
                    tmp = courant ;
                }
                dir = strtok(nom,"/") ;    //Si le destination se fait dans un répértoire absolu
                if(strcmp(dir,".") == 0)
                {
                    dir = strtok(NULL,"/") ;
                }
                if(strcmp(dir,"..") == 0)
                {
                    if(courant->parent != NULL)
                    {
                        tmp = courant->parent ;
                    }
                    else
                    {
                        tmp = courant ;
                    }
                    dir = strtok(NULL,"/") ;
                }
                
                while(dir != NULL)    //Parcourir la chaine
                {
                    tmp = chercher(tmp,dir) ;    //Vérifier un par un si les dossiers existent   
                    if (tmp == NULL)
                    {
                        printf("Erreur %s : aucun dossier de ce nom\n", dir) ;
                        return ;
                    }
                    repertoire = tmp ;
                    dir = strtok(NULL, "/") ;
                }
            }
    }
    if(repertoire->perm[0] != 'r')
    {
        printf("Permission non accordée\n") ;
        return ;
    }
    fichier *temp = repertoire->premierfils ;
    while(temp)
    {
        if (temp->nom[0] != '.')      //On n'affiche pas un fichier si son nom se commence par un point
        {
            if (temp->estDossier)
            {
                printf("\033[1;36m%s\033[0m  ",temp->nom);
            }
            else
            {
                printf("\033[1;37m%s\033[0m  ",temp->nom);
            }
        }
        
        temp = temp->frereSuivant ;
    }
    printf("\n") ;
}



/** 
* @fn my_ls_a
* @arg répértoire: le dossier où on fait l'affichage
* @brief afficher tous les fils du dossier
*/
void my_ls_a(fichier* racine, fichier *courant,char *nom)
{
    fichier *repertoire ;
    if(nom[0]=='\0')
    {
        repertoire = courant ;
    }
    else if(strcmp(nom, "/") == 0)
    {
        repertoire = racine ;
    }
    else
    {
            if (strcmp(nom, "..") == 0)      //Pour remonter dans son parent
            {
                if (courant->parent) 
                {
                    repertoire = courant->parent ;  //dans son parent
                }
                else
                {
                    repertoire = courant; // reste à la racine
                }
            }

            else if (strcmp(nom, ".") == 0)   //Pour ne pas faire un déplacement
            {
                repertoire = courant ;
            }
            else 
            {
                char *dir ;
                fichier *tmp , *dernier;
                if (nom[0] == '/')      //Si le nom se commence par un "/", donc parcourir depuis la racine   
                {
                    tmp = racine; 
                }
                else    //Sinon, parcourir depuis le répértoire courant 
                {
                    tmp = courant ;
                }
                dir = strtok(nom,"/") ;    //Si le destination se fait dans un répértoire absolu
                if(strcmp(dir,".") == 0)
                {
                    dir = strtok(NULL,"/") ;
                }
                if(strcmp(dir,"..") == 0)
                {
                    if(courant->parent != NULL)
                    {
                        tmp = courant->parent ;
                    }
                    else
                    {
                        tmp = courant ;
                    }
                    dir = strtok(NULL,"/") ;
                }
                
                while(dir != NULL)    //Parcourir la chaine
                {
                    tmp = chercher(tmp,dir) ;    //Vérifier un par un si les dossiers existent   
                    if (tmp == NULL)
                    {
                        printf("Erreur %s : aucun dossier de ce nom\n", dir) ;
                        return ;
                    }
                    repertoire = tmp ;
                    dir = strtok(NULL, "/") ;
                }
            }
    }
    if(repertoire->perm[0] != 'r')
    {
        printf("Permission non accordée\n") ;
        return ;
    }
    fichier *temp = repertoire->premierfils ;
    while(temp)
    {
        if (temp->estDossier)
        {
            printf("\033[1;36m%s\033[0m  ",temp->nom);
        }
        else
        {
            printf("\033[1;37m%s\033[0m  ",temp->nom);
        }
        temp = temp->frereSuivant ;
    }
    printf("\n") ;
}
/** 
* @fn my_rmdir
* @arg courant: le dossier où on fait le suppression
       nom: nom de dossier à supprimer
* @brief supprimer seulement un dossier vide 
*/
void my_rmdir(fichier *racine, fichier *courant, char *nom)
{ 

    char *dir , *dernier ;
    char *mot ;
    mot = malloc(strlen(nom) *sizeof(char)) ;
    int compt = 0 ;
    fichier* temoin ;
    fichier *parent ;
    char *suppr ;
    strcpy(mot, nom) ;
    if(strcmp(nom,"/") == 0)
    {
        printf("Erreur: impossible de supprimer racine\n") ;
        return ;
    }
    
    
        dir = strtok(mot, "/") ;    //Vérifier si le nom de dossier est un répértoire absolu (exemple:/.../.../..../) ou un seul mot         
        while(dir != NULL)   //Parcourir le répértoire absolu
        {
            compt++ ;
            dernier = dir ;    
            dir = strtok(NULL, "/") ;
        }
        if (compt == 1)   //Si on ne voit pas un répértoire absolu mais un seul mot
        {
            parent = courant ;
            suppr = dernier ;
        }
        else
        {
            if (nom[0] == '/')      //Si le répértoire absolu se commence par un "/" , alors on va parcourir depuis la racine 
            {
                parent = racine; 
            }
            else    //Sinon parcourir depuis le répértoire courant
            {
                parent = courant ;
            }
            dir = strtok(nom, "/") ;
            if(strcmp(dir,"..") == 0)
            {
                //strcpy(dir,courant->parent->nom) ;
                dir = strtok(NULL, "/") ;
            }
            if(strcmp(dir,".") == 0)
            {
                //strcpy(dir,courant->nom) ;
                dir = strtok(NULL, "/") ;
            }
            while(dir != NULL)     //Parcourir la chaine 
            {
                if (strcmp(dir, dernier) == 0)
                {
                    suppr = dernier ;
                    break ;
                }

                if(dir != dernier)
                {
                    parent = chercher(parent, dir) ;    //Chercher les dossiers un par un si ils existent
                }

                if(parent == NULL)     //Si on ne trouve pas, on envoie un message d'erreur: destination introuvable
                {
                    printf("Erreur: %s : aucun dossier de ce nom\n", dir) ;
                    return  ;
                }
                    
                dir = strtok(NULL, "/") ;
            }
        
        
    }
    if (parent->perm[1] != 'w' || parent->perm[2] != 'x')
    {
        printf("rm : %s: Permission non accordée\n", nom) ;
        return ;
    }  
    fichier *temp = parent->premierfils ;
    fichier *prec = NULL ;

    while(temp != NULL)     //Parcourir les contenus du répértoire
    {
        if(strcmp(temp->nom, suppr) == 0)
        {
            if(!temp->estDossier) //Si il n'est pas un dossier
            {
                printf("Erreur: %s n'est pas un dossier\n", temp->nom) ;
                return ;
            }
            if(temp->premierfils != NULL)   //Si il n'est pas vide
            {
                printf("Erreur: %s n'est pas vide\n", temp->nom) ;
                return ;
            }
            if(temp->estProtege)
            {
                printf("Erreur: ce dossier est protégé\n") ;
                return ;
            }
            if (prec != NULL)   //Pour ne pas détruire l'arborescence
            {
                prec->frereSuivant = temp->frereSuivant;
            }
            else 
            {
                parent->premierfils = temp->frereSuivant;
            }
             
            free(temp) ;
            return ;
        }
        prec = temp ;
        temp = temp->frereSuivant ;
    }
    printf("Erreur: %s n'existe pas\n", suppr) ;
}


/** 
* @fn my_cd
* @arg racine: un répère principal
       courant: le dossier où on est maintenant
       nom: nom de dossier où on va se déplacer
* @brief creer un fichier dans un dossier 
*/
fichier* my_cd(fichier *racine,fichier *courant, char *nom)
{
    if(nom == NULL || strcmp(nom,"/") == 0)
    {
        return(racine) ;
    }
    if (strcmp(nom, "..") == 0)      //Pour remonter dans son parent
    {
        if (courant->parent)
        {
            courant->parent->nombre_ouverture++ ;
            return courant->parent ;  //dans son parent
        }
        courant->nombre_ouverture++ ;
        return courant; // reste à la racine
    }
    if (strcmp(nom, ".") == 0)   //Pour ne pas faire un déplacement
    {
        courant->nombre_ouverture++ ;
        return(courant) ;
    }
    char *dir ;
    fichier *tmp , *dernier;
    if (nom[0] == '/')      //Si le nom se commence par un "/", donc parcourir depuis la racine   
    {
        tmp = racine; 
    }
    else    //Sinon, parcourir depuis le répértoire courant 
    {
        tmp = courant ;
    }
    dir = strtok(nom,"/") ;    //Si le destination se fait dans un répértoire absolu
    if(strcmp(dir,".") == 0)
    {
        dir = strtok(NULL,"/") ;
    }
    if(strcmp(dir,"..") == 0)
    {
        if(courant->parent != NULL)
        {
            tmp = courant->parent ;
        }
        else
        {
            tmp = courant ;
        }
        dir = strtok(NULL,"/") ;
    }
    
    while(dir != NULL)    //Parcourir la chaine
    {
        tmp = chercher(tmp,dir) ;    //Vérifier un par un si les dossiers existent   
        if (tmp == NULL)
        {
            printf("Erreur %s : aucun dossier de ce nom\n", dir) ;
            return (courant) ;
        }
        dernier = tmp ;
        dir = strtok(NULL, "/") ;
    }
    if (dernier->estDossier == 1)    //On fait le déplacement si il est vraiment dossier
    {
        if(dernier->perm[2] != 'x')
        {
            printf("cd : %s: Permission non accordée\n", dernier->nom) ;
            return (courant) ;
        }
        dernier->nombre_ouverture++ ;
        return dernier;
    }
    else if (dernier->estDossier == 0)    //Si il n'est pas dossier, on envoie message d'erreur alors on ne fait rien de déplacement
    {
        printf("Erreur: %s n'est pas un dossier\n", dernier->nom) ;
        return(courant) ;
    }
    
    printf("my_cd: dossier '%s' introuvable\n", nom);
    return courant;
}


/** 
* @fn cp
* @arg racine: un répère principal
       rep_actuel: le dossier où on est maintenant
       nom: nom de source à copier
       destination: destination de copie
* @brief copier seulement un fichier dans un dossier 
*/
void my_cp(fichier *racine, fichier *rep_actuel, char *nom, char *destination)
{
    if(strcmp(nom, "/") == 0)
    {
        printf("Erreur: impossible de copier racine\n") ;
        return ;
    }
    fichier *source ;
    char *mot1 ;
    fichier *temp, *temp1, *courant1 ;
    if (nom[0] == '/')    //Si le nom source se commence par un "/", parcourir depuis la racine 
    {
        courant1 = racine; 
    }
    else    //Sinon, parcourir depuis le répértoire actuel
    {
        courant1 = rep_actuel ;
    }
    mot1 = strtok(nom, "/") ;
    if(strcmp(mot1,".") == 0)
    {
        mot1 = strtok(NULL,"/") ;
    }
    if(strcmp(mot1,"..") == 0)
    {
        if(rep_actuel->parent != NULL)
        {
            courant1 = rep_actuel->parent ;
        }
        else
        {
            courant1 = rep_actuel ;
        }
        mot1 = strtok(NULL,"/") ;
    }
    while (mot1 != NULL)      //parcourir la source
    {
        source = chercher(courant1, mot1) ;     //Vérifier un par un si source existe
        if (source == NULL)       //Si source introuvable
        {
            printf("Erreur: %s (source) introuvable\n", mot1) ;
            return ;
        }
        courant1 = source ;
        mot1 = strtok(NULL, "/") ;
    }
    if (source->estDossier)      //Si le source est un dossier, on envoie un message d'erreur
    {
        printf("cp: -r non spécifié ; omission du répertoire %s\n", source->nom) ;
        return ;
    }


    //pour la destination
    fichier *courant, *dest ;
    char *mot ;
    if(strcmp(destination, ".") == 0)
    {
        dest = rep_actuel ;
    }
    else if(strcmp(destination, "..") == 0)
    {
        if(rep_actuel->parent == NULL)
        {
            printf("Erreur: %s n'a pas de dossier parent\n", rep_actuel->nom) ;
            return ;
        }
        dest = rep_actuel->parent ;
    }
    else if(strcmp(destination,"/") == 0)
    {
        dest = racine ;
    }
    else
    {
        if (destination[0] == '/')    //Si le destination se commence par un "/", parcourir depuis la racine 
        {
            courant = racine; 
        }
        else
        {
            courant = rep_actuel ;  //Sinon, parcourir depuis rep_actuel
        }
        mot = strtok(destination, "/") ;
        if(strcmp(mot,".") == 0)
        {
            mot = strtok(NULL,"/") ;
        }
        if(strcmp(mot,"..") == 0)
        {
            if(rep_actuel->parent != NULL)
            {
                courant = rep_actuel->parent ;
            }
            else
            {
                courant = rep_actuel ;
            }
            mot = strtok(NULL,"/") ;
        }
        while (mot != NULL)
        {
            dest = chercher(courant, mot) ;     //Vérifier un par un si les dossiers existent
            if (dest == NULL)       //Si destination introuvable
            {
                printf("Erreur: %s (destination) introuvable\n", mot) ;
                return ;
            }
            courant = dest ;
            mot = strtok(NULL, "/") ;
        }
        if (!dest->estDossier)     //Si destination n'est pas un dossier, on ne peut rien y ajouter  
        {
            printf("Erreur: %s (destination) n'est pas un dossier", dest->nom) ;
            return ;
        }
    }
    if(chercher(dest, source->nom) != NULL)
    {
        printf("Erreur: ce nom de fichier existe déjà\n") ;
        return ;
    }
    if(dest->perm[1] != 'w' || dest->perm[2] != 'x')
    {
        printf("Permission non accordée\n") ;
        return ;
    }
    my_touch(racine,dest, source->nom, source->estProtege) ;
}



/** 
* @fn my_mv
* @arg racine: un répère principal
       parent: le dossier où on est maintenant
       nom: nom de source à déplacer
       destination: destination
* @brief déplacer un dossier ou un fichier 
*/
void my_mv(fichier *racine, fichier *rep_actuel, char *nom, char *destination)
{
     if(strcmp(nom, "/") == 0)
    {
        printf("Erreur: impossible de déplacer racine\n") ;
        return ;
    }
    fichier *source ;
    char *mot1 ;
    fichier *temp, *temp1, *courant1 ;
    if (nom[0] == '/')    //Si le nom source se commence par un "/", parcourir depuis la racine 
    {
        courant1 = racine ; 
    }
    else    //Sinon, parcourir depuis le répértoire actuel
    {
        courant1 = rep_actuel ;
    }
    mot1 = strtok(nom, "/") ;
    if(strcmp(mot1,".") == 0)
    {
        mot1 = strtok(NULL,"/") ;
    }
    if(strcmp(mot1,"..") == 0)
    {
        if(rep_actuel->parent != NULL)
        {
            courant1 = rep_actuel->parent ;
        }
        else
        {
            courant1 = rep_actuel ;
        }
        mot1 = strtok(NULL,"/") ;
    }
    
    while (mot1 != NULL)      //parcourir la source
    {
        source = chercher(courant1, mot1) ;     //Vérifier un par un si source existe
        if (source == NULL)       //Si source introuvable
        {
            printf("Erreur: %s (source) introuvable\n", mot1) ;
            return ;
        }
        courant1 = source ;
        mot1 = strtok(NULL, "/") ;
    }

    //pour la destination
    fichier *courant, *dest ;
    char *mot ;
    char copie_dest[20] ;
    char *dernier ;
    if(strcmp(destination, ".") == 0)
    {
        if(rep_actuel == source->parent)
        {
            printf("Erreur: impossible de déplacer %s dans le même répértoire\n", source->nom) ;
            return ;
        }
        if(rep_actuel->perm[1] != 'w' || rep_actuel->perm[2] != 'x')
        {
            printf("Permission non accordée\n") ;
            return ;
        }
        deplacer(rep_actuel, source) ;
    }

    else if(strcmp(destination, "..") == 0)
    {
        if(rep_actuel->parent == NULL)
        {
            printf("Erreur: %s n'a pas de dossier parent\n", rep_actuel->nom) ;
            return ;
        }
        if(rep_actuel->parent->perm[1] != 'w' || rep_actuel->parent->perm[2] != 'x')
        {
            printf("Permission non accordée\n") ;
            return ;
        }
        
        deplacer(rep_actuel->parent, source) ;
    }
    else if(strcmp(destination, "/") == 0)
    {
        deplacer(racine, source) ;
    }
    else
    { 
        char* test ;
        strcpy(copie_dest, destination) ;
        test = strtok(copie_dest,"/") ;
        while(test != NULL)
        {
            dernier = test ;
            test = strtok(NULL, "/") ;
        }
        if (destination[0] == '/')    //Si le destination se commence par un "/", parcourir depuis la racine 
        {
            courant = racine; 
        }
        if(strchr(destination,'/')==NULL)
        {
            dest = chercher(rep_actuel,destination) ;
            if(dest != NULL)
            {
                 if(dest->perm[1] != 'w' || dest->perm[2] != 'x')
                {
                    printf("Permission non accordée\n") ;
                    return ;
                }
                deplacer(dest, source) ;
                return ;
            }
            strcpy(source->nom, destination) ;    //renommer
            return;
        }
        else
        {
            courant = rep_actuel ;  //Sinon, parcourir depuis rep_actuel
        }

        mot = strtok(destination, "/") ;

        while (mot != NULL)
        {
            dest = chercher(courant, mot) ;     //Vérifier un par un si les dossiers existent
            if(strcmp(mot,dernier) == 0   &&   dest != NULL)
            {
                if (!dest->estDossier)     //Si destination n'est pas un dossier, on ne peut rien y ajouter  
                {
                    printf("Erreur: %s (destination) n'est pas un dossier", dest->nom) ;
                    return ;
                }
                if(dest->perm[1] != 'w' || dest->perm[2] != 'x')
                {
                    printf("Permission non accordée\n") ;
                    return ;
                } 
                deplacer(dest, source) ;
                return ;
            }
            if(strcmp(mot,dernier) == 0   &&   dest == NULL)
            {
                if (!courant->estDossier)     //Si destination n'est pas un dossier, on ne peut rien y ajouter  
                {
                    printf("Erreur: %s (destination) n'est pas un dossier\n", courant->nom) ;
                    return ;
                } 
                if(chercher(courant, dernier) != NULL)
                {
                    printf("Erreur: ce nom de fichier existe déjà\n") ;
                    return;
                }
                if(courant->perm[1] != 'w' || courant->perm[2] != 'x')
                {
                    printf("Permission non accordée\n") ;
                    return ;
                }
                strcpy(source->nom, dernier) ;    //renommer
                deplacer(courant, source) ;
                return ;
            }
            if (dest == NULL)       //Si destination introuvable
            {
                printf("Erreur: %s (destination) introuvable\n", mot) ;
                return ;
            }

            courant = dest ;
            mot = strtok(NULL, "/") ;
        }
        
    }
}

 /**
 *  @fn
  */  
void deplacer(fichier* dest, fichier* source)
{
        //effacer la vraie source
    fichier *copie_source = source ;
    fichier *temporaire = source->parent->premierfils ;
    fichier *prec = NULL ;
    while(temporaire != NULL)    //Parcourir les contenus du dossier afin de trouver le fichier à supprimer
    {
        if(strcmp(temporaire->nom, source->nom) == 0)
        {
            
            if (prec != NULL) 
            {
                prec->frereSuivant = temporaire->frereSuivant ;
            }
            else 
            {
                source->parent->premierfils = temporaire->frereSuivant ;
            }
                
            break ;
        }
        prec = temporaire ;
        temporaire = temporaire->frereSuivant ;
    }

    copie_source->parent = dest ;
    if(dest->premierfils == NULL)   //Si il n'y pas déjà un premier fils dans le dossier  
    {
        dest->premierfils = copie_source ;
        copie_source->frereSuivant = NULL ;
    }
    else
    {
        fichier *temp1 = dest->premierfils ;   
        while(temp1->frereSuivant != NULL)     //Parcourir dans les frères suivants si il y a déjà des contenus dans le dossier
        {
            temp1 = temp1->frereSuivant ;
        }
        temp1->frereSuivant = copie_source ;
        copie_source->frereSuivant = NULL ;
    }
      
}




/** 
* @fn chercher
* @arg parent: le dossier où on fait la recherche
       nom: nom de dossier ou fichier à chercher
* @brief chercher un fichier ou dossier  
*/
fichier* chercher(fichier *courant, char *nom)
{
    fichier *tmp ;
    tmp = courant->premierfils ;     
    while(tmp != NULL)          //Parcourir dans les fils du répértoire
    {
        if(strcmp(tmp->nom,nom) == 0)
        {
            return (tmp) ;
        }
        tmp = tmp->frereSuivant ;
    }
    return (NULL) ;
}


/** 
* @fn my_rm
* @arg parent: le dossier où on est maintenant
       nom: nom de ficheir à supprimer
* @brief supprimer seulement un fichier 
*/
void my_rm(fichier *racine, fichier *courant, char *nom)
{ 
    if(strcmp(nom,"/") == 0)
    {
        printf("Impossible de supprimer racine\n") ;
        return ;
    }
    char *dir , *dernier ;
    char *mot ;
    mot = malloc(strlen(nom) *sizeof(char)) ;
    int compt = 0 ;
    fichier* temoin ;
    fichier *parent ;
    char *suppr ;
    strcpy(mot, nom) ;
    dir = strtok(mot, "/") ;    //Vérifier si le nom de dossier est un répértoire absolu (exemple:/.../.../..../) ou un seul mot         
    while(dir != NULL)   //Parcourir le répértoire absolu
    {
        compt++ ;
        dernier = dir ;    
        dir = strtok(NULL, "/") ;
    }
    if (compt == 1)   //Si on ne voit pas un répértoire absolu mais un seul mot
    {
        parent = courant ;
        suppr = dernier ;
    }
    else
    {
        if (nom[0] == '/')      //Si le répértoire absolu se commence par un "/" , alors on va parcourir depuis la racine 
        {
            parent = racine; 
        }
        else    //Sinon parcourir depuis le répértoire courant
        {
            parent = courant ;
        }
        dir = strtok(nom, "/") ;
        if(strcmp(dir,"..") == 0)
        {
            //strcpy(dir,courant->parent->nom) ;
            dir = strtok(NULL, "/") ;
        }
        if(strcmp(dir,".") == 0)
        {
            //strcpy(dir,courant->nom) ;
            dir = strtok(NULL, "/") ;
        }
        while(dir != NULL)     //Parcourir la chaine 
        {
            if (strcmp(dir, dernier) == 0)
            {
                suppr = dernier ;
                break ;
            }

            if(dir != dernier)
            {
                parent = chercher(parent, dir) ;    //Chercher les dossiers un par un si ils existent
            }

            if(parent == NULL)     //Si on ne trouve pas, on envoie un message d'erreur: destination introuvable
            {
                printf("Erreur: %s : aucun dossier de ce nom\n", dir) ;
                return  ;
            }
                
            dir = strtok(NULL, "/") ;
        }
    
    }

    if (courant->perm[1] != 'w' || courant->perm[2] != 'x')
    {
        printf("rm : %s: Permission non accordée\n", nom) ;
        return ;
    }
    fichier *temp = parent->premierfils ;
    fichier *prec = NULL ;
    while(temp != NULL)    //Parcourir les contenus du dossier afin de trouver le fichier à supprimer
    {
        if(strcmp(temp->nom, suppr) == 0)
        {
          
            if(temp->estDossier)     //On envoie "erreur" si il est dossier
            {
                printf("Erreur: %s est un dossier\n", temp->nom) ;
                return ;
            }
            if (prec != NULL) 
            {
                prec->frereSuivant = temp->frereSuivant;
            }
            else 
            {
                parent->premierfils = temp->frereSuivant;
            }
             
            free(temp) ;
            
            return ;
        }
        prec = temp ;
        temp = temp->frereSuivant ;
    }
    printf("Erreur: %s n'existe pas\n", suppr) ;
}

/** 
* @fn my_mkdir_p
* @arg racine: un répère principal
       parent: le dossier où on est maintenant
       nom: nom de dossier à creer
* @brief creer recursivement des dossiers 
*/
void my_mkdir_p(fichier *racine, fichier *parent, char *nom)
{
    char *dir ;
    fichier *courant ; 
    dir = strtok(nom, "/") ;
    while(dir != NULL) 
    {
        //my_mkdir(racine, parent, dir, statut) ;
        courant = my_cd(racine, parent, dir) ;
        parent = courant ;
        dir = strtok(NULL, "/") ;
    }
}


/** 
* @fn my_mkdir 
* @arg racine: répère principal dans l'arborescence  
       courant: on cree le dossier là-dans
       nom: nom de dossier
* @brief creer un fichier dans un dossier 
*/
void my_mkdir(fichier *racine, fichier *courant, char *nom,int statut)
{
    if(strcmp(nom,"/") == 0)
    {
        printf("Erreur: impossible de créer racine\n") ;
        return ;
    }
    char *dir , *dernier ;
    char *mot ;
    mot = malloc(strlen(nom) *sizeof(char)) ;
    fichier *temoin ;
    int compt = 0 ;
    fichier *parent ;
    strcpy(mot, nom) ;
    dir = strtok(mot, "/") ;    //Vérifier si le nom de dossier est un répértoire absolu (exemple:/.../.../..../) ou un seul mot         
    while(dir != NULL)   //Parcourir le répértoire absolu
    {
        compt++ ;
        dernier = dir ;    
        dir = strtok(NULL, "/") ;
    }
    if (compt == 1)   //Si on ne voit pas un répértoire absolu mais un seul mot
    {
       temoin = chercher(courant, dernier) ;    //Vérifier si existe déjà
        if(temoin == NULL)
        {
            if(courant->perm[1] != 'w' || courant->perm[2] != 'x')
            {
                printf("Permission non accordée\n") ;
                return ;
            }
            ajouter_enfant(courant, dernier, 1, statut) ;
        }
        else
        {
            printf("Erreur: le fichier existe déjà\n") ;
        }
        return ;
    }
    if (nom[0] == '/')      //Si le répértoire absolu se commence par un "/" , alors on va parcourir depuis la racine 
    {
        parent = racine; 
    }
    else    //Sinon parcourir depuis le répértoire courant
    {
        parent = courant ;
    }
    if (compt > 1)     //Si le nom de dossier est un répértoire absolu (exemple:/.../.../..../)  
    {
        dir = strtok(nom, "/") ;
       
        while(dir != NULL)     //Parcourir la chaine 
        {
            if (strcmp(dir, dernier) == 0)
            {
                break ;
            }

            if(dir != dernier)
            {
                parent = chercher(parent, dir) ;    //Chercher les dossiers un par un si ils existent
            }

            if(parent == NULL)     //Si on ne trouve pas, on envoie un message d'erreur: destination introuvable
            {
                printf("Erreur: %s : aucun dossier de ce nom\n", dir) ;
                return  ;
            }
            
            dir = strtok(NULL, "/") ;
        }
    }
    temoin = chercher(parent, dernier) ;    //Vérifier si existe déjà
    if(temoin == NULL)
    {
        if(parent->perm[1] != 'w' || parent->perm[2] != 'x')
        {
            printf("Permission non accordée\n") ;
            return ;
        }
        ajouter_enfant(parent, dernier, 1, statut) ;
    }
    else
    {
        printf("Erreur: le dossier existe déjà\n") ;
    }
}

void my_pwd(fichier *rep_actuel)
{
    char *chemin;

    chemin = calloc(32 , sizeof(char));
    get_pwd( rep_actuel , chemin );
    printf("%s\n", chemin);

    free(chemin);
}
/**
 *  @fn void my_echo(char **ligne_c)
 *  @brief c'est comme la commande 'echo' mais en devellopement
 */
void my_echo(char **ligne_c)
{
    int i;

    i = 1;
    while(ligne_c[i][0] != '\0')
    {
        printf("%s " , ligne_c[i]);
        i++;
    }
    printf("\n");
}


void my_chmod(fichier* racine, fichier* courant, char* option, char* nom)
{
    if(strcmp(nom,"/") == 0)
    {
        printf("Erreur: impossible de modifier les permissions de racine\n") ;
        return ;
    }
    char *dir ;
    fichier *tmp , *dernier;
    if (nom[0] == '/')      //Si le nom se commence par un "/", donc parcourir depuis la racine   
    {
        tmp = racine; 
    }
    else    //Sinon, parcourir depuis le répértoire courant 
    {
        tmp = courant ;
    }
    dir = strtok(nom,"/") ;    //Si le destination se fait dans un répértoire absolu
    if(strcmp(dir,".") == 0)
    {
        dir = strtok(NULL,"/") ;
    }
    if(strcmp(dir,"..") == 0)
    {
        if(courant->parent != NULL)
        {
            tmp = courant->parent ;
        }
        else
        {
            tmp = courant ;
        }
        dir = strtok(NULL,"/") ;
    }
    
    while(dir != NULL)    //Parcourir la chaine
    {
        tmp = chercher(tmp,dir) ;    //Vérifier un par un si les dossiers existent   
        if (tmp == NULL)
        {
            printf("Erreur %s : aucun dossier de ce nom\n", dir) ;
            return ;
        }
        dernier = tmp ;
        dir = strtok(NULL, "/") ;
    }

    if(option[0] == '+')
    {
        //On ajoute
        if(strchr(option,'r'))
        {
            dernier->perm[0] = 'r' ;
        }     
        if(strchr(option,'w'))
        {
            dernier->perm[1] = 'w' ;
        }    
        if(strchr(option,'x'))
        {
            dernier->perm[2] = 'x' ;
        } 
        if(strchr(option,'r') == NULL && strchr(option,'w') == NULL && strchr(option,'x') == NULL)
        {
            printf("Erreur: option %s incorrecte\n", option) ;
            return ;
        }        
    }
    else if(option[0] == '-')
    {
        if(strchr(option,'r'))
        {
            dernier->perm[0] = '-' ;
        }     
        if(strchr(option,'w'))
        {
            dernier->perm[1] = '-' ;
        }    
        if(strchr(option,'x'))
        {
            dernier->perm[2] = '-' ;
        } 
        if(strchr(option,'r') == NULL && strchr(option,'w') == NULL && strchr(option,'x') == NULL)
        {
            printf("Erreur: option %s incorrecte\n", option) ;
            return ;
        }    
    }
    else if(option[0] == '0')
    {
        strcpy(dernier->perm, "---") ;
    }
    else if(option[0] == '1')
    {
        strcpy(dernier->perm, "--x") ;
    }
    else if(option[0] == '2')
    {
        strcpy(dernier->perm, "-w-") ;
    }
    else if(option[0] == '3')
    {
        strcpy(dernier->perm, "-wx") ;
    }
    else if(option[0] == '4')
    {
        strcpy(dernier->perm, "r--") ;
    }
    else if(option[0] == '5')
    {
        strcpy(dernier->perm, "r-x") ;
    }
    else if(option[0] == '6')
    {
        strcpy(dernier->perm, "rw-") ;
    }
    else if(option[0] == '7')
    {
        strcpy(dernier->perm, "rwx") ;
    }
    else
    {
        printf("Erreur: option %c incorrecte\n", option[0]) ;
        return ;
    }
}


void my_ls_l(fichier* racine, fichier *courant,char *nom)
{

    fichier *repertoire ;
    if(nom[0]=='\0')
    {
        repertoire = courant ;
    }
    else if (strcmp(nom, "/") == 0)
    {
        repertoire = racine ;
    }
    else
    {
            if (strcmp(nom, "..") == 0)      //Pour remonter dans son parent
            {
                if (courant->parent) 
                {
                    repertoire = courant->parent ;  //dans son parent
                }
                else
                {
                    repertoire = courant; // reste à la racine
                }
            }

            else if (strcmp(nom, ".") == 0)   //Pour ne pas faire un déplacement
            {
                repertoire = courant ;
            }
            else 
            {
                char *dir ;
                fichier *tmp , *dernier;
                if (nom[0] == '/')      //Si le nom se commence par un "/", donc parcourir depuis la racine   
                {
                    tmp = racine; 
                }
                else    //Sinon, parcourir depuis le répértoire courant 
                {
                    tmp = courant ;
                }
                dir = strtok(nom,"/") ;    //Si le destination se fait dans un répértoire absolu
                if(strcmp(dir,".") == 0)
                {
                    dir = strtok(NULL,"/") ;
                }
                if(strcmp(dir,"..") == 0)
                {
                    if(courant->parent != NULL)
                    {
                        tmp = courant->parent ;
                    }
                    else
                    {
                        tmp = courant ;
                    }
                    dir = strtok(NULL,"/") ;
                }
                
                while(dir != NULL)    //Parcourir la chaine
                {
                    tmp = chercher(tmp,dir) ;    //Vérifier un par un si les dossiers existent   
                    if (tmp == NULL)
                    {
                        printf("Erreur %s : aucun dossier de ce nom\n", dir) ;
                        return ;
                    }
                    repertoire = tmp ;
                    dir = strtok(NULL, "/") ;
                }
            }
    }
    if(repertoire->perm[0] != 'r' || repertoire->perm[2] != 'x')
    {
        printf("Permission non accordée\n") ;
        return ;
    }
    fichier *temp = repertoire->premierfils ;
    while(temp)
    {
        if (temp->nom[0] != '.')      //On n'affiche pas un fichier si son nom se commence par un point
        {
            if (temp->estDossier)
            {
                printf("%s\t\033[1;36m%s\033[0m  \n",temp->perm,temp->nom);    
            }
            else
            {
                printf("%s\t\033[1;37m%s\033[0m  \n", temp->perm, temp->nom);
            }
        }
        
        temp = temp->frereSuivant ;
    }
    printf("\n") ;
}


void afficher_nbr_ouverture(fichier* home, int prfnd)
{
    if(home == NULL)
    {
        return ;
    }
    for(int i = 0; i<prfnd; i++)
    {
        printf("---") ;
    }
      printf("%d : %s\n", home->nombre_ouverture, home->nom ) ;
      afficher_nbr_ouverture(home->premierfils, prfnd + 1) ;   //On appelle recursivement la fonction lui-même
      afficher_nbr_ouverture(home->frereSuivant, prfnd) ; 
}

int recherche_max(fichier* home, int max)
{
    if(home == NULL)
    {
        return max;
    }
    if(strcmp(home->nom ,"AK-07") != 0 && home->nombre_ouverture > max)
    {
        max = home->nombre_ouverture ;
    } 
    max = recherche_max(home->premierfils, max) ;   //On appelle recursivement la fonction lui-même
    max = recherche_max(home->frereSuivant, max) ;
    return max ;                   
}

void favori(fichier* home,  int max)
{
    if(home == NULL)
    {
        return ;
    }
    if(strcmp(home->nom, "AK-07") != 0 && home->nombre_ouverture == max)
    {
        printf("%d : %s\n", home->nombre_ouverture, home->nom ) ;
    }
    favori(home->premierfils, max) ;   //On appelle recursivement la fonction lui-même
    favori(home->frereSuivant,  max) ; 
}

void affichage(fichier* home)
{
    int max;
    max = recherche_max(home,0) ;
   favori(home, max) ;
}

void my_ls_la(fichier* racine, fichier *courant,char *nom)
{
    fichier *repertoire ;
    if(nom[0]=='\0')
    {
        repertoire = courant ;
    }
    else if(strcmp(nom,"/") == 0)
    {
        repertoire = racine ;
    }
    else
    {
            if (strcmp(nom, "..") == 0)      //Pour remonter dans son parent
            {
                if (courant->parent) 
                {
                    repertoire = courant->parent ;  //dans son parent
                }
                else
                {
                    repertoire = courant; // reste à la racine
                }
            }

            else if (strcmp(nom, ".") == 0)   //Pour ne pas faire un déplacement
            {
                repertoire = courant ;
            }
            else 
            {
                char *dir ;
                fichier *tmp , *dernier;
                if (nom[0] == '/')      //Si le nom se commence par un "/", donc parcourir depuis la racine   
                {
                    tmp = racine; 
                }
                else    //Sinon, parcourir depuis le répértoire courant 
                {
                    tmp = courant ;
                }
                dir = strtok(nom,"/") ;    //Si le destination se fait dans un répértoire absolu
                if(strcmp(dir,".") == 0)
                {
                    dir = strtok(NULL,"/") ;
                }
                if(strcmp(dir,"..") == 0)
                {
                    if(courant->parent != NULL)
                    {
                        tmp = courant->parent ;
                    }
                    else
                    {
                        tmp = courant ;
                    }
                    dir = strtok(NULL,"/") ;
                }
                
                while(dir != NULL)    //Parcourir la chaine
                {
                    tmp = chercher(tmp,dir) ;    //Vérifier un par un si les dossiers existent   
                    if (tmp == NULL)
                    {
                        printf("Erreur %s : aucun dossier de ce nom\n", dir) ;
                        return ;
                    }
                    repertoire = tmp ;
                    dir = strtok(NULL, "/") ;
                }
            }
    }
    if(repertoire->perm[0] != 'r' || repertoire->perm[2] != 'x')
    {
        printf("Permission non accordée\n") ;
        return ;
    }
    fichier *temp = repertoire->premierfils ;
    while(temp)
    {
        
            if (temp->estDossier)
            {
                printf("%s\t\033[1;36m%s\033[0m  \n",temp->perm,temp->nom);    
            }
            else
            {
                printf("%s\t\033[1;37m%s\033[0m  \n", temp->perm, temp->nom);
            }
        
        
        temp = temp->frereSuivant ;
    }
    printf("\n") ;
} 
