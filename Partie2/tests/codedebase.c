#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef struct 
{
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

typedef int t_grille[TAILLE][TAILLE];
typedef tCase1 t_grille1[TAILLE][TAILLE];

void chargerGrille(t_grille grille);
void afficherGrille(t_grille1 g, char message[]);
bool backtracking(t_grille1 grille, int numeroCase);
bool singletonNu(t_grille1 grille);
bool singletonCache(t_grille1 grille);
void candidat(t_grille1 grille);
void ajouterCandidat(tCase1 *c, int val);
void retireTousCandidats(t_grille1 grille,int ligne,int colonne, int valeur);
void retirerCandidat(tCase1 *c, int val);
bool estCandidat(tCase1 *c, int val);
bool pairenu(t_grille1 grille);
bool possible(t_grille1 grille, int ligne, int colonne, int valeur);
bool estGrillePleine(t_grille1 g);
int main() {
    t_grille grille;
    t_grille1 grille2;
    int numeroCase;

    chargerGrille(grille);
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            (grille2[i][j]).valeur=grille[i][j];
            (grille2[i][j]).nbCandidats=0;
        }
    }
    numeroCase = 0;

    afficherGrille(grille2, "\nGrille initiale");
    clock_t begin = clock();
    candidat(grille2);
    while (!(estGrillePleine(grille2)) && (singletonNu(grille2) || singletonCache(grille2) || !pairenu(grille2)));
    backtracking(grille2, numeroCase);
    clock_t end = clock();
    afficherGrille(grille2, "\nGrille finale");
    double tempsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;
    printf("Résolution de la grille en %f secondes\n", tempsCPU);

    return EXIT_SUCCESS;
}

bool estCandidat(tCase1 *c, int val) {
    for (int i = 0; i < c->nbCandidats; i++) {
        if (c->candidats[i] == val) {
            return true;
        }
    }
    return false;
}

bool estGrillePleine(t_grille1 g) {
  bool complet = true;
  // Parcours du tableau
  for (int lig = 0; lig < TAILLE && complet; lig++) {
    for (int col = 0; col < TAILLE && complet; col++) {
      // Vérifie si la case est vide
      if (g[lig][col].valeur == 0) {
        complet = false;
        break;
      }
    }
  }
  return complet;
}

void retirerCandidat(tCase1 *c, int val) {
    int i, j;
    i = 0;
    while (i < (*c).nbCandidats) {
        if ((*c).candidats[i] == val) {
            for (j = i; j < (*c).nbCandidats - 1; j++) {
                (*c).candidats[j] = (*c).candidats[j + 1];
            }
            (*c).nbCandidats--;
        } else {
            i++;
        }
    }
}

void ajouterCandidat(tCase1 *c, int val) {
    if ((*c).nbCandidats < TAILLE) {
        (*c).candidats[(*c).nbCandidats] = val;
        (*c).nbCandidats++;
    }
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
void afficherGrille(t_grille1 grille, char message[]) {
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
bool possible(t_grille1 grille, int ligne, int colonne, int valeur) {
    bool trouve =true;
    for (int j = 0; j < TAILLE; ++j) {
        if (grille[ligne][j].valeur == valeur) {
            trouve=false;
            break;
        }
    }
    if (trouve ==true)
    {
        for (int i = 0; i < TAILLE; ++i) {
            if (grille[i][colonne].valeur == valeur) {
                trouve=false;
                break;
            }
        }
    }
    if (trouve ==true)
    {
        int coinLigne = N * (ligne / N);
        int coinColonne = N * (colonne / N);
        for (int i = coinLigne; i < coinLigne + N; ++i) {
            for (int j = coinColonne; j < coinColonne + N; ++j) {
                if (grille[i][j].valeur == valeur) {
                    trouve=false;
                    break;
                }
            }
        }
    }
    

    return trouve;
}

bool backtracking(t_grille1 grille, int numeroCase) {
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
bool singletonNu(t_grille1 grille) {
    int numLig, numCol;
    bool updated = false;

    for (numLig = 0; numLig < TAILLE; numLig++) {
        for (numCol = 0; numCol < TAILLE; numCol++) {
            if (grille[numLig][numCol].valeur == 0 && grille[numLig][numCol].nbCandidats == 1) {
                grille[numLig][numCol].valeur = grille[numLig][numCol].candidats[0];
                retireTousCandidats(grille, numLig, numCol, grille[numLig][numCol].valeur);
                updated = true;
            }
        }
    }

    return updated;
}

bool singletonCache(t_grille1 grille) {
    int numLig, numCol, valeur;
    bool updated = false;

    // Vérifier les lignes
    for (numLig = 0; numLig < TAILLE; numLig++) {
        for (valeur = 1; valeur <= TAILLE; valeur++) {
            int conteur = 0;
            int derniereColonne = -1;

            for (numCol = 0; numCol < TAILLE; numCol++) {
                if (grille[numLig][numCol].valeur == 0 && grille[numLig][numCol].nbCandidats > 0) {
                    if (estCandidat(&grille[numLig][numCol], valeur)) {
                        conteur++;
                        derniereColonne = numCol;
                    }
                }
            }

            if (conteur == 1) {
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
            int conteur = 0;
            int derniereLigne = -1;

            for (numLig = 0; numLig < TAILLE; numLig++) {
                if (grille[numLig][numCol].valeur == 0 && grille[numLig][numCol].nbCandidats > 0) {
                    if (estCandidat(&grille[numLig][numCol], valeur)) {
                        conteur++;
                        derniereLigne = numLig;
                    }
                }
            }

            if (conteur == 1) {
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
                int conteur = 0;
                int derniereLigne = -1, derniereColonne = -1;

                for (numLig = coinLigne * N; numLig < (coinLigne + 1) * N; numLig++) {
                    for (numCol = coinColonne * N; numCol < (coinColonne + 1) * N; numCol++) {
                        if (grille[numLig][numCol].valeur == 0 && grille[numLig][numCol].nbCandidats > 0) {
                            if (estCandidat(&grille[numLig][numCol], valeur)) {
                                conteur++;
                                derniereLigne = numLig;
                                derniereColonne = numCol;
                            }
                        }
                    }
                }

                if (conteur == 1) {
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


void candidat(t_grille1 grille){
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

void retireTousCandidats(t_grille1 grille,int ligne,int colonne, int valeur){
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

bool pairenu(t_grille1 grille){
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



