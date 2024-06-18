#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef int t_grille[TAILLE][TAILLE];

void chargerGrille(t_grille grille);
void afficherGrille(t_grille g, char message[]);
bool absentSurLigne(int valeur, t_grille grille, int numLig);
bool absentSurColonne(int valeur, t_grille grille, int numCol);
bool absentSurBloc(int valeur, t_grille grille, int numLig, int numCol);
bool backtracking(t_grille grille, int numeroCase);

int main() {
    t_grille grille;
    int numeroCase;

    chargerGrille(grille);
    numeroCase = 0;

    afficherGrille(grille, "\nGrille initiale");
    clock_t begin = clock();
    backtracking(grille, numeroCase);
    clock_t end = clock();
    afficherGrille(grille, "\nGrille finale");
    double tempsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;
    printf("Résolution de la grille en %f secondes\n", tempsCPU);

    return EXIT_SUCCESS;
}

void chargerGrille(t_grille grille) {
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL) {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else {
        fread(grille, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
}
void afficherGrille(t_grille grille, char message[]) {
    int lig; // indice de ligne
    int col; // indice de colonne

    printf("%s\n", message);
    printf("+-------------+-------------+-------------+-------------+\n");
    for (lig = 1; lig < TAILLE + 1; lig++) {
        printf("|");
        for (col = 1; col < TAILLE + 1; col++) {
            if (grille[lig - 1][col - 1] == 0) printf("  .");
            else printf("%3d", grille[lig - 1][col - 1]);
            if (col % N == 0) printf(" |");
        }
        printf("\n");
        if (lig % N == 0) printf("+-------------+-------------+-------------+-------------+\n");
    }
    printf("\n");
}
bool absentSurLigne(int valeur, t_grille grille, int numLig) {
    bool condition = true;
    int col; // indice de colonne

    for (col = 0; col < TAILLE; col++) {
        if (grille[numLig][col] == valeur) {
            condition = false;
        }
    }
    
    return condition;
}
bool absentSurColonne(int valeur, t_grille grille, int numCol) {
    bool condition = true;
    int lig; // indice de ligne

    for (lig = 0; lig < TAILLE; lig++) {
        if (grille[lig][numCol] == valeur) {
            condition = false;
        }
    }

    return condition;
}
bool absentSurBloc(int valeur, t_grille grille, int numLig, int numCol) {
    bool condition = true;
    int lig; // indice de ligne
    int col; // indice de colonne
    int debLig; // début de la ligne du bloc
    int debCol; // début de la colonne du bloc

    debLig = N * (numLig / N);
    debCol = N * (numCol / N);

    for (lig = debLig; lig < debLig + N; lig++) {
        for (col = debCol; col < debCol + N; col++) {
            if (grille[lig][col] == valeur) {
                condition = false;
            }
        }
    }
    
    return condition;
}
bool backtracking(t_grille grille, int numeroCase) {
    int numLig;
    int numCol;
    bool result = false;

    if (numeroCase == TAILLE * TAILLE) {
        // on a traité toutes les cases, la grille est résolue
        result = true;
    }
    else {
        // on récupère les "coordonnées" de la case
        numLig = numeroCase / TAILLE;
        numCol = numeroCase % TAILLE;

        if (grille[numLig][numCol] != 0) {
            // si la case n'est pas vide, on passe à la suivante (appel récursif)
            result = backtracking(grille, numeroCase + 1);
        }
        else {
            for (int valeur = 1; valeur < TAILLE + 1; valeur++) {
                if (absentSurLigne(valeur, grille, numLig) && absentSurColonne(valeur, grille, numCol) && absentSurBloc(valeur, grille, numLig, numCol)) {
                    // si la valeur est autorisée on l'inscrit dans la case...
                    grille[numLig][numCol] = valeur;
                    // ... et on passe à la suivante : appel récursif pour voir si ce choix est bon par la suite
                    if (backtracking(grille, numeroCase + 1)) {
                        result = true;
                    }
                    else {
                        grille[numLig][numCol] = 0;
                    }
                }
            }
        }
    }
    return result;
}