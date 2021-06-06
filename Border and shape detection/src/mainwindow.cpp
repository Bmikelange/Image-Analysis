#include "header/mainwindow.h"
#include "ui_mainwindow.h"
#include "header/fileselector.h"
#include <string>
#include "header/matrice.h"
#include <QDebug>
#include <QDir>
#include <header/connexe.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // fixe la taille de l'application
    setFixedSize(1270, 570);

    // déclare les éléments de l'interface
    lmatrice = new QLabel("Write the matrice you\n want to use",this);
    patch = new QLabel("patch size :",this);
    lcmode = new QLabel("choose the mode",this);
    lfmode = new QLabel("choose the filtering mode",this);
    ltext = new QLabel("max seuil :",this);
    ltext2 = new QLabel("min seuil :",this);
    convMode =  new QComboBox(this);
    filteringmode = new QComboBox(this);
    v1= new QSpinBox(this);
    v2= new QSpinBox(this);
    v3= new QSpinBox(this);
    v4= new QSpinBox(this);
    v5= new QSpinBox(this);
    v6= new QSpinBox(this);
    v7= new QSpinBox(this);
    v8= new QSpinBox(this);
    v9= new QSpinBox(this);
    Sizepatch=new QSpinBox(this);
    validate = new QPushButton("run",this);
    Connexes = new QPushButton("affiche connexe",this);
    cptMorph = new QPushButton("compute Morph", this);
    cptClos = new QPushButton("closure", this);
    hough = new QPushButton("hough line", this);
    houghseg = new QPushButton("hough seg", this);
    houghCircle = new QPushButton("hough cir", this);
    Slider = new QSlider(Qt::Horizontal,this);
    Slider2 = new QSlider(Qt::Horizontal,this);
    changeImageBtn=new QPushButton("change Image",this);

    // définis la taille des éléments de l'interface
    lmatrice->setFixedSize(200,50);
    lcmode->setFixedSize(200,25);
    lfmode->setFixedSize(200,25);
    convMode->setFixedSize(150,25);
    filteringmode->setFixedSize(150,25);
    ltext->setFixedSize(0,0);
    ltext2->setFixedSize(0,0);
    v1->setFixedSize(50,25);
    v2->setFixedSize(50,25);
    v3->setFixedSize(50,25);
    v4->setFixedSize(50,25);
    v5->setFixedSize(50,25);
    v6->setFixedSize(50,25);
    v7->setFixedSize(50,25);
    v8->setFixedSize(50,25);
    v9->setFixedSize(50,25);
    Sizepatch->setFixedSize(0,0);
    patch->setFixedSize(0,0);
    validate->setFixedSize(50,25);
    Slider->setFixedSize(0,0);
    Slider2->setFixedSize(0,0);
    Connexes->setFixedSize(150,25);
    cptMorph->setFixedSize(150,25);
    cptClos->setFixedSize(150,25);
    hough->setFixedSize(150,25);
    houghseg->setFixedSize(150,25);
    houghCircle->setFixedSize(150,25);

    //rempli les élements de l'interface contenant plusieurs choix
    convMode->addItem("unidirectional");
    convMode->addItem("bidirectional");
    convMode->addItem("multidirectional");
    filteringmode->addItem("average of the image");
    filteringmode->addItem("average of the neighboring pixels");
    filteringmode->addItem("hysteresis");
    filteringmode->addItem("hysteresis with Kmean treshold");

    //positionne les différents éléments sur l'image
    lcmode->move(0,25);
    convMode->move(0,55);
    lfmode->move(0,85);
    filteringmode->move(0,115);
    lmatrice->move(0,145);
    v1->move(0,200);
    v2->move(55,200);
    v3->move(110,200);
    v4->move(0,230);
    v5->move(55,230);
    v6->move(110,230);
    v7->move(0,260);
    v8->move(55,260);
    v9->move(110,260);
    validate->move(110,290);
    Sizepatch->move(75,290);
    patch->move(0,290);
    ltext->move(750,520);
    ltext2->move(750,540);
    Connexes->move(20,380);
    cptMorph->move(20,410);
    hough->move(20,470);
    houghseg->move(20,500);
    houghCircle->move(20,530);
    cptClos->move(20,440);
    Slider->move(850,520);
    Slider2->move(850,540);
    changeImageBtn->move(400,520);

    // définis les valeurs minimum et maximum des différent éléments numériques de l'image
    v1->setMinimum(-1000);
    v2->setMinimum(-1000);
    v3->setMinimum(-1000);
    v4->setMinimum(-1000);
    v5->setMinimum(-1000);
    v6->setMinimum(-1000);
    v7->setMinimum(-1000);
    v8->setMinimum(-1000);
    v9->setMinimum(-1000);
    Slider->setMaximum(100);
    Slider->setMinimum(1);
    Slider2->setMaximum(99);
    Slider2->setMinimum(1);
    Sizepatch->setMinimum(1);
    v1->setValue(-3);
    v2->setValue(-3);
    v3->setValue(5);
    v4->setValue(-3);
    v5->setValue(0);
    v6->setValue(5);
    v7->setValue(-3);
    v8->setValue(-3);
    v9->setValue(5);


    // définis et crée les éléments d'affichage et de gestion de l'image
    imEntree= new image();
    imEntree->setParent(this);
    imEntree->move(200,10);
    Imsortie= new image();
    Imsortie->setParent(this);
    Imsortie->move(750,10);
    imEntree->setimgIn(cv::Mat::zeros(cv::Size(500,500), CV_32S),1);
    Imsortie->setimgIn(cv::Mat::zeros(cv::Size(500,500), CV_32S),1);



    // lie les fonctions exécutés en modifiant un élément de l'interface
    // à l'élément concerné
    QObject::connect(validate, SIGNAL(clicked()), this, SLOT(execute()));
    QObject::connect(Connexes, SIGNAL(clicked()), this, SLOT(drawConnexe()));
    QObject::connect(cptMorph, SIGNAL(clicked()), this, SLOT(ComputeMorph()));
    QObject::connect(cptClos, SIGNAL(clicked()), this, SLOT(runClosure()));
    QObject::connect(hough, SIGNAL(clicked()), this, SLOT(Computehough()));
    QObject::connect(houghseg, SIGNAL(clicked()), this, SLOT(Computehoughseg()));
    QObject::connect(houghCircle, SIGNAL(clicked()), this, SLOT(ComputehoughCircle()));
    QObject::connect(changeImageBtn, SIGNAL(clicked()), this, SLOT(changeImage()));
    QObject::connect(v1, SIGNAL(valueChanged(int)), this, SLOT(change1(int)));
    QObject::connect(v2, SIGNAL(valueChanged(int)), this, SLOT(change2(int)));
    QObject::connect(v3, SIGNAL(valueChanged(int)), this, SLOT(change3(int)));
    QObject::connect(v4, SIGNAL(valueChanged(int)), this, SLOT(change4(int)));
    QObject::connect(v5, SIGNAL(valueChanged(int)), this, SLOT(change5(int)));
    QObject::connect(v6, SIGNAL(valueChanged(int)), this, SLOT(change6(int)));
    QObject::connect(v7, SIGNAL(valueChanged(int)), this, SLOT(change7(int)));
    QObject::connect(v8, SIGNAL(valueChanged(int)), this, SLOT(change8(int)));
    QObject::connect(v9, SIGNAL(valueChanged(int)), this, SLOT(change9(int)));
    QObject::connect(Sizepatch, SIGNAL(valueChanged(int)), this, SLOT(change(int)));
    QObject::connect(convMode, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMode(int)));
    QObject::connect(filteringmode, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFMode(int)));
    QObject::connect(Slider, SIGNAL(valueChanged(int)), this, SLOT(setMaxT(int)));
    QObject::connect(Slider2, SIGNAL(valueChanged(int)), this, SLOT(setMinT(int)));
}

void MainWindow::changeImage() {
    fileselector file;
    std::string filename = file.getFileName();
    if(!filename.empty())
    {
        cv::Mat input_image = cv::imread(filename, cv::IMREAD_COLOR);
        imEntree->setimgIn(input_image,1);
    }
}

void MainWindow::setMaxT(int i){
    max_threshold=i;
    execute();
}

void MainWindow::setMinT(int i){
    lowThreshold=i;
    execute();
}

void MainWindow::change(int i) {
    sizepatch=i;
}

void MainWindow::change1(int i) {
    iv1=i;
}

void MainWindow::change2(int i) {
    iv2=i;
}

void MainWindow::change3(int i) {
    iv3=i;
}

void MainWindow::change4(int i) {
    iv4=i;
}

void MainWindow::change5(int i) {
    iv5=i;
}

void MainWindow::change6(int i) {
    iv6=i;
}

void MainWindow::change7(int i) {
    iv7=i;
}

void MainWindow::change8(int i) {
    iv8=i;
}

void MainWindow::change9(int i) {
    iv9=i;
}

void MainWindow::changeMode(int index)
{
    mode=index;
}

void MainWindow::changeFMode(int index)
{
    filtering_mode=index;
    if(index==0)
    {
        Sizepatch->setFixedSize(0,0);
        patch->setFixedSize(0,0);
        validate->move(110,290);
        Slider->setFixedSize(0,0);
        Slider2->setFixedSize(0,0);
        ltext->setFixedSize(0,0);
        ltext2->setFixedSize(0,0);
    }
    if(index==1)
    {
        Sizepatch->setFixedSize(50,25);
        patch->setFixedSize(100,25);
        validate->move(110,315);
        Slider->setFixedSize(0,0);
        Slider2->setFixedSize(0,0);
        ltext->setFixedSize(0,0);
        ltext2->setFixedSize(0,0);
    }
    if(index==2)
    {
        Sizepatch->setFixedSize(0,0);
        patch->setFixedSize(0,0);
        validate->move(110,290);
        Slider->setFixedSize(400,20);
        Slider2->setFixedSize(400,20);
        ltext->setFixedSize(100,20);
        ltext2->setFixedSize(100,20);
    }
    if(index==3)
    {
        Sizepatch->setFixedSize(0,0);
        patch->setFixedSize(0,0);
        validate->move(110,290);
        Slider->setFixedSize(0,0);
        Slider2->setFixedSize(0,0);
        ltext->setFixedSize(0,0);
        ltext2->setFixedSize(0,0);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

