#include <iostream>
#include "segmentation.hpp"

void Region::AddPixel(cv::Point2i pixel_pos, cv::Vec3b pixel_color)
{
    _pixels.push_back(pixel_pos);
    for (unsigned int i = 0; i < 3; ++i)
    {
        _sum[i] += pixel_color.val[i];
    }
    _count++;
}

std::set<unsigned int> Region::AbsorbRegion(Region &r)
{
    std::set<unsigned int> n = r.GetNeighbours();
    _neighbours.erase(r._id);
    for (auto pixel : r.GetPixels())
        _pixels.push_back(pixel);

    for (auto neigh : r.GetNeighbours())
    {
        if (neigh != _id)
            _neighbours.insert(neigh);
    }
    for (unsigned int i = 0; i < 3; ++i)
    {
        _sum[i] += r.GetSum(i);
    }
    _count += r.GetCount();
    CalcAvg();
    return n;
}

void Region::CalcAvg()
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _avg.val[i] = _sum[i] / _count;
    }
}

std::vector<cv::Point2i> Region::GetPixels()
{
    return _pixels;
}

int Region::GetSum(int channel)
{
    return _sum[channel];
}

cv::Point2i Region::GetMarkedPixel()
{
    cv::Point2i p = _marked_pixels.front();
    _marked_pixels.pop_front();
    return p;
}

void Region::AddMarkedPixel(cv::Point2i pixel)
{
    _marked_pixels.push_back(pixel);
}

void Region::AddBorderPixel(cv::Point2i pixel)
{
    _border_pixels.push_back(pixel);
}

std::deque<cv::Point2i> &Region::GetBorderPixels()
{
    return _border_pixels;
}

bool Region::MarkedPixelEmpty()
{
    return _marked_pixels.empty();
}

void Region::AddNeighbour(unsigned int id)
{
    if (id != _id)
        _neighbours.insert(id);
}

void Region::ChangeNeighbour(unsigned int old_id, unsigned int new_id)
{
    if (new_id != _id)
    {
        _neighbours.erase(old_id);
        _neighbours.insert(new_id);
    }
}
std::set<unsigned int> &Region::GetNeighbours()
{
    return _neighbours;
}

cv::Vec3b Region::GetColor()
{
    return _avg;
}

int Region::GetCount()
{
    return _count;
}