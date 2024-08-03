#ifndef FAST_HPP
#define FAST_HPP

#include "mat.hpp"
#include "feature.hpp"

class FAST
{
public:
    std::vector<Keypoint> &keypoints;
    Mat &image, &descriptor;
    FAST(Mat &image, std::vector<Keypoint> &keypoints, Mat &descriptor);
    // Check if the given pixel coordinates and intensity (x, y, I) is a keypoint
    // Return true if it's a keypoint, otherwise false
    std::pair<bool, unsigned char> isPixelKeypoint(int x, int y, float I);
    // Perform keypoint detection algorithm using the image data
    // Store the detected keypoints in the 'keypoints' vector
    void detectKeypoints();
    // Compute descriptors for the detected keypoints
    // Store the descriptors in the 'descriptor' Mat object
    void detectDescriptor();
    std::vector<std::pair<int,int>> detectAndCompute();
};

#endif