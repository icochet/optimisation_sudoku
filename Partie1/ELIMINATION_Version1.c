#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define N 3
#define TAILLE (N * N)

typedef struct {
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} t_case;
typedef t_case t_grille[TAILLE][TAILLE];

void ajouterCandidat(t_case *adr_laCase, int val);
void retirerCandidat(t_case *adr_laCase, int val);
bool estCandidat(t_case laCase, int val);
void retirerTousCandidats(t_grille g, int numLigne, int numColonne, int valeur, int *adr_nbCandidatsElimines);
int chargerGrille(t_grille g, char nomFichier[30]);
bool possible(t_grille g, int numLigne, int numColonne, int valeur);
void initialiserCandidats(t_grille g, int *adr_nbCandidatsBase);
void afficherGrille(t_grille g);
void afficherStats(char nomFichier[30], int ncvt, int nbCasesVides, int nbCandidatsBase, int nbCandidatsElimines);

int main() {
    t_grille g;
    bool progression;
    char nomFichier[30];
    int nbCasesVides;
    int ncvt; // ncvt = NbCasesVidesTemplate
    int nbCandidatsBase = 0;
    int nbCandidatsElimines = 0;
    int candidat;
    int debLig; // le début de la ligne du bloc
    int debCol; // le début de la colonne du bloc
    int clig; // candidat indice de ligne
    int ccol; // candidat indice de colonne
    int lig;  // indice de ligne
    int col;  // indice de colonne

    nbCasesVides = chargerGrille(g, nomFichier);
    ncvt = nbCasesVides;
    afficherGrille(g);
    initialiserCandidats(g, &nbCandidatsBase);
    progression = true;

    while ((nbCasesVides > 0) && progression) {
        progression = false;

        // technique du singleton nu
        for (lig = 0; lig < TAILLE; lig++) {
            for (col = 0; col < TAILLE; col++) {

                if (g[lig][col].nbCandidats == 1) {
                    g[lig][col].valeur = g[lig][col].candidats[0];

                    retirerTousCandidats(g, lig, col, g[lig][col].valeur, &nbCandidatsElimines);
                    
                    nbCasesVides--;
                    progression = true;
                }
            }
        }
        // technique du singleton caché
        for (lig = 0; lig < TAILLE; lig++) {
            for (col = 0; col < TAILLE; col++) {
                int nbValeurs;

                for (candidat = 1; candidat < TAILLE + 1; candidat++) {
                    if (estCandidat(g[lig][col], candidat)) {
                        nbValeurs = 0;

                        // check la ligne
                        for (ccol = 0; ccol < TAILLE; ccol++) {
                            if (estCandidat(g[lig][ccol], candidat)) {
                                nbValeurs++;
                            }
                        }
                        if (nbValeurs == 1) {
                            g[lig][col].valeur = candidat;
                            retirerTousCandidats(g, lig, col, g[lig][col].valeur, &nbCandidatsElimines);
                            nbCasesVides--;
                            progression = true;
                        } else {
                            nbValeurs = 0;

                            //check la colonne
                            for (clig = 0; clig < TAILLE; clig++) {
                                if (estCandidat(g[clig][col], candidat)) {
                                    nbValeurs++;
                                }
                            }
                            if (nbValeurs == 1) {
                                g[lig][col].valeur = candidat;
                                retirerTousCandidats(g, lig, col, g[lig][col].valeur, &nbCandidatsElimines);
                                nbCasesVides--;
                                progression = true;
                            } else {
                                nbValeurs = 0;

                                // check le bloc
                                debLig = N * (lig / N);
                                debCol = N * (col / N);

                                for (clig = debLig; clig < debLig + N; clig++) {
                                    for (ccol = debCol; ccol < debCol + N; ccol++) {
                                        if (estCandidat(g[clig][ccol], candidat)) {
                                            nbValeurs++;
                                        }
                                    }
                                }
                                if (nbValeurs == 1) {
                                    g[lig][col].valeur = candidat;
                                    retirerTousCandidats(g, lig, col, g[lig][col].valeur, &nbCandidatsElimines);
                                    nbCasesVides--;
                                    progression = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    afficherGrille(g);
    afficherStats(nomFichier, ncvt, nbCasesVides, nbCandidatsBase, nbCandidatsElimines);

    return EXIT_SUCCESS;
}

void ajouterCandidat(t_case *adr_laCase, int val) {
    if (adr_laCase->nbCandidats < TAILLE) {
        adr_laCase->candidats[adr_laCase->nbCandidats] = val;
        adr_laCase->nbCandidats++;
    }
}
void retirerCandidat(t_case *adr_laCase, int val) {
    bool trouve = false;

    for (int i = 0; (i < adr_laCase->nbCandidats) && !trouve; i++) {
        if (adr_laCase->candidats[i] == val) {
            for (int j = i; j < adr_laCase->nbCandidats - 1; j++) {
                adr_laCase->candidats[j] = adr_laCase->candidats[j + 1];
            }
            adr_laCase->nbCandidats--;
            trouve = true;
        }
    }
}
bool estCandidat(t_case laCase, int val) {
    bool condition = false;

    for (int i = 0; (i < laCase.nbCandidats) && !condition; i++) {
        condition = laCase.candidats[i] == val;
    }

    return condition;
}
void retirerTousCandidats(t_grille g, int numLigne, int numColonne, int valeur, int *adr_nbCandidatsElimines) {
    int lig;    // indice de ligne
    int col;    // indice de colonne
    int ligcol; // indice de ligne et de colonne
    int debLig; // le début de la ligne du bloc
    int debCol; // le début de la colonne du bloc

    for (ligcol = 0; ligcol < TAILLE; ligcol++) {
        if (estCandidat(g[numLigne][ligcol], valeur)) {
            retirerCandidat(&g[numLigne][ligcol], valeur);
            (*adr_nbCandidatsElimines)++;
        }
        if (estCandidat(g[ligcol][numColonne], valeur)) {
            retirerCandidat(&g[ligcol][numColonne], valeur);
            (*adr_nbCandidatsElimines)++;
        }
    }

    debLig = N * (numLigne / N);
    debCol = N * (numColonne / N);

    for (lig = debLig; lig < debLig + N; lig++) {
        for (col = debCol; col < debCol + N; col++) {
            if (estCandidat(g[lig][col], valeur)) {
                retirerCandidat(&g[lig][col], valeur);
                (*adr_nbCandidatsElimines)++;
            }
        }
    }
}
int chargerGrille(t_grille g, char nomFichier[30]) {
    int grille[TAILLE][TAILLE]; // tableau d'entiers pour charger la grille de base
    int nbCasesVides = 0;
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
        fread(grille, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);

    for (lig = 0; lig < TAILLE; lig++) {
        for (col = 0; col < TAILLE; col++) {
            g[lig][col].valeur = grille[lig][col];
            g[lig][col].nbCandidats = 0;
            if (g[lig][col].valeur == 0) nbCasesVides++;
        }
    }

    return nbCasesVides;
}
bool possible(t_grille g, int numLigne, int numColonne, int valeur) {
    bool condition = true;
    int lig;    // indice de ligne
    int col;    // indice de colonne
    int ligcol; // indice de ligne et de colonne
    int debLig; // le début de la ligne du bloc
    int debCol; // le début de la colonne du bloc

    for (ligcol = 0; ligcol < TAILLE; ligcol++) {
        if (g[numLigne][ligcol].valeur == valeur) {
            condition = false;
        }
        if (g[ligcol][numColonne].valeur == valeur) {
            condition = false;
        }
    }

    debLig = N * (numLigne / N);
    debCol = N * (numColonne / N);

    for (lig = debLig; lig < debLig + N; lig++) {
        for (col = debCol; col < debCol + N; col++) {
            if (g[lig][col].valeur == valeur) {
                condition = false;
            }
        }
    }

    return condition;
}
void initialiserCandidats(t_grille g, int *adr_nbCandidatsBase) {
    int lig; // indice de ligne
    int col; // indice de colonne

    for (lig = 0; lig < TAILLE; lig++) {
        for (col = 0; col < TAILLE; col++) {
            if (g[lig][col].valeur == 0) {
                for (int i = 1; i < TAILLE + 1; i++) {
                    if (possible(g, lig, col, i)) {
                        ajouterCandidat(&g[lig][col], i);
                        (*adr_nbCandidatsBase)++;
                    }
                }
            }
        }
    }
}
void afficherGrille(t_grille g) {
    t_case laCase;
    int lig; // indice de ligne
    int col; // indice de colonne

    printf("\n");
    printf("      1  2  3   4  5  6   7  8  9\n\n");
    printf("    +---------+---------+---------+\n");
    for (lig = 1; lig < TAILLE + 1; lig++) {
        printf("%d   |", lig);
        for (col = 1; col < TAILLE + 1; col++) {
            laCase = g[lig - 1][col - 1];

            if (laCase.valeur == 0) printf(" . ");
            else printf(" %d ", laCase.valeur);
            if (col % N == 0) printf("|");
        }
        printf("\n");
        if (lig % N == 0) printf("    +---------+---------+---------+\n");
    }
    printf("\n");
}
void afficherStats(char nomFichier[30], int ncvt, int nbCasesVides, int nbCandidatsBase, int nbCandidatsElimines) {
    printf("\n");
    printf("******   RESULTATS POUR %s   ******\n\n", nomFichier);
    printf("Nombre de cases remplies = %d sur %d\t", ncvt - nbCasesVides, ncvt);
    printf("Taux de remplissage = %.3f %%\n\n", ((ncvt - nbCasesVides) * 1.0 / ncvt * 1.0) * 100.0);
    printf("Nombre de candidats éliminés = %d\t", nbCandidatsElimines);
    printf("Pourcentage d'élimination = %.3f %%", (nbCandidatsElimines * 1.0 / nbCandidatsBase * 1.0) * 100.0);
    printf("\n");
}