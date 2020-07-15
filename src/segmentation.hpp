#include <opencv2/core/types.hpp>

#include <queue>
#include <set>

// structure de données servant à stocker nos région de pixels
class Region
{
public:
    //constructeur
    Region(unsigned int id) : _marked_pixels(), _border_pixels(), _pixels(), _neighbours(), _id(id), _avg() {}
    //ajoute un pixel à la région 
    void AddPixel(cv::Point2i pixel_pos, cv::Vec3b pixel_color);
    //fusionne deux régions
    std::set<unsigned int> AbsorbRegion(Region &r);
    //renvoie la liste des pixels d'une région
    std::vector<cv::Point2i> GetPixels();
    //renvoie le pixel qui va être traité
    cv::Point2i GetMarkedPixel();
    //vérifie si il reste des pixels à traiter
    bool MarkedPixelEmpty();
    //la liste des bordures de la zone
    std::deque<cv::Point2i> &GetBorderPixels();
    //ajoute un pixel à traiter
    void AddMarkedPixel(cv::Point2i pixel);
    //ajoute un pixel à la bordure
    void AddBorderPixel(cv::Point2i pixel);
    //ajoute une région comme voisine de la région en cours
    void AddNeighbour(unsigned int id);
    //change le voisin i de la région en cours (uniquement pendant le merge)
    void ChangeNeighbour(unsigned int old_id, unsigned int new_id);
    //renvoie la liste des voisins de la région
    std::set<unsigned int> &GetNeighbours();
    //calcule la moyenne de couleur de la région
    void CalcAvg();
    //renvoie la moyenne de couleur de la région
    cv::Vec3b GetColor();
    //renvoie le nombre de pixels dans la région
    int GetCount();
    //L'ID de la région
    unsigned int _id;

protected:
    //renvoie la somme pour tous les pixels du chanel de couleur i (r,g,b)
    int GetSum(int channel);

private:
    // la liste des pixels à traiter
    std::deque<cv::Point2i> _marked_pixels;
    // la liste des bordures de la région
    std::deque<cv::Point2i> _border_pixels;
    //la liste de tous les pixels de la région
    std::vector<cv::Point2i> _pixels;
    //la liste des voisins de la région
    std::set<unsigned int> _neighbours;
    //la somme des couleurs de tous les pixels
    int _sum[3] = {0};
    //la moyenne de couleur sur la région
    cv::Vec3b _avg;
    //le nombre de pixels dans la région
    int _count = 0;
};
