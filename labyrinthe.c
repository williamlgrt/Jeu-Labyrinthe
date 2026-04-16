#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h> // Bibliothèque ncurses pour la gestion des touches

#define MAX_SALLES 100
#define MAX_PORTES 4
#define SAUVEGARDE_FICHIER "sauvegarde.txt"

typedef struct {
    int numero;
    int liens[MAX_PORTES]; // N, S, E, O : -1 si pas de lien
    int cle;               // 1 si une clé est présente, 0 sinon
    int verrou[MAX_PORTES]; // 1 si la porte est verrouillée, 0 sinon
    int azatoth;           // 1 si Azatoth est ici, 0 sinon
} Salle;

Salle labyrinthe[MAX_SALLES];
int nombre_salles = 0;
int position_chevalier = 0;
int possede_cle = 0;

void charger_plan(const char *fichier);
void sauvegarder_partie();
void charger_sauvegarde();
void afficher_etat_salle(int salle);
void deplacer_chevalier(int direction);
void placer_cles_et_verrous();

void charger_plan(const char *fichier) {
    FILE *fp = fopen(fichier, "r");
    if (!fp) {
        perror("Erreur lors de l'ouverture du fichier de plan");
        exit(EXIT_FAILURE);
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fp)) {
        if (strncmp(ligne, "debutPlan", 9) == 0 || strncmp(ligne, "finPlan", 7) == 0) {
            continue;
        }

        int salle1, salle2;
        char porte1, porte2;
        if (sscanf(ligne, "%*s %d.%c-%d.%c;", &salle1, &porte1, &salle2, &porte2) == 4) {
            if (strchr("NSEO", porte1) == NULL || strchr("NSEO", porte2) == NULL) {
                printf("Erreur dans le fichier: direction invalide dans la ligne: %s", ligne);
                continue;
            }

            while (salle1 >= nombre_salles || salle2 >= nombre_salles) {
                labyrinthe[nombre_salles].numero = nombre_salles;
                for (int i = 0; i < MAX_PORTES; i++) {
                    labyrinthe[nombre_salles].liens[i] = -1;
                    labyrinthe[nombre_salles].verrou[i] = 0;
                }
                labyrinthe[nombre_salles].cle = 0;
                labyrinthe[nombre_salles].azatoth = 0;
                nombre_salles++;
            }

            int index1 = (porte1 == 'N' ? 0 : porte1 == 'S' ? 1 : porte1 == 'E' ? 2 : 3);
            int index2 = (porte2 == 'N' ? 0 : porte2 == 'S' ? 1 : porte2 == 'E' ? 2 : 3);

            labyrinthe[salle1].liens[index1] = salle2;
            labyrinthe[salle2].liens[index2] = salle1;
        }
    }
    fclose(fp);
}

void sauvegarder_partie() {
    FILE *fp = fopen(SAUVEGARDE_FICHIER, "w");
    if (!fp) {
        return;
    }
    fprintf(fp, "%d %d\n", position_chevalier, possede_cle);
    for (int i = 0; i < nombre_salles; i++) {
        fprintf(fp, "%d\n", labyrinthe[i].cle);
    }
    fclose(fp);
}

void charger_sauvegarde() {
    FILE *fp = fopen(SAUVEGARDE_FICHIER, "r");
    if (!fp) {
        return;
    }
    fscanf(fp, "%d %d", &position_chevalier, &possede_cle);
    for (int i = 0; i < nombre_salles; i++) {
        fscanf(fp, "%d", &labyrinthe[i].cle);
    }
    fclose(fp);
}

void afficher_etat_salle(int salle) {
    printw("Vous êtes dans la salle %d.\n", salle);

    // Afficher l'état de la clé
    printw("Clé : %d\n", possede_cle);

    const char *portes[] = {"Nord", "Sud", "Est", "Ouest"};
    printw("Passages disponibles :\n");
    for (int i = 0; i < MAX_PORTES; i++) {
        if (labyrinthe[salle].liens[i] != -1) {
            if (labyrinthe[salle].verrou[i]) {
                printw("- %s (verrouillé)\n", portes[i]);
            } else {
                printw("- %s (vers salle %d)\n", portes[i], labyrinthe[salle].liens[i]);
            }
        }
    }
    refresh();
}

void deplacer_chevalier(int direction) {
    // Vérifie si un lien existe dans la direction donnée
    if (labyrinthe[position_chevalier].liens[direction] != -1) {
        // Vérifie si la porte est verrouillée
        if (labyrinthe[position_chevalier].verrou[direction]) {
            printw("La porte est verrouillée. Voulez-vous utiliser une clé ? (o/n)\n");
            refresh();

            int choix = getch(); // Lit la réponse du joueur
            if (choix == 'o') {
                if (possede_cle > 0) {
                    printw("Vous utilisez une clé pour déverrouiller la porte.\n");
                    labyrinthe[position_chevalier].verrou[direction] = 0; // Déverrouille la porte
                    possede_cle = possede_cle - 1; // Consomme la clé
                } else {
                    printw("Vous n'avez pas de clé !\n");
                    refresh();
                    return; // Bloque le déplacement
                }
            } else {
                printw("Vous choisissez de ne pas utiliser de clé.\n");
                refresh();
                return; // Bloque le déplacement
            }
        }

        // Déplace le chevalier dans la nouvelle salle
        position_chevalier = labyrinthe[position_chevalier].liens[direction];
        printw("Vous entrez dans la salle %d.\n", position_chevalier);
        refresh();
        getch();

        // Vérifie si une clé est présente dans la salle
        if (labyrinthe[position_chevalier].cle) {
            possede_cle = 1; // Ajoute une clé à l'inventaire
            labyrinthe[position_chevalier].cle = 0; // Supprime la clé de la salle
        }
    } else {
        printw("Il n'y a pas de passage dans cette direction.\n");
        refresh();
        getch();
    }
    refresh();
}
void placer_cles_et_verrous() {
    for (int i = 0; i < 3; i++) {
        int salle_cle = rand() % nombre_salles;
        labyrinthe[salle_cle].cle = 1;
    }
    for (int i = 0; i < 3; i++) {
        int salle_verrou = rand() % nombre_salles;
        int direction_verrou = rand() % MAX_PORTES;
        if (labyrinthe[salle_verrou].liens[direction_verrou] != -1) {
            labyrinthe[salle_verrou].verrou[direction_verrou] = 1;
        }
    }
}

void jouer() {
    initscr();                // Initialiser ncurses
    noecho();                 // Ne pas afficher les caractères saisis
    cbreak();                 // Lire les touches immédiatement
    keypad(stdscr, TRUE);     // Activer les touches spéciales
    curs_set(0);              // Masquer le curseur

    int jeu_actif = 1;
    while (jeu_actif) {
        clear(); // Effacer l'écran
        afficher_etat_salle(position_chevalier);

        printw("\nUtilisez les flèches directionnelles pour vous déplacer :\n");
        printw("( Nord = Haut, Ouest = Gauche, Est = Droite, Sud = Bas )\n");
        printw("q - Quitter, p - Sauvegarder, c - Charger\n");
        refresh(); // Afficher le contenu

        int touche = getch();
        switch (touche) { // if , else
            case KEY_UP:
                deplacer_chevalier(0); // Nord
                break;
            case KEY_DOWN:
                deplacer_chevalier(1); // Sud
                break;
            case KEY_RIGHT:
                deplacer_chevalier(2); // Est
                break;
            case KEY_LEFT:
                deplacer_chevalier(3); // Ouest
                break;
            case 'p':
                sauvegarder_partie();
                break;
            case 'c':
                charger_sauvegarde();
                break;
            case 'q':
                jeu_actif = 0; // Quitter
                break;
        }

        if (labyrinthe[position_chevalier].azatoth) {
            clear();
            printw("Félicitations, vous avez trouvé Azatoth !\n");
            printw("Appuyez sur une touche pour quitter.\n");
            refresh();
            getch();
            jeu_actif = 0;
        }
    }

    endwin(); // Fermer ncurses
}

int main() {
    char nom_fichier[50];
    printf("Veuillez le nom du fichier pour le labyrinthe ( ****.pln ) : ");
    scanf("%49s", nom_fichier); // L'utilisateur entre le nom du fichier

    srand(time(NULL)); // Initialise le générateur aléatoire

    charger_plan(nom_fichier); // Charge le fichier spécifié

    labyrinthe[nombre_salles - 1].azatoth = 1; // Place Azatoth dans la dernière salle

    placer_cles_et_verrous();

    printf("Bienvenue dans mon labyrinthe\n");
    jouer(); // Lance le jeu

    return 0;
}