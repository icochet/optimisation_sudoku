#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 4
#define TAILLE (N * N)

typedef int t_grille[TAILLE][TAILLE];

void chargerGrille(t_grille grille);
void afficherGrille(t_grille g);
bool absentSurLigne(int valeur, int numLig);
bool absentSurColonne(int valeur, int numCol);
bool absentSurBloc(int valeur, int numLig, int numCol);
bool backtracking(t_grille grille, int numeroCase);
void initialiserMarqueurs(t_grille grille);
void afficherMarqueurs(t_grille grille);

bool ligne[TAILLE][TAILLE + 1];   // Pour chaque ligne, les valeurs de 1 à TAILLE (Marqueur de présence)
bool colonne[TAILLE][TAILLE + 1]; // Pour chaque colonne, les valeurs de 1 à TAILLE (Marqueur de présence)
bool bloc[TAILLE][TAILLE + 1];    // Pour chaque bloc, les valeurs de 1 à TAILLE (Marqueur de présence)

int main()
{
    t_grille grille;
    int numeroCase;

    chargerGrille(grille);
    initialiserMarqueurs(grille);
    numeroCase = 0;

    afficherGrille(grille);
    clock_t begin = clock();
    backtracking(grille, numeroCase);
    clock_t end = clock();
    afficherMarqueurs(grille);
    afficherGrille(grille);
    double tempsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;
    printf("Temps CPU = %.3f secondes\n", tempsCPU);

    return EXIT_SUCCESS;
}

void initialiserMarqueurs(t_grille grille)
{
    int lig, col;
    for (lig = 0; lig < TAILLE; lig++)
    {
        for (col = 0; col < TAILLE; col++)
        {
            int valeur = grille[lig][col];
            ligne[lig][valeur] = true;
            colonne[col][valeur] = true;
            int blocIndex = (lig / N) * N + col / N;
            bloc[blocIndex][valeur] = true;
        }
    }
}
void afficherMarqueurs(t_grille grille)
{
    int lig; // indice de ligne
    int col; // indice de colonne
    int valeur;
    int blocIndex;

    for (lig = 0; lig < TAILLE; lig++)
    {
        for (col = 0; col < TAILLE; col++)
        {
            valeur = grille[lig][col];
            blocIndex = N * (lig / N) + col / N;

            printf("%d", ligne[lig][valeur]);
            printf("%d", colonne[col][valeur]);
            printf("%d", bloc[blocIndex][valeur]);
        }
    }
}
void chargerGrille(t_grille grille)
{
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else
    {
        fread(grille, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
}
void afficherGrille(t_grille grille)
{
    int lig; // indice de ligne
    int col; // indice de colonne

    printf("\n");
    printf("    +---------+---------+---------+\n");
    for (lig = 1; lig < TAILLE + 1; lig++)
    {
        printf("%d   |", lig);
        for (col = 1; col < TAILLE + 1; col++)
        {
            if (grille[lig - 1][col - 1] == 0)
                printf(" . ");
            else
                printf(" %d ", grille[lig - 1][col - 1]);
            if (col % N == 0)
                printf("|");
        }
        printf("\n");
        if (lig % N == 0)
            printf("    +---------+---------+---------+\n");
    }
    printf("\n");
}
bool absentSurLigne(int valeur, int numLig)
{
    return !ligne[numLig][valeur];
}

bool absentSurColonne(int valeur, int numCol)
{
    return !colonne[numCol][valeur];
}

bool absentSurBloc(int valeur, int numLig, int numCol)
{
    int blocIndex = (numLig / N) * N + numCol / N;
    return !bloc[blocIndex][valeur];
}
bool backtracking(t_grille grille, int numeroCase)
{
    int numLig, numCol;
    bool result = false;

    if (numeroCase == TAILLE * TAILLE)
    {
        // on a traité toutes les cases, la grille est résolue
        result = true;
    }
    else
    {
        // on récupère les "coordonnées" de la case
        numLig = numeroCase / TAILLE;
        numCol = numeroCase % TAILLE;

        if (grille[numLig][numCol] != 0)
        {
            // si la case n'est pas vide, on passe à la suivante (appel récursif)
            result = backtracking(grille, numeroCase + 1);
        }
        else
        {
            for (int valeur = 1; valeur < TAILLE + 1; valeur++)
            {
                if (absentSurLigne(valeur, numLig) && absentSurColonne(valeur, numCol) && absentSurBloc(valeur, numLig, numCol))
                {
                    // si la valeur est autorisée on l'inscrit dans la case...
                    grille[numLig][numCol] = valeur;

                    // Mettre à jour les marqueurs pour cette valeur
                    ligne[numLig][valeur] = true;
                    colonne[numCol][valeur] = true;
                    int blocIndex = (numLig / N) * N + numCol / N;
                    bloc[blocIndex][valeur] = true;

                    // ... et on passe à la suivante : appel récursif pour voir
                    // si ce choix est bon par la suite
                    if (backtracking(grille, numeroCase + 1))
                    {
                        result = true;
                        break; // Si une solution est trouvée, sortir de la boucle for
                    }
                    else
                    {
                        // Annuler la modification et remettre les marqueurs à false
                        grille[numLig][numCol] = 0;
                        ligne[numLig][valeur] = false;
                        colonne[numCol][valeur] = false;
                        bloc[blocIndex][valeur] = false;
                    }
                }
            }
        }
    }
    return result;
}
