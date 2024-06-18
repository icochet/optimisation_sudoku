void singletonCacheLigne(tGrille g, bool *adr_continue){

    bool condition, trouve;
    int test, cand, L;

    for(int l = 0; l<TAILLE; l++){
        for(int c = 0; c<TAILLE; c++){

            test = 0;
            trouve = false;

            while((g[l][c].nbCandidat > 1 ) && (test<g[l][c].nbCandidat) && (! trouve)){

                condition = true;

                L = 0;

                //printf("Pour la case (%d, %d) On cherche un si il existe un candidat = à %d \n",l, c, g[l][c].tabCandidat[test]);

                while((L<TAILLE) && (condition)){

                    cand = 0;

                    if(L != c){
                        
                        while((cand<g[l][L].nbCandidat) && (condition)){

                            //printf("Candidat de la case (%d,%d) analysé est : %d\n", l, L, g[l][L].tabCandidat[cand]);

                            if(g[l][L].tabCandidat[cand] == g[l][c].tabCandidat[test]){

                                //printf("On a trouvé qu'il était égale a celui de base analysé donc la condition passe à false\n");
                                condition = false;
                            }

                            cand++;
                        }
                    }

                    L++;
                }

                if(condition){

                    g[l][c].valeurCase = g[l][c].tabCandidat[test];
                    //printf("(ligne) La valeur de la case (%d, %d) devient : %d\n", l, c, g[l][c].valeurCase);
                    supprimerCandCase(&g[l][c]);
                    retirerToutLesCandidats(g, l, c, g[l][c].valeurCase);
                    trouve = true;
                    CaseCompleteALaFin++;
                    *adr_continue = true;
                }

                test++;
            }
        }
    }
}

void singletonCacheColonne(tGrille g, bool *adr_continue){

    bool condition, trouve;
    int test, cand, C;

    for(int l = 0; l<TAILLE; l++){
        for(int c = 0; c<TAILLE; c++){

            test = 0;
            trouve = false;

            while((g[l][c].nbCandidat > 1 ) && (test<g[l][c].nbCandidat) && (! trouve)){

                condition = true;

                C = 0;

                //printf("Pour la case (%d, %d) On cherche un si il existe un candidat = à %d \n",l, c, g[l][c].tabCandidat[test]);

                while((C<TAILLE) && (condition)){

                    cand = 0;

                    if(C != l){

                        while((cand<g[C][c].nbCandidat) && (condition)){

                            //printf("Candidat de la case (%d,%d) analysé est : %d\n", C, c, g[C][c].tabCandidat[cand]);

                            if(g[C][c].tabCandidat[cand] == g[l][c].tabCandidat[test]){

                                //printf("On a trouvé qu'il était égale a celui de base analysé donc la condition passe à false\n");
                                condition = false;
                            }

                            cand++;
                        }
                    }

                    C++;
                }

                if(condition){

                    g[l][c].valeurCase = g[l][c].tabCandidat[test];
                    //printf("(colonne) La valeur de la case (%d, %d) devient : %d\n", l, c, g[l][c].valeurCase);
                    supprimerCandCase(&g[l][c]);
                    retirerToutLesCandidats(g, l, c, g[l][c].valeurCase);
                    trouve = true;
                    CaseCompleteALaFin++;
                    *adr_continue = true;

                }

                test++;
            }
        }
    }
}

void singletonCacheCase(tGrille g, bool *adr_continue){
    
    bool condition, trouve;
    int test, cand, L, C;
    int indiceLignePetitCarre, indiceColonnePetitCarre;


    for(int l = 0; l<TAILLE; l++){

        for(int c = 0; c<TAILLE; c++){

            if(l < 3){
                indiceLignePetitCarre = 0;
            }
            else if(l < 6){
                indiceLignePetitCarre = 3;
            }
            else{
                indiceLignePetitCarre = 6;
            }

            if(c < 3){
                indiceColonnePetitCarre = 0;
            }
            else if(c < 6){
                indiceColonnePetitCarre = 3;
            }
            else{
                indiceColonnePetitCarre = 6;
            }
            

            test = 0;
            trouve = false;

            while((g[l][c].nbCandidat > 1 ) && (test<g[l][c].nbCandidat) && (! trouve)){

                condition = true;

                L = indiceLignePetitCarre;

                //printf("Pour la case (%d, %d) On cherche un si il existe un candidat = à %d \n",l, c, g[l][c].tabCandidat[test]);

                while((L<indiceLignePetitCarre+3) && (condition)){

                    C = indiceColonnePetitCarre;

                    while((C<indiceColonnePetitCarre+3) && (condition)){

                        cand = 0;

                        if((l != L) || (c != C)){

                            while((cand<g[L][C].nbCandidat) && (condition)){

                                //printf("Candidat de la case (%d,%d) analysé est : %d\n", L, C, g[L][C].tabCandidat[cand]);

                                if(g[L][C].tabCandidat[cand] == g[l][c].tabCandidat[test]){

                                    //printf("On a trouvé qu'il était égale a celui de base analysé donc la condition passe à false\n");

                                    condition = false;
                                }

                                cand++;
                            }
                        }

                        C++;
                    }

                    L++;
                }

                if(condition){

                    g[l][c].valeurCase = g[l][c].tabCandidat[test];
                    //printf("(case) La valeur de la case (%d, %d) devient : %d\n", l, c, g[l][c].valeurCase);
                    supprimerCandCase(&g[l][c]);
                    retirerToutLesCandidats(g, l, c, g[l][c].valeurCase);
                    trouve = true;
                    CaseCompleteALaFin++;
                    *adr_continue = true;

                }

                test++;
            }
        }
    }
}