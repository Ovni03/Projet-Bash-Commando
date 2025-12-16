/**
 * @file mission.c
 * @brief : Fonction pour gérer les missions du jeu
 * @author : RAKOTOARIVONY Zo Mamitiana Olivier
 * @date : 03 déc 2025
 */
#ifndef __MISSION__
#define __MISSION__

    typedef struct Mission // Structure pour gérer les missions
    {
        int debloqué ;  // 1 si la mission est débloqué , 0 sinon
        int terminé  ;  // 1 si la mission est términée , 0 sinon
        char titre[50] ; // Titre du mission
        int point ; // Points attribués pour la mission
        char indice[100]; // Indices pour la mission
    }Mission;
    

    #define NOMBRE_MISSION 14 // Nombre du mission dans le jeu
    #include "../include/arborescence.h" // Pour la struct fichier

    /**
     * @fn intro
     * @brief Affiche l'introduction du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu
    */
    void intro(Mission *mission , fichier *racine);
    /**
     * @fn réinitialisation_mission
     * @brief Réinitialise les missions
     * @param mission : Structur mission
    */
    void réinitialisation_mission( Mission *mission );
    /**
     * @fn sauvegarde_dernier_partie
     * @brief Sauvegarde la dernière partie jouée
     * @param mission : Structur mission
     * @param rang_mission_actuel : Rang de la mission actuelle
    */
    void sauvegarde_dernier_partie( Mission *mission , int rang_mission_actuel );
    /**
     * @fn load_struct_mission
     * @brief Charge la structure de mission depuis un fichier
     * @param mission : Structur mission
    */
    void load_struct_mission(Mission *mission );
    /**
     * @fn mission_1
     * @brief Mission 1 du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence
     */
    void mission_1( Mission *mission , fichier *racine );
    /**
     * @fn mission_2
     * @brief Mission 2 du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu    
    */
    void mission_2( Mission *mission , fichier *racine );
    /**
     * @fn mission_3
     * @brief Mission 3 du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu    
    */
    void mission_3( Mission *mission , fichier *racine );
    /**
     * @fn mission_4
     * @brief Mission 4 du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu    
    */  
    void mission_4(Mission *mission , fichier *racine );
    /**
     * @fn mission_5
     * @brief Mission 5 du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu    
    */
    void mission_5(Mission *mission , fichier *racine );
    /**
     * @fn mission_6
     * @brief Mission 6 du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu    
    */
    void mission_6(Mission *mission , fichier *racine ) ; // cree un fichier
    /**
     * @fn mission_8
     * @brief Mission 8 du jeu
     * @param mission : Structur mission
    */
    void mission_8(Mission *mission , fichier *racine ) ; // Deplacer avec cp 
    /**
     * @fn mission_7
     * @brief Mission 7 du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence
    */
    void mission_7(Mission *mission , fichier *racine ) ;
    /**
     * @fn mission_9
     * @brief Déplacer avec mv
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     */
    void mission_9(Mission *mission , fichier *racine ) ;
    /**
     * @fn mission_10
     * @brief Afficher du texte avec echo
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     */
    void mission_10(Mission *mission , fichier *racine ) ;
    /**
     * @fn mission_11
     * @brief Afficher le contenu d'un fichier avec cat
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     */
    void mission_11(Mission *mission , fichier *racine ) ;
    /**
     * @fn mission_12
     * @brief Afficher le début d'un fichier avec head
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     */
    void mission_12(Mission *mission , fichier *racine ) ;
    /**
     * @fn mission_13
     * @brief Afficher la fin d'un fichier avec tail
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     */
    void mission_13(Mission *mission , fichier *racine ) ;

    /**
     * @fn verification_reussite
     * @brief Vérifie la réussite d'une mission
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     * @param reuissite : Booléen de réussite
     * @param num_mission : Numéro de la mission
    */
    void verification_reussite(Mission *mission , fichier *racine , bool reuissite , int num_mission );

    /**
     * @fn mission_14
     * @brief Changer les permissions d'un fichier ou d'un dossier avec chmod
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     */
    void mission_14(Mission *mission , fichier *racine) ; // Chmod

    /**
     * @fn verification_mission
     * @brief Vérifie si une mission est accomplie
     * @param mission : Structur mission
     * @param rang_mission : Rang de la mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     * @param courant : Répertoire courant
    */
    bool verification_mission(Mission *mission , int rang_mission , fichier *racine , char* courant);
    /**
     * @fn prompt_mission
     * @brief Lance le prompt pour une mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu 
     * @param commande : Commande à exécuter pour réussir la mission
     * @param titre : Titre de la mission
     * @param chemin : Chemin du fichier de la mission
     * @param rep_courant : Répertoire courant
     * @param miss_actuelle : Structur de la mission actuelle
     * @param rang_mission : Rang de la mission
     * @param autoverifi : Booléen d'auto-vérification
    */
    bool prompt_mission(fichier *racine , char *commande , char *titre , char *chemin , char *rep_courant  , Mission *miss_actuelle , int rang_mission , int autoverifi);
    /**
     * @fn map_initialisation
     * @brief Initialise la carte du jeu
     * @param mission : Structur mission
     * @param racine : Structur fichier ( racine ) l'arboressence du jeu
    */
    void map_initialisation(Mission *mission , fichier *racine);


#endif