#ifndef __AFFICHE__
#define __AFFICHE__

    #include <stdbool.h> // Pour le type booléen
    
    /**
     * @fn screen_clear
     * @brief Fonction pour nettoyer l'écran
    */
    void screen_clear();
    /**
     * @fn afficher_tableau_avec_titre_position_exacte
     * @brief Affiche un fichier texte dans un tableau ncurses à une position exacte
     * @param titre : Titre du tableau
     * @param nom_fichier : nom du fichier à afficher
     */
    void afficher_tableau_avec_titre_position_exacte(const char *titre, const char *nom_fichier);
    /**
     * @fn afficher_tableau_fichier
     * @brief Affiche un fichier texte dans un tableau ncurses
     * @param titre : Titre du tableau
     * @param nom_fichier : nom du fichier à afficher
    */
    void afficher_tableau_fichier(const char *titre, const char *nom_fichier);
    /**
     * @fn affiche_fichier_tableau_curse_position_exacte
     * @brief Affiche un fichier texte dans un tableau ncurses à une position exacte
     * @param titre : Titre du tableau
     * @param nom_fichier : nom du fichier à afficher
    */
    void affiche_fichier_tableau_curse_position_exacte(const char *titre , const char *nom_fichier);

    /**
     * @fn felicitation
     * @brief Affiche un message de félicitation
    */
    bool felicitation();
    /**
     * @fn echec
     * @brief Affiche un message d'échec
    */
    bool echec();
    void affiche_fichier(char *nom_fichier ) ;
    
#endif