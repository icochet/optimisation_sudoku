#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef struct {
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} t_case;

typedef t_case t_grille[TAILLE][TAILLE];

void chargerGrille(t_grille grille);
void afficherGrille(t_grille grille, char message[]);
bool backtracking(t_grille grille, int numeroCase);
bool singletonNu(t_grille grille);
bool singletonCache(t_grille grille);
void candidat(t_grille grille);
void ajouterCandidat(t_case *adr_laCase, int val);
void retireTousCandidats(t_grille grille, int ligne, int colonne, int valeur);
void retirerCandidat(t_case *adr_laCase, int val);
bool estCandidat(t_case *adr_laCase, int val);
bool pairenu(t_grille grille);
bool possible(t_grille grille, int ligne, int colonne, int valeur);
bool estGrillePleine(t_grille grille);
bool pairecache (t_grille grille);

int main() {
    t_grille grille;
    int numeroCase;

    chargerGrille(grille);
    numeroCase = 0;

    afficherGrille(grille, "\nGrille initiale");
    clock_t begin = clock();

    candidat(grille);
    while (!(estGrillePleine(grille)) && (singletonNu(grille) || singletonCache(grille) || !pairenu(grille) || pairecache (grille)));
    backtracking(grille, numeroCase);

    clock_t end = clock();
    afficherGrille(grille, "\nGrille finale");
    double tempsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;
    printf("Résolution de la grille en %f secondes\n", tempsCPU);

    return EXIT_SUCCESS;
}

void chargerGrille(t_grille grille) {
    char nomFichier[30];
    int gINT[TAILLE][TAILLE]; // tableau d'entiers pour charger la grille de base
    int lig; // indice de ligne
    int col; // indice de colonne

    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL) {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } 
    else {
        fread(gINT, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);

    for (lig = 0; lig < TAILLE; lig++) {
        for (col = 0; col < TAILLE; col++) {
            grille[lig][col].valeur = gINT[lig][col];
            for (int i = 1; i < TAILLE + 1; i++) {
                grille[lig][col].candidats[i] = false;
            }
            grille[lig][col].nbCandidats = 0;
        }
    }
}
void afficherGrille(t_grille grille, char message[]) {
    int lig; // indice de ligne
    int col; // indice de colonne

    printf("%s\n", message);
    printf("+-------------+-------------+-------------+-------------+\n");
    for (lig = 1; lig < TAILLE + 1; lig++) {
        printf("|");
        for (col = 1; col < TAILLE + 1; col++) {
            if (grille[lig - 1][col - 1].valeur == 0) printf("  .");
            else printf("%3d", grille[lig - 1][col - 1].valeur);
            if (col % N == 0) printf(" |");
        }
        printf("\n");
        if (lig % N == 0) printf("+-------------+-------------+-------------+-------------+\n");
    }
    printf("\n");
}
bool estCandidat(t_case *adr_laCase, int val) {
    for (int i = 0; i < adr_laCase->nbCandidats; i++) {
        if (adr_laCase->candidats[i] == val) {
            return true;
        }
    }
    return false;
}
bool estGrillePleine(t_grille grille) {
    bool complet = true;

    // Parcours du tableau
    for (int lig = 0; lig < TAILLE && complet; lig++) {
        for (int col = 0; col < TAILLE && complet; col++) {
            // Vérifie si la case est vide
            if (grille[lig][col].valeur == 0) {
                complet = false;
            }
        }
    }

    return complet;
}
void retirerCandidat(t_case *adr_laCase, int val) {
    int i, j;
    i = 0;

    while (i < (*adr_laCase).nbCandidats) {
        if ((*adr_laCase).candidats[i] == val) {
            for (j = i; j < (*adr_laCase).nbCandidats - 1; j++) {
                (*adr_laCase).candidats[j] = (*adr_laCase).candidats[j + 1];
            }
            (*adr_laCase).nbCandidats--;
        } else {
            i++;
        }
    }
}
void ajouterCandidat(t_case *adr_laCase, int val) {
    if ((*adr_laCase).nbCandidats < TAILLE) {
        (*adr_laCase).candidats[(*adr_laCase).nbCandidats] = val;
        (*adr_laCase).nbCandidats++;
    }
}
bool possible(t_grille grille, int ligne, int colonne, int valeur) {
    bool trouve = true;

    for (int j = 0; (j < TAILLE) && trouve; ++j) {
        if (grille[ligne][j].valeur == valeur) {
            trouve = false;
        }
    }
    for (int i = 0; (i < TAILLE) && trouve; ++i) {
        if (grille[i][colonne].valeur == valeur) {
            trouve = false;
        }
    }
    int coinLigne = N * (ligne / N);
    int coinColonne = N * (colonne / N);

    for (int i = coinLigne; (i < coinLigne + N) && trouve; ++i) {
        for (int j = coinColonne; (j < coinColonne + N) && trouve; ++j) {
            if (grille[i][j].valeur == valeur) {
                trouve = false;
            }
        }
    }
    

    return trouve;
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

        if (grille[numLig][numCol].valeur != 0) {
            // si la case n'est pas vide, on passe à la suivante (appel récursif)
            result = backtracking(grille, numeroCase + 1);
        }
        else {
            for (int valeur = 1; valeur < TAILLE + 1; valeur++) {
                if (possible(grille, numLig, numCol,valeur)) {
                    // si la valeur est autorisée on l'inscrit dans la case...
                    grille[numLig][numCol].valeur = valeur;
                    // ... et on passe à la suivante : appel récursif pour voir si ce choix est bon par la suite
                    if (backtracking(grille, numeroCase + 1)) {
                        result = true;
                    }
                    else {
                        grille[numLig][numCol].valeur = 0;
                    }
                }
            }
        }
    }
    return result;
}
bool singletonNu(t_grille grille) {
    int numLig, numCol;
    bool updated = false;

    for (numLig = 0; numLig < TAILLE; numLig++) {
        for (numCol = 0; numCol < TAILLE; numCol++) {
            if ((grille[numLig][numCol].valeur == 0) && (grille[numLig][numCol].nbCandidats == 1)) {
                grille[numLig][numCol].valeur = grille[numLig][numCol].candidats[0];
                retireTousCandidats(grille, numLig, numCol, grille[numLig][numCol].valeur);
                updated = true;
            }
        }
    }

    return updated;
}
bool singletonCache(t_grille grille) {
    int numLig, numCol, valeur;
    bool updated = false;

    // Vérifier les lignes
    for (numLig = 0; numLig < TAILLE; numLig++) {
        for (valeur = 1; valeur <= TAILLE; valeur++) {
            int counter = 0;
            int derniereColonne = -1;

            for (numCol = 0; numCol < TAILLE; numCol++) {
                if (grille[numLig][numCol].valeur == 0 && grille[numLig][numCol].nbCandidats > 0) {
                    if (estCandidat(&grille[numLig][numCol], valeur)) {
                        counter++;
                        derniereColonne = numCol;
                    }
                }
            }

            if (counter == 1) {
                // Un seul candidat caché dans la ligne, remplir la case
                grille[numLig][derniereColonne].valeur = valeur;
                retireTousCandidats(grille, numLig, derniereColonne, valeur);
                updated = true;
            }
        }
    }

    // Vérifier les colonnes
    for (numCol = 0; numCol < TAILLE; numCol++) {
        for (valeur = 1; valeur <= TAILLE; valeur++) {
            int counter = 0;
            int derniereLigne = -1;

            for (numLig = 0; numLig < TAILLE; numLig++) {
                if (grille[numLig][numCol].valeur == 0 && grille[numLig][numCol].nbCandidats > 0) {
                    if (estCandidat(&grille[numLig][numCol], valeur)) {
                        counter++;
                        derniereLigne = numLig;
                    }
                }
            }

            if (counter == 1) {
                // Un seul candidat caché dans la colonne, remplir la case
                grille[derniereLigne][numCol].valeur = valeur;
                retireTousCandidats(grille, derniereLigne, numCol, valeur);
                updated = true;
            }
        }
    }

    // Vérifier les blocs
    for (int coinLigne = 0; coinLigne < N; coinLigne++) {
        for (int coinColonne = 0; coinColonne < N; coinColonne++) {
            for (valeur = 1; valeur <= TAILLE; valeur++) {
                int counter = 0;
                int derniereLigne = -1, derniereColonne = -1;

                for (numLig = coinLigne * N; numLig < (coinLigne + 1) * N; numLig++) {
                    for (numCol = coinColonne * N; numCol < (coinColonne + 1) * N; numCol++) {
                        if (grille[numLig][numCol].valeur == 0 && grille[numLig][numCol].nbCandidats > 0) {
                            if (estCandidat(&grille[numLig][numCol], valeur)) {
                                counter++;
                                derniereLigne = numLig;
                                derniereColonne = numCol;
                            }
                        }
                    }
                }

                if (counter == 1) {
                    // Un seul candidat caché dans le bloc, remplir la case
                    grille[derniereLigne][derniereColonne].valeur = valeur;
                    retireTousCandidats(grille, derniereLigne, derniereColonne, valeur);
                    updated = true;
                }
            }
        }
    }

    return updated;
}
void candidat(t_grille grille){
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            for (int l = 0; l < TAILLE; l++)
            {
                if (possible(grille,i,j,l+1) )
                {
                    ajouterCandidat(&grille[i][j],l+1);
                }
            }            
        }
    }
}
void retireTousCandidats(t_grille grille, int ligne, int colonne, int valeur){
    for (int i = 0; i < TAILLE; i++)
    {
        retirerCandidat(&grille[ligne][i],valeur);
        retirerCandidat(&grille[i][colonne],valeur);
    }
    int coinLigne = N * (ligne / N);
    int coinColonne = N * (colonne / N);
    for (int i = coinLigne; i < coinLigne + N; ++i) {
        for (int j = coinColonne; j < coinColonne + N; ++j) {
            retirerCandidat(&grille[i][j],valeur);
        }
    }
}
bool pairenu(t_grille grille){
    bool trouve =false;
    int v1,v2;
    for (int numLig = 0; numLig < TAILLE; numLig++)
    {
        for (int numCol = 0; numCol < TAILLE; numCol++)
        {
            if (grille[numLig][numCol].nbCandidats == 2)
            {
                v1=grille[numLig][numCol].candidats[0];
                v2=grille[numLig][numCol].candidats[1];
                int coinLigne = N * (numLig / N);
                int coinColonne = N * (numCol / N);
                for (int i = coinLigne; i < coinLigne + N; ++i) {
                    for (int j = coinColonne; j < coinColonne + N; ++j) {
                        if ((i!=numLig && j!=numCol)&&(grille[i][j].nbCandidats == 2)&&((grille[i][j].candidats[0]==v1 || grille[i][j].candidats[0]==v2)&&(grille[i][j].candidats[1]==v1 || grille[i][j].candidats[1]==v2)) ){
                            trouve =true;
                            for (int k = coinLigne; k < coinLigne + N; k++) {
                               for (int l = coinColonne; l < coinColonne + N; l++) {
                                   if ((k!=numLig && l!=numCol)&&(k!=i && l!=j))
                                   {
                                   retirerCandidat(&grille[k][l],v1);
                                   retirerCandidat(&grille[k][l],v2);
                                   }
                               }
                            }                       
                        }
                    }
                }
            }
        }
    }
    return trouve;
}
bool pairecache(t_grille grille) {
    bool trouve = false;
    int counter;
    
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j].nbCandidats > 2) {
                for (int v1 = 0; v1 < grille[i][j].nbCandidats - 1; v1++) {
                    for (int v2 = v1 + 1; v2 < grille[i][j].nbCandidats; v2++) {
                        
                        // Vérification ligne
                        counter = 0;
                        for (int k = 0; k < TAILLE; k++) {
                            if (k != j) {
                                for (int cand = 0; cand < grille[i][k].nbCandidats; cand++) {
                                    if (grille[i][k].candidats[cand] == grille[i][j].candidats[v1] || grille[i][k].candidats[cand] == grille[i][j].candidats[v2]) {
                                        counter++;
                                        if (counter > 2) {
                                            break;
                                        }
                                    }
                                }
                            }
                            if (counter > 2) {
                                break;
                            }
                        }

                        if (counter == 2) {
                            // Retirer les candidats dans la ligne
                            for (int supp = 0; supp < grille[i][j].nbCandidats; supp++) {
                                if (grille[i][j].candidats[supp] != grille[i][j].candidats[v1] && grille[i][j].candidats[supp] != grille[i][j].candidats[v2]) {
                                    retirerCandidat(&grille[i][j], grille[i][j].candidats[supp]);
                                }
                            }
                            trouve = true;
                            printf("ok (ligne)\n");
                        }

                        // Vérification colonne
                        counter = 0;
                        for (int k = 0; k < TAILLE; k++) {
                            if (k != i) {
                                for (int cand = 0; cand < grille[k][j].nbCandidats; cand++) {
                                    if (grille[k][j].candidats[cand] == grille[i][j].candidats[v1] || grille[k][j].candidats[cand] == grille[i][j].candidats[v2]) {
                                        counter++;
                                        if (counter > 2) {
                                            break;
                                        }
                                    }
                                }
                            }
                            if (counter > 2) {
                                break;
                            }
                        }

                        if (counter == 2) {
                            // Retirer les candidats dans la colonne
                            for (int supp = 0; supp < grille[i][j].nbCandidats; supp++) {
                                if (grille[i][j].candidats[supp] != grille[i][j].candidats[v1] && grille[i][j].candidats[supp] != grille[i][j].candidats[v2]) {
                                    retirerCandidat(&grille[i][j], grille[i][j].candidats[supp]);
                                }
                            }
                            trouve = true;
                            printf("ok (colonne)\n");
                        }

                        // Vérification bloc
                        int coinLigne = N * (i / N);
                        int coinColonne = N * (j / N);
                        counter = 0;
                        for (int m = coinLigne; m < coinLigne + N; m++) {
                            for (int n = coinColonne; n < coinColonne + N; n++) {
                                if (m != i || n != j) {
                                    for (int cand = 0; cand < grille[m][n].nbCandidats; cand++) {
                                        if (grille[m][n].candidats[cand] == grille[i][j].candidats[v1] || grille[m][n].candidats[cand] == grille[i][j].candidats[v2]) {
                                            counter++;
                                            if (counter > 2) {
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (counter > 2) {
                                    break;
                                }
                            }
                            if (counter > 2) {
                                break;
                            }
                        }

                        if (counter == 2) {
                            // Retirer les candidats dans le bloc
                            for (int supp = 0; supp < grille[i][j].nbCandidats; supp++) {
                                if (grille[i][j].candidats[supp] != grille[i][j].candidats[v1] && grille[i][j].candidats[supp] != grille[i][j].candidats[v2]) {
                                    retirerCandidat(&grille[i][j], grille[i][j].candidats[supp]);
                                }
                            }
                            trouve = true;
                            printf("ok (bloc)\n");
                        }
                    }
                }
            }
        }
    }
    return trouve;
}

