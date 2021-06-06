#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <header/image.h>
#include <QLabel>
#include "header/matrice.h"
#include <QSlider>

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/types.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/core.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/photo.hpp>

#include <iostream>
#include <string>
#include "math.h"
#include <vector>
#include "header/connexe.h"
#include "Hough.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// contient les fonctions principales de l'algorithme

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //constructeur par défaut, initialise l'interface
    MainWindow(QWidget *parent = nullptr);
    //destructeur par défaut
    ~MainWindow();
    // calcul la direction et la valeur du gradient des pixels de l'image
    float calculateConv(matrice m, int tab[3][3], float sumv, std::vector<float> &gradient, std::vector<float> &direction);
    // effectue un premier passage pour appliquer le filtre sur l'image
    float firstPass(matrice m, cv::Mat Image, float sumv, std::vector<float> &gradient, std::vector<float> &direction);
    // effectue un second passage pour effectuer le seuillage de l'image
    cv::Mat secondPass(matrice m, cv::Mat Image, float sumv, int lowThreshold, int mlowThreshold, const std::vector<float> &gradient, const std::vector<float> &direction);
    // lance les passages et les opérations de post traitement
    cv::Mat ApplyFilter(matrice m, cv::Mat Image, int lowThreshold, int mlowThreshold);
    // lance ApplyFilter
    void images();
    // effectue un algorithme des K-moyennes sur les gradients d'une image traitée afin d'en extraire des bornes
    // utilisées dans le traitement par hystérésie
    cv::Point calculKmean(cv::Mat Image, std::vector<float> gradient);
    // calcul et stock l'ensemble des composantes connexes de l'image
    void ComposanteConnexe(cv::Mat Image,cv::Mat m);
    // relie les composantes connexes entre elle avec un morphisme
    cv::Mat morph(int tab[3][3], int type);
    // prépare l'image pour qu'elle soit affichée
    void affiche_image(cv::Mat m);
    // relie les composantes connexes entre elle avec une recherche du meilleur chemin
    void relieConnexe(int sens);

public slots:
    // fonctions exécutées quand on appuye sur les différents éléments de l'interface

    // change l'image en entrée de l'application
    void changeImage();

    // change la valeur de l'élément du filtre associé
    void change1(int i);
    void change2(int i);
    void change3(int i);
    void change4(int i);
    void change5(int i);
    void change6(int i);
    void change7(int i);
    void change8(int i);
    void change9(int i);

    // change le nombre de voisins pour le seuillage par voisinage
    void change(int i);
    // change le mode d'exécution
    void changeMode(int index);
    // change le mode de seuillage et met à jour l'affichage
    void changeFMode(int index);
    // lance le programme
    void execute();
    // modifie la valeur du seuil maximal
    void setMaxT(int i);
    //modifie la valeur du seuil minimal
    void setMinT(int i);
    // dessine les composantes connexes
    void drawConnexe();
    //applique les opérations de morphologie
    void ComputeMorph();
    void runClosure();
    void Computehough();
    void Computehoughseg();
    void ComputehoughCircle();


private:
    Ui::MainWindow *ui;

    //L'ensemble des éléments lié à l'interface
    QLabel * lmatrice;
    QSpinBox * v1;
    QSpinBox * v2;
    QSpinBox * v3;
    QSpinBox * v4;
    QSpinBox * v5;
    QSpinBox * v6;
    QSpinBox * v7;
    QSpinBox * v8;
    QSpinBox * v9;
    QSpinBox * MaxT;
    QPushButton * Connexes;
    QLabel * patch;
    QSpinBox * Sizepatch;
    QPushButton * validate;
    QPushButton * changeImageBtn;
    QPushButton * cptMorph;
    QPushButton * cptClos;
    QPushButton * hough;
    QPushButton * houghseg;
    QPushButton * houghCircle;
    QLabel * lcmode;
    QLabel * lfmode;
    QLabel * ltext;
    QLabel * ltext2;
    QComboBox * convMode;
    QComboBox * filteringmode;
    QSlider * Slider;
    QSlider * Slider2;

    // l'ensemble des paramêtres de départ de l'application

    //Image en entrée de l'application
    image * imEntree;

    //ensemble des valeurs prises par la matrice
    int iv1=-3;
    int iv2=-3;
    int iv3=5;
    int iv4=-3;
    int iv5=0;
    int iv6=5;
    int iv7=-3;
    int iv8=-3;
    int iv9=5;

    //mode d'éxécution (unilateral, bilateral, multilateral)
    int mode=0;
    //mode de seuillage
    int filtering_mode=0;

    //valeur minimale du seuillage
    int lowThreshold = 1;
    //nombre de voisin traité dans le seuillage par voisinage
    int sizepatch=1;
    //valeur maximale du seuillage
    int max_threshold=1;

    // les différents résultats en sortie de l'application

    //filtre utilisé
    matrice m;
    //image affichée en sortie de l'application
    image * Imsortie;

    //Image en niveau de gris sur laquelle est effectué la détection
    cv::Mat detected_edges;
    //l'ensemble des composantes connexes de l'image
    std::vector<connexe *> connexes;
    //Image noire et blanche servant à detecter les composantes connexes
    cv::Mat me;
    //Image réelle en sortie de l'application, utilisée dans le post traitement
    cv::Mat resultat;
};
#endif // MAINWINDOW_H
