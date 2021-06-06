#include "header/fileselector.h"
#include <QFileDialog>


// ouvre une fenêtre de sélection de fichier
fileselector::fileselector()
{
    filename = QFileDialog::getOpenFileName(this,
         QFileDialog::tr("Open Image"), "/home/", QFileDialog::tr("Image Files (*.png , *.jpg , *.jpeg)")).toUtf8().constData();
}

// renvoie le nom du fichier à lire
string fileselector::getFileName()
{
    return filename;
}
