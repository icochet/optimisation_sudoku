constante entier N := 3;
constante entier TAILLE := (N * N);

type tCase1 = structure
debut
    valeur: entier;
    candidats: tableau[TAILLE] de entier;
    nbCandidats: entier;
fin
type tCase2 = structure
debut
    valeur: entier;
    candidats: tableau[TAILLE + 1] de booleen;
    nbCandidats: entier;
fin

// tCase1

procedure ajouterCandidat(entF/sortF laCase: tCase1, entF val: entier) c'est
debut
    laCase.candidats[nbCandidats + 1] := val;
    laCase.nbCandidats := laCase.nbCandidats + 1;
fin
procedure retirerCandidat(entF/sortF laCase: tCase1, entF val: entier) c'est
debut
    i: entier;

    pour (i de 1 à TAILLE) faire
        si (laCase.candidats[i] == val) alors
            laCase.candidats[i] := 0;
            laCase.nbCandidats := laCase.nbCandidats - 1;
        finsi
    finfaire
fin
fonction estCandidat(entF laCase: tCase1, entF val: entier) delivre booleen c'est
debut
    i: entier;
    condition: booleen;
    condition := faux;
    
    tant que ((!condition) ET (i < TAILLE)) faire
        si (laCase.candidats[i] == val) alors
            condition := vrai;
        finsi
        i := i + 1;
    finfaire

    retourne condition;
fin
procedure nbCandidats(entF laCase: tCase1) delivre entier c'est
debut
    retourne laCase.nbCandidats;
fin

// tCase2

procedure ajouterCandidat(entF/sortF laCase: tCase2, entF val: entier) c'est
debut
    laCase.candidats[val] := vrai;
    laCase.nbCandidats := laCase.nbCandidats + 1;
fin
procedure retirerCandidat(entF/sortF laCase: tCase2, entF val: entier) c'est
debut
    laCase.candidats[val] := faux;
    laCase.nbCandidats := laCase.nbCandidats - 1;
fin
fonction estCandidat(entF laCase: tCase2, entF val: entier) delivre booleen c'est
debut
    retourne (laCase.candidats[val]);
fin
fonction nbCandidats(entF laCase: tCase2) delivre entier c'est
debut
    retourne (laCase.nbCandidats);
fin

programme deduction c'est
debut
    g: tGrille;
    progression: booleen;
    nbCasesVides: entier;

    nbCasesVides = chargerGrille(g);
    initialiserCandidats(g);
    progression = vrai;

    tant que (nbCaseVides != 0 ET progression) faire
        progression = faux;

        // technique du singleton nu
        pour chaque case libre de la grille faire
            si la case n'a qu'un seul candidat alors
                •affecter ce candidat à la case
                •nbCasesVides = nbCasesVides - 1;
                •retirer ce candidat de toutes les cases de la même ligne, de la même colonne et du même bloc
                •progression = vrai;
            finsi
        finfaire

        // technique du singleton caché
        ...

        // autres techniques...
        ...
    finfaire
fin

procedure afficherStats() c'est
debut
    
fin