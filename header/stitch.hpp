#ifndef STITCH_HPP
#define STITCH_HPP
#include "mat.hpp"
#include "feature.hpp"
#include <vector>
#include <string>

class BFMatcher
{
};

class Stitcher
{
    public:
    void drawKeypoints(Mat &image, std::vector<Keypoint> &keypoints,const std::string& filename);
};

#endif