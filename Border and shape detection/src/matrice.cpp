#include "header/matrice.h"
#include <iostream>

// le constructeur par défaut, rempli la matrice de 0
matrice::matrice()
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            _tab[i][j]=0;
        }
    }
}

// le constructeur par copie, rempli la matrice avec les valeurs contenus dans la table
matrice::matrice(int tab[3][3])
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            _tab[i][j]=tab[i][j];
        }
    }
}

// effectue la multiplication entre deux matrices
matrice * matrice::multiplieM(matrice m)
{
    matrice * ret = new matrice();
    for(int i = 0; i < 3; i++)
         for(int j = 0; j < 3; j++)
             ret->_tab[i][j]= _tab[i][0] * m._tab[0][j] + _tab[i][1] * m._tab[1][j] + _tab[i][2] * m._tab[2][j];
    return ret;
}

// fais tourner la matrice autour de son centre v fois
matrice matrice::rotateMatrice(int v)
{
    int tab[3][3];
    for(int i=0;i<v;i++)
    {
        tab[0][0]=_tab[1][0];
        tab[0][1]=_tab[0][0];
        tab[0][2]=_tab[0][1];
        tab[1][0]=_tab[2][0];
        tab[1][1]=_tab[1][1];
        tab[1][2]=_tab[0][2];
        tab[2][0]=_tab[2][1];
        tab[2][1]=_tab[2][2];
        tab[2][2]=_tab[1][2];
    }
    matrice m2(tab);
    return m2;
}

// effectue l'addition de deux matrices

matrice matrice::addM(matrice m)
{
    matrice ret;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            ret._tab[i][j]=_tab[i][j]+m._tab[i][j];
        }
    }
    return ret;
}

// effectue l'addition d'une matrice et d'un tableau
matrice  matrice::addM(int tab[3][3])
{
    matrice ret;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            ret._tab[i][j]=_tab[i][j]+tab[i][j];
        }
    }
    return ret;
}

// effectue la multiplication d'une matrice et d'un tableau
matrice * matrice::multiplieM(int tab[3][3])
{
    matrice * ret=new matrice();
    for(int i = 0; i < 3; i++)
         for(int j = 0; j < 3; j++)
             ret->_tab[i][j]= _tab[i][0] * tab[0][j] + _tab[i][1] * tab[1][j] + _tab[i][2] * tab[2][j];
    return ret;
}


// effectue la convolution entre deux matrices
int * matrice::convolution(matrice m)
{
    int * x=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            *x+=_tab[i][j]*m._tab[i][j];
        }
    }
    return x;
}

// effectue la convolution entre une matrice et un tableau
void matrice::convolution(int tab[3][3],float & retour)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            retour+=tab[i][j]*_tab[i][j];
        }
    }
}
