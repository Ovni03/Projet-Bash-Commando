/**
 * @file main.c
 * @brief : Fonction principale du jeu Bash Commando
 * @author : RAKOTOARIVONY Zo Mamitiana Olivier
 * @date : 03 déc 2025
 */
#ifndef __MENU__
#define __MENU__

#include "../include/affichage.h" // Pour l'affichage
#include "../include/sécure.h"  // Pour la securité
#include "../include/mission.h" // Pour la gestion des missions

    /**
     * @fn menu_pricipale
     * @brief Fonction qui regle le menu principale du jeu (affichage , selection partie et continuer partie)
     * @param 1 : Struct mission
     * @param 2 : Struct fichier
     */
    void menu_pricipale(Mission *mission , fichier *racine );
    /**
     * @fn load_game
     * @brief Charger la mission choisie par l'user a travers une tableau de fonction
     * @param 1 : struct mission
     * @param 2 : Rang du mission que l'user souhaite joué
     * @param 3 : struct fichier
    */
    void load_game(Mission *mission , int rang_du_dernier_mission , fichier *racine);
    /**
     * @fn ptit_prompt
     * @brief Affiche le prompt avec le nom de l'utilisateur ou "USER" par défaut
     * @param 1 : nom de l'utilisateur
    */
    void ptit_prompt(char *nom);
    /**
     * @fn charger_partie
     * @brief Affiche tout les partie débloqué laisse l'user choisir la partie qu'il veut rejouer
     * @param 1 : struct mission
    */
    int charger_partie(Mission *mission);
    /**
     * @fn quit_game
     * @brief Fonction pour quitter le jeu
     * @param 1 : struct mission
    */
    void quit_game(Mission *mission);
    /**
     * @fn loging
     * @brief Fonction pour gérer la connexion de l'utilisateur
    */
    void loging();
    /**
     * @fn dossierExiste
     * @brief Vérifie si un dossier existe
     * @param 1 : chemin du dossier
    */
    int dossierExiste(const char *chemin);

#endif 
