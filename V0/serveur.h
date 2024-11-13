#define MAX_JEUX 10


// Définition d'une structure représentant un jeu
typedef struct {
    char NomJeu[25];
    char *Code; // NULL si le jeu n'est pas encore téléchargé
} Jeu;


Jeu jeux[MAX_JEUX];

// Définition d'une structure représentant une demande d'opération
typedef struct {
    int CodeOp; // Code de l’opération : 1 pour tester, 2 pour ajouter, etc.
    char NomJeu[25]; // Nom du jeu sur lequel l'opération est effectuée
    char Param[200]; // Paramètre supplémentaire comme l'adresse de téléchargement
    int flag; // 0 par défaut, utilisé pour des informations supplémentaires
} DemandeOperation;

// Prototypes des fonctions
int execute_demande(DemandeOperation OP, Jeu jeux[], int *nbJeux); // Exécute une demande
