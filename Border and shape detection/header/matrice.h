#ifndef MATRICE_H
#define MATRICE_H


// contient les fonctions nécessaires aux calculs sur matrices 3*3
class matrice
{
    public :
    // une matrice de taille 3*3
    int _tab[3][3];

    matrice();
    matrice(int tab[3][3]);
    matrice * multiplieM(matrice m);
    matrice addM(matrice m);
    matrice addM(int tab[3][3]);
    matrice * multiplieM(int tab[3][3]);
    matrice rotateMatrice(int v);
    int * convolution(matrice m);
    void convolution(int tab[3][3],float & retour );

};

#endif
