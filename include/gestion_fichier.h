/**
 *  @file   gestion_fichier.h
 *  @brief  regroupe tous les fonctions qui manipule les fichiers
 *  @author Valisoa
 *  @date   29 Août 2025
 *
 *      toutes les manipulartions de fichier comme les enregistrements et les affichages sont regroupés
 *      dans celle-ci ; ces fonctions sont definies dans gestion_fichier.c
 */
 #ifndef GEST_FIC
 #define GEST_FIC
    #include "../include/mission.h"

    /**
     *  @fn void head_fichier(char *nom_fichier , int max)
     *  @param  nom_fichier le nom de la fichier
     *  @param  max le nombre de ligne à afficher
     *  @brief  il affiche des premiers lignes d'une fichier entrer en parametre
     */
    void head_fichier(char *nom_fichier , int max);
    /**
     *  @fn int affiche_ligne(char *chemin , int num_ligne , char *emplacement)
     *  @param  chemin le chemin vers la fichier
     *  @param  num_ligne le numero de ligne à afficher
     *  @param  emplacement le variable qui va contenir la ligne voulue
     *  @return renvoie une valeur negative si le fichier n'est pas existant
     *  @brief  Lire une ligne precise dans un fichier depart de la fin si negative
     */
    int affiche_ligne(char *chemin , int num_ligne , char *emplacement);
    /**
     *  @fn int my_man(char *nom_commande)
     *  @param  nom_commande le nom de la commmande à afficher l'aide
     *  @return une valeur negative en cas d'erreur
     *  @brief  Il affiche une page d'aide pour une commande entrer en argument
     */
    int my_man(char *nom_commande);
    /**
     *  @fn void qui_est_meilleur(int n_miss)
     *  @param  n_miss le numero de la mission
     *  @brief cette fonction affche qui a le meilleur temps concernant une mission entrer en argument
     */
    void qui_est_meilleur(int n_miss);
    /**
     *  @fn void voir_indice(Mission *mission)
     *  @param  mission l'adresse du mission que l'on veut voir l'indice
     *  @brief  Elle affiche une petite aide apropos d'une mission en decrementant le score du joueur actuelle
     */
    void voir_indice(Mission *mission);

    /**
     *  @fn int lire_histoire()
     *  @return une valeur negatif si le fichier histoire est vide ou inexistant
     *  @brief cette fonction lit le fichier historique des commandes
     */
    int lire_histoire();
    /**
     *  @fn void cat_fichier(char *nom_fichier , bool num)
     *  @param  nom_fichier nom de la fichier à afficher
     *  @brief  afficher le  fichier entrée en parametre
     */
    void tail_fichier(char *nom_fichier , int min);
    /**
     *  @fn void cat_fichier(char *nom_fichier , bool num)
     *  @param  nom_fichier le nom de fichier à afficher
     *  @param  num affiche le numero de ligne si true est la valeur de celle ci
     *  @brief  afficher le contenu d'un fichier
     */
    void cat_fichier(char *nom_fichier , bool num);
    /**
     *  @fn int my_history(bool effacer)
     *  @param effacer effacer le contenu historique si true
     *  @return une valeur negative si la lecture a echouée
     *  @brief afficher ou effacer les historiques de ligne de commande
     */
    int my_history(bool effacer);

    /**
     *  @fn insert_fin
     *  @param  nom_fichier nom de la fichier
     *  @brief  inserer une chaine de caractere à la fin de fichier
     */
    void insert_fin(char *nom_fichier, char *chaine_caractere);
    /**
     *  @fn void echanger(char *chaine1 , char *chaine2)
     *  @param  chaine1 la premiere chaine à echanger de contenus avec l'autre 
     *  @param  chaine2 l'autre chaine 
     *  @brief  cette fonction change le contenu de chaine1 avec le contenus de chaine2 
     */
    void echanger(char *chaine1 , char *chaine2);
    /**
     *  @fn int histoire_s(char option)
     *  @param  option l'option à utiliser
     *  @return renvoie une valeur negative en cas d'erreur
     *  @brief  Il lit l'historique des commandes en fonction de l'option en argument
     */
    int histoire_s(char option);
    
#endif 
