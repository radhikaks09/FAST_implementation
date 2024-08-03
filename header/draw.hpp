#ifndef DRAW_HPP
#define DRAW_HPP
#include "mat.hpp"
#include "feature.hpp"
#include <vector>
#include <string>

class BFMatcher
{
};

class Draw
{
    public:
    void drawKeypoints(Mat &image, std::vector<Keypoint> &keypoints,const std::string& filename);
};

#endif