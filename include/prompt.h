/**
 *  @author Valisoa
 *  @file   prompt.h
 *  @brief regroupement des fonctions dans prompt.c
 */
#ifndef DECL
#define DECL

    /**
     *  @def    SORTIR
     *  @brief  c'est la commande pour sortir du petit terminale
     */
    #define SORTIR "exit"
    /**
     *  @include    #include "../include/arborescence.h"
     *  @brief  utile pour la reconnaissance de la structure fichier*
     */
    #include "../include/arborescence.h"
    /**
     *  @include    #include <stdbool.h>
     *  @brief  c'est la pour l'utilisation des valeurs booléennes
     */
    #include <stdbool.h>

    /**
     *  @def    LIGNE    
     *  @brief   c'est la taille max defini pour les lignes .   
     */
    #define LIGNE 100
    /**
     *  @def    MOT 
     *  @brief   c'est la taille max defini pour les mots .   
     */
    #define MOT 30
    /**
     *  @struct ligne_commande
     *  @brief  une representation d'une ligne de commande
     *  une ligne de commande doit contenir evidement la commande et les parametres ; options qui peut être facultatif ou pas
     */
    struct ligne
    {
        char *commande;   /*!< la commande principale , c'est à dire le premier mot de la ligne de commande */
        char **option;    /*!< la tableau des options dont le dernier doit être null */
        char **argument;  /*!<  la tableau des arguments dont le dernier doit être null*/
    };
    typedef struct ligne ligne_commande;
    /**
     *  @fn     void affiche_prompt(fichier *repertoire_actuel)
     *  @param  repertoire_actuel l'adresse du repertoire actuel
     *  @brief  il affiche le ligne de prompt où l'on insere du commande
     */
    void affiche_prompt(fichier *repertoire_actuel);

    /**
     *  @fn int prend_ligne_command(char **emplacement, int taille)
     *  @param  emplacement l'endroit ou l'on va placer la ligne recuperer
     *  @param  taillle la taille maximale de l'emplacement
     *  @param  repertoire_actuel l'adresse du repertoire où on est
     *  @brief  prend une ligne en l'introduisant dans une tableau 2D pour mieux le manipuler
     */
    int prend_ligne_command(char **emplacement, int taille , fichier *repertoire_actuel);

    /**
     *  @fn     int getNomJoueur(char *emplacement)
     *  @param  emplacement l'emplacement du mot prise
     *  @brief  trouver le nom du joueur actuelle et puis l'inserer dans emplacement
     */
    int getNomJoueur(char *emplacement);

    /**
     *  @fn fichier *execute(char **commande , fichier *repertoire_actuel , int nbMot )
     *  @param  commande La ligne entrer par l'utilisateur
     *  @param  repertoire_actuel l'adresse du repertoire où l'on travaille actuellement
     *  @param  nbMot le nombre de mot prise dans commande qui est de type char**
     *  @return l'adresse du repertoire de travaille qui a peut etre changé durant le prompt
     *  @brief  execution du commande entrée en argument
     */
    fichier *execute(char **commande , fichier *repertoire_actue , int nbMot);

    /**
     *  @fn void get_pwd(fichier *adresse, char *emplacement)
     *  @param  adresse l'emplacement actuelle dans l'arborescence
     *  @param  emplacement la variable ou stocker le resulatat
     *  @brief  trouver le chemin du repertoire actuelle et puis l'inserer dans emplacement
     */
    void get_pwd(fichier *adresse, char *emplacement);
    /**
     *  @fn int get_point()
     *  @return il return le nombre de point de l'utilisateur actuel
     *  @brief  c'est pour savoir le point que le joueur actuel possède
     */
    int get_point();
    /**
     *  @fn void increment_score(int actuPTS)
     *  @param  actuPTS le nombre de point à ajouter au score actuel
     *  @brief  c'est pour augmenter le nombre de score du joueur actuel
     */
    void increment_score(int actuPTS);

    /**
     *  @fn int decrement_score(int penalite)
     *  @param  penalite c'est le nombre de point à enlever dans le nombre de score
     *  @return renvoie une entier negatif si le score est insuffisant ou n'existe pas encore
     *  @brief  c'est une fonction qui fait decrementer le nombre de score d'une joueur en jeu
     */
    int decrement_score(int penalite);
    /**
     *  @fn int my_get_ligne(char *mobile , int taille_Max)
     *  @param  mobile  l'endroit où placer la ligne recupérer encore en chaine de caractère
     *  @param  taille_Max  la longueur maximal que l'on peut insérer dans mobile
     *  @param  repertoire_actuel l'adresse du repertoire où on est
     *  @return envoie 1 s'il a reussie
     *  @brief  cette fonction a pour but de prendre la ligne entrée sans occupé 
     *des touches indésirable comme les touches de directions
     */
    int my_get_ligne(char *mobile , int taille_Max , fichier *repertoire_actuel);

int verifier(char *chaine);
    /**
     *  @fn int rapport_mission(int num_mission , bool reussie)
     *  @param num_mission c'est la numero de la mission dont on va faire le rapport
     *  @param  reussie possede une valeur true si la mission est reussie
     *  @return 1 si l'ecriture s'est bien passe ou -1  si non
     *  @brief  celui ci ecrit le joueur , temps de jeu , reussie ou pas et numero de mission dans le fichier d'enregistrement
     */
    int rapport_mission(int num_mission , bool reussie);
    /**
     *  @fn  void vide_ligne(int droite , char *chaine)
     *  @param  droite la position par rapport à la plus droite
     *  @param  chaine la chaine de caractere de la ligne à vider
     *  @brief  c'est une simple fonction qui permet de vider la ligne de commande actuelle
     */
    void vide_ligne(int droite , char *chaine);
    /**
     *  @fn void efface_cara(char *chaine ,  int *position)
     *  @param chaine la chaine de caractere dont on veut supprimer un caractere
     *  @param  position    la position du caractere dans la chaine
     *  @brief  On lui donne une chaine de caractere et une position ; Il supprime le caractere qui se trouve à cette position
     */
    void efface_cara(char *chaine ,  int *position);
    /**
     *  @fn int taille_de(char *ligne)
     *  @param  ligne la chaine de caractere à compter
     *  @return le nombre de caractere de la chaine entrer en argument
     *  @brief  elle ne compte pas les caracteres comme strlen ; elle donne la taille en affichage
     */
    int taille_de(char *ligne);
    /**
     *  @fn int prend_mot_dans(int position , char *chaine , char *emplacement)
     *  @param  position    la position du mot à prendre
     *  @param  chaine      la chaine de caractere où extracter le mot
     *  @param  emplacement l'adresse de l'emplacement de la chaine extractée
     *  @return renvoie 0 si tous s'est bien passé
     */
    int prend_mot_dans(int position , char *chaine , char *emplacement);
    /**
     *  @fn int auto_completion(char *chaine , fichier *repertoire_actuel , int *nb_Tab , int *pos_droite)
     *  @param  chaine la chaine à completer
     *  @param  repertoire_actuel   l'adresse du repertoire actuelle
     *  @param  nb_Tab  nombre de tabulation deja effectuer
     *  @param  pos_droite la position actuelle par rapport à la position la plus à droite
     *  @return la taille de la chaine en affichage ; donc reussi si different de zero
     *  @brief  c'est la fonction principale de l'auto-completion sur ligne de commande
     */
    int auto_completion(char *chaine , fichier *repertoire_actuel , int *nb_Tab , int *pos_droite);
    /**
     *  @fn void touche_suppr(int *position , int *pos_droite , char *chaine)
     *  @param  position la position dans le chaine de caractere
     *  @param  pos_droite  la position de curseur en comptant depuis la droite
     *  @param  chaine le ligne où l'on veut supprimer une caractere
     *  @brief  c'est la fonction associer à la touche suppr
     */
    void touche_suppr(int *position , int *pos_droite , char *chaine);
    /**
     *  @fn void touches_gauche_droite(int *indice , int *pos_droite , char *chaine , int gauche)
     *  @param  indice  la position dans la ligne actuelle
     *  @param  pos_droite  la position par rapport à la position la plus à droite
     *  @param  chaine la ligne actuelle
     *  @param  gauche  valeur si 0 si touche droite et 1 si touche gauche
     *  @brief  c'est la fonction associer à la touche gauche et droite
     */
    void touches_gauche_droite(int *indice , int *pos_droite , char *chaine , int gauche);
    /**
     *  @fn void touches_haut_bas(int *indice , int *position , int *droite , char *chaine , char *tmp , int haut)
     *  @param  indice position dans la chaine
     *  @param  position    position actuelle dans les visions des historiques des commandes
     *  @param  droite      position par rapport à la plus droite
     *  @param  chaine      la chaine de caractere contenant la ligne
     *  @param  tmp         la chaine ou l'on stocke une ligne temporairement ; ici la ligne la plus bas est enregistrer
     *  @param  haut        1 si touche haut et 0 si touche bas
     *  @brief  la fonctione associer à la touche haut et bas
     */
    void touches_haut_bas(int *indice , int *position , int *droite , char *chaine , char *tmp , int haut);
    /**
     *  @fn void inserer_mot_dans(int position , char *chaine , char *insertion)
     *  @param  position    la position de l'insertion dans la chaine
     *  @param  chaine      la chaine d'insertion
     *  @param  insertion   la chaine à inserer
     *  @brief  c'est pour inserer une mot dans une position precise dans une chaine ; separateur = espace
     */
    void inserer_mot_dans(int position , char *chaine , char *insertion);
    /**
     *  @fn int trouver_avant(char *chaine , char *recherche)
     *  @param  chaine la chaine de recherche
     *  @param  recherche la chaine à rechercher
     *  @return Elle renvoie la taille de chaine semblable
     *  @brief  Elle permet de trouver si la chaine recherche est le debut de chaine ; les espaces sont ignorés
     */
    int trouver_avant(char *chaine , char *recherche);
    /**
     *  @fn void affiche_commande(int *indice , char caractere ,char *chaine ,int  droite)
     *  @param  indice  la position d'insertion dans la chaine
     *  @param  caractere   le caractere à inserer dans la chaine
     *  @param  chaine  la chaine où l'on va inserer une caractere
     *  @param  droite  la position actuelle en se referant de la position la plus à droite
     *  @brief  Elle permet d'inserer une caractere dans une chaine et d'affcher la chaine
     */
    void affiche_commande(int *indice , char caractere ,char *chaine ,int  droite);
    /**
     *  @fn int compter_mot(char *chaine)
     *  @param  chaine la chaine dont on veut compter le nombre de mot
     *  @return le nombre de mot dans chaine  
     *  @brief  fonction qui compte le nombre de mot dans une chaine de caractere
     */
    int compter_mot(char *chaine);
    /**
     *  @fn int commande_option(ligne_commande *ligne , char **liste , int nb_mot)
     *  @param  ligne l'adresse du structure où l'on stockera la representation de la ligne de commande
     *  @param  liste la ligne actuelle deja separer mot par mot ( dans une tableau mot )
     *  @param  nb_mot  le nombre de mot dans liste
     *  @return 0 si tous s'est bien passer 
     *  @brief  cette fonction represente une ligne de commande dans une structure ligne_commande 
     */
    int commande_option(ligne_commande *ligne , char **liste , int nb_mot);
    /**
     *  @fn void inserer(char caractere , int position , char *chaine);
     *  @param  caractere le caractere à inserer dans une chaine
     *  @param  position la position de l'insertion
     *  @param  chaine  la chaine où l'on va inserer caractere
     *  @brief  Elle permet d'inserer une caractere dans une chaine à une position preciser en argument 
     */
    void inserer(char caractere , int position , char *chaine);

#endif