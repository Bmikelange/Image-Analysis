#include <string>
using namespace std;

#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QObject>
#include <QWidget>


// contient les fonctions nécessaires à l'écriture
class fileselector : public QWidget
{
public:
    fileselector();
    string getFileName();
private:
    string filename;
};

#endif // FILESELECTOR_H
