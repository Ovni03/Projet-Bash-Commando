#ifndef declaration
#define declaration
struct fichier 
{
      char nom[20] ;
      int estDossier ;
      int estProtege ;
      int nombre_ouverture;
      char perm[10] ;
      char **contenu ;
      struct fichier* parent;
      struct fichier* premierfils ;
      struct fichier* frereSuivant ;      
} ;
typedef struct fichier fichier ;
/**
 * @fn  fichier* creer(fichier *parent, char *nom, int estDossier, int statut) ;
 * @brief  Crée un nouveau fichier ou dossier.
 * @param parent Pointeur vers le dossier parent.
 * @param nom Nom du fichier ou dossier à créer.
 * @param estDossier Indicateur si c'est un dossier (1) ou un fichier (0).
 * @param statut Statut de protection du fichier ou dossier.
 * @return Pointeur vers le nouveau fichier ou dossier créé.
 */
fichier* creer(fichier *parent, char *nom, int estDossier, int statut) ;
/**
 * @fn  void ajouter_enfant(fichier *parent, char *nom, int nombre, int statut) ;
 * @brief  Ajoute un enfant (fichier ou dossier) à un dossier parent.
 * @param parent Pointeur vers le dossier parent.
 * @param nom Nom de l'enfant à ajouter.
 * @param nombre Indicateur si c'est un dossier (1) ou un fichier (0).
 * @param statut Statut de protection de l'enfant.
 */
void ajouter_enfant(fichier *parent, char *nom, int nombre, int statut) ;
/**
 * @fn  void my_mkdir(fichier *racine, fichier *courant, char *nom,int statut) ;
 * @brief  Crée un nouveau dossier dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du dossier à créer.
 * @param statut Statut de protection du dossier.   
 */
void my_mkdir(fichier *racine, fichier *courant, char *nom,int statut) ;
/**
 * @fn  void afficher(fichier* home, int prfnd) ;
 * @brief  Affiche la structure de l'arborescence des fichiers et dossiers.
 * @param home Pointeur vers le répertoire racine.
 * @param prfnd Profondeur actuelle dans l'arborescence.    
 */
void afficher(fichier* home, int prfnd) ;
/**
 * @fn  void my_ls(fichier* racine, fichier *courant,char *nom) ;
 * @brief  Liste les fichiers et dossiers dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du répertoire à lister.
 */
void my_ls(fichier* racine, fichier *courant,char *nom) ;
/**
 * @fn  void my_touch(fichier *racine, fichier *courant, char *nom, int statut) ;
 * @brief  Crée un nouveau fichier dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du fichier à créer.
 * @param statut Statut de protection du fichier.   
 */
void my_touch(fichier *racine, fichier *courant, char *nom, int statut) ;
/**
 * @fn  void my_rmdir(fichier *racine,fichier *courant, char *nom) ;
 * @brief  Supprime un dossier dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du dossier à supprimer.
 */
void my_rmdir(fichier *racine,fichier *courant, char *nom) ;
/**
 * @fn  fichier* my_cd(fichier *racine,fichier *courant, char *nom) ;
 * @brief  Change le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du répertoire cible.
 * @return Pointeur vers le nouveau répertoire courant.
 */
fichier* my_cd(fichier *racine,fichier *courant, char *nom) ;
/**
 * @fn  void initialiser(fichier *racine) ;
 * @brief  Initialise l'arborescence des fichiers et dossiers avec une structure de base.
 * @param racine Pointeur vers le répertoire racine.
 */
void initialiser(fichier *racine) ;
/**
 * @fn  void my_cp(fichier *racine, fichier *parent, char *nom, char *destination) ;
 * @brief  Copie un fichier ou dossier vers une destination.
 * @param racine Pointeur vers le répertoire racine.
 * @param parent Pointeur vers le répertoire parent du fichier ou dossier à copier.
 * @param nom Nom du fichier ou dossier à copier.
 * @param destination Chemin de destination pour la copie.
 */
void my_cp(fichier *racine, fichier *parent, char *nom, char *destination) ;
/**
 * @fn  fichier* chercher(fichier *courant, char *nom) ;
 * @brief  Cherche un fichier ou dossier par son nom dans le répertoire courant.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du fichier ou dossier à chercher.
 * @return Pointeur vers le fichier ou dossier trouvé, ou NULL s'il n'existe pas.
 */
fichier* chercher(fichier *courant, char *nom) ;
/**
 * @fn  void my_mv(fichier *racine, fichier *parent, char *nom, char *destination) ;
 * @brief  Déplace un fichier ou dossier vers une destination.
 * @param racine Pointeur vers le répertoire racine.
 * @param parent Pointeur vers le répertoire parent du fichier ou dossier à déplacer.
 * @param nom Nom du fichier ou dossier à déplacer.
 * @param destination Chemin de destination pour le déplacement.
 */
void my_mv(fichier *racine, fichier *parent, char *nom, char *destination) ;
/**
 * @fn  void my_rm(fichier *racine, fichier *courant, char *nom) ;
 * @brief  Supprime un fichier dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du fichier à supprimer.
 */
void my_rm(fichier *racine, fichier *courant, char *nom) ;
/**
 * @fn  void my_mkdir_p(fichier *racine, fichier *parent, char *nom) ;
 * @brief  Crée un chemin de dossiers, créant les dossiers intermédiaires si nécessaire.
 * @param racine Pointeur vers le répertoire racine.
 * @param parent Pointeur vers le répertoire parent.
 * @param nom Chemin du dossier à créer.
 */
void my_mkdir_p(fichier *racine, fichier *parent, char *nom) ;
/**
 * @fn  void my_pwd(fichier *rep_actuel) ;
 * @brief  Affiche le chemin complet du répertoire courant.
 * @param rep_actuel Pointeur vers le répertoire courant.
 */
void my_pwd(fichier *rep_actuel);
/**
 * @fn  void my_echo(char **ligne_c) ;
 * @brief  Affiche une ligne de texte.
 * @param ligne_c Tableau de chaînes de caractères à afficher.
 */
void my_echo(char **ligne_c);
/**
 * @fn  void my_ls_a(fichier* racine, fichier *courant,char *nom) ;
 * @brief  Liste tous les fichiers et dossiers, y compris les fichiers cachés, dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du répertoire à lister.
 */
void my_ls_a(fichier* racine, fichier *courant,char *nom) ;
/**
 * @fn  void deplacer(fichier* dest, fichier* source) ;
 * @brief  Déplace un fichier ou dossier d'une source vers une destination.
 * @param dest Pointeur vers le dossier de destination.
 * @param source Pointeur vers le fichier ou dossier source à déplacer.
 */
void deplacer(fichier* dest, fichier* source) ;
/**
 * @fn  void affichage(fichier* home) ;
 * @brief  Affiche les fichiers les plus ouverts dans l'arborescence.
 * @param home Pointeur vers le répertoire racine.
 */
fichier* chercher(fichier *courant, char *nom) ;
/**
 * @fn  void my_chmod(fichier* racine, fichier* courant, char* option, char* nom) ;
 * @brief  Modifie les permissions d'un fichier ou dossier.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param option Chaîne de caractères représentant les options de permission.
 * @param nom Nom du fichier ou dossier dont les permissions doivent être modifiées.
 */
void my_chmod(fichier* racine, fichier* courant, char* option, char* nom) ;
/**
 * @fn  void my_ls_l(fichier* racine, fichier *courant,char *nom) ;
 * @brief  Liste les fichiers et dossiers avec des détails dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du répertoire à lister.
 */
void my_ls_l(fichier* racine, fichier *courant,char *nom) ;
/**
 * @fn  void afficher_nbr_ouverture(fichier* home, int prfnd) ;
 * @brief  Affiche le nombre d'ouvertures pour chaque fichier dans l'arborescence.
 * @param home Pointeur vers le répertoire racine.
 * @param prfnd Profondeur actuelle dans l'arborescence.
 */
void afficher_nbr_ouverture(fichier* home, int prfnd) ;
/**
 * @fn  int recherche_max(fichier* home, int max) ;
 * @brief  Recherche le nombre maximum d'ouvertures parmi les fichiers dans l'arborescence.
 * @param home Pointeur vers le répertoire racine.
 * @param max Nombre maximum d'ouvertures trouvé jusqu'à présent.
 * @return Nombre maximum d'ouvertures.
 */
int recherche_max(fichier* home, int max) ;
/**
 * @fn  void favori(fichier* home, int max) ;
 * @brief  Affiche les fichiers ayant le nombre maximum d'ouvertures.
 * @param home Pointeur vers le répertoire racine.
 * @param max Nombre maximum d'ouvertures.
 */
void favori(fichier* home, int max) ;
/**
 * @fn  void affichage(fichier* home) ;
 * @brief  Affiche les fichiers les plus ouverts dans l'arborescence.
 * @param home Pointeur vers le répertoire racine.
 */
void affichage(fichier* home) ;
/**
 * @fn  void my_ls_la(fichier* racine, fichier *courant,char *nom) ;
 * @brief  Liste tous les fichiers et dossiers avec des détails, y compris les fichiers cachés, dans le répertoire courant.
 * @param racine Pointeur vers le répertoire racine.
 * @param courant Pointeur vers le répertoire courant.
 * @param nom Nom du répertoire à lister.
 */
void my_ls_la(fichier* racine, fichier *courant,char *nom) ;

#endif
 
