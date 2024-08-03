#include <iostream>
#include <fstream>
#include <vector>
#include "../header/mat.hpp"
#include "../header/draw.hpp"
#include "../header/fast.hpp"
#include "../header/feature.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];
    if (filename.empty()) {
        std::cerr << "Error: Could not open or find the image.\n";
        return 1;
    }
    Mat mat = Mat(filename);
    Mat gray = convertToGray(mat);
    // Write the grayscale image to a new PPM file
    Draw draw = Draw();
    Mat descriptor;
    std::vector<Keypoint> keypoints;
    FAST fast = FAST(gray,keypoints,descriptor);
    std::vector<std::pair<int,int>> commonKeypoint = fast.detectAndCompute();
    draw.drawKeypoints(gray,fast.keypoints,"output");
    Mat resized = resize(0.125,gray);
    writePPM(resized,"resized.ppm");
    return 0;
}