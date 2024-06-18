#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 4
#define TAILLE (N * N)

typedef int t_grille[TAILLE][TAILLE];

// Structure pour stocker les marqueurs de présence dans les lignes, colonnes et blocs
typedef struct {
    bool ligne[TAILLE][TAILLE + 1];
    bool colonne[TAILLE][TAILLE + 1];
    bool bloc[TAILLE][TAILLE + 1];
} Marqueurs;

void initialiserMarqueurs(Marqueurs *marqueurs);
void mettreAJourMarqueurs(Marqueurs *marqueurs, int numLig, int numCol, int valeur);
bool absentSurLigne(Marqueurs *marqueurs, int valeur, int numLig);
bool absentSurColonne(Marqueurs *marqueurs, int valeur, int numCol);
bool absentSurBloc(Marqueurs *marqueurs, int valeur, int numLig, int numCol);
bool backtracking(t_grille grille, int numeroCase, Marqueurs *marqueurs);

int main() {
    t_grille grille;
    int numeroCase;

    Marqueurs marqueurs;
    initialiserMarqueurs(&marqueurs);

    chargerGrille(grille);
    numeroCase = 0;

    afficherGrille(grille);
    clock_t begin = clock();
    backtracking(grille, numeroCase, &marqueurs);
    clock_t end = clock();
    afficherGrille(grille);
    double tempsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;
    printf("Temps CPU = %.3f secondes\n", tempsCPU);

    return EXIT_SUCCESS;
}

void initialiserMarqueurs(Marqueurs *marqueurs) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j <= TAILLE; j++) {
            marqueurs->ligne[i][j] = false;
            marqueurs->colonne[i][j] = false;
            marqueurs->bloc[i][j] = false;
        }
    }
}

void mettreAJourMarqueurs(Marqueurs *marqueurs, int numLig, int numCol, int valeur) {
    marqueurs->ligne[numLig][valeur] = true;
    marqueurs->colonne[numCol][valeur] = true;
    int blocIndex = (numLig / N) * N + numCol / N;
    marqueurs->bloc[blocIndex][valeur] = true;
}

bool absentSurLigne(Marqueurs *marqueurs, int valeur, int numLig) {
    return !marqueurs->ligne[numLig][valeur];
}

bool absentSurColonne(Marqueurs *marqueurs, int valeur, int numCol) {
    return !marqueurs->colonne[numCol][valeur];
}

bool absentSurBloc(Marqueurs *marqueurs, int valeur, int numLig, int numCol) {
    int blocIndex = (numLig / N) * N + numCol / N;
    return !marqueurs->bloc[blocIndex][valeur];
}

bool backtracking(t_grille grille, int numeroCase, Marqueurs *marqueurs) {
    int numLig, numCol;
    bool result = false;

    if (numeroCase == TAILLE * TAILLE) {
        // on a traité toutes les cases, la grille est résolue
        result = true;
    } else {
        // on récupère les "coordonnées" de la case
        numLig = numeroCase / TAILLE;
        numCol = numeroCase % TAILLE;

        if (grille[numLig][numCol] != 0) {
            // si la case n'est pas vide, on passe à la suivante (appel récursif)
            result = backtracking(grille, numeroCase + 1, marqueurs);
        } else {
            for (int valeur = 1; valeur <= TAILLE && !result; valeur++) {
                if (absentSurLigne(marqueurs, valeur, numLig) && absentSurColonne(marqueurs, valeur, numCol) && absentSurBloc(marqueurs, valeur, numLig, numCol)) {
                    // mettre à jour les marqueurs pour cette valeur
                    mettreAJourMarqueurs(marqueurs, numLig, numCol, valeur);

                    // si la valeur est autorisée, on l'inscrit dans la case...
                    grille[numLig][numCol] = valeur;

                    // ... et on passe à la suivante : appel récursif
                    result = backtracking(grille, numeroCase + 1, marqueurs);

                    // si ce choix est bon par la suite
                    if (!result) {
                        // Annuler la modification et remettre les marqueurs à false
                        grille[numLig][numCol] = 0;
                        marqueurs->ligne[numLig][valeur] = false;
                        marqueurs->colonne[numCol][valeur] = false;
                        int blocIndex = (numLig / N) * N + numCol / N;
                        marqueurs->bloc[blocIndex][valeur] = false;
                    }
                }
            }
        }
    }
    return result;
}


