#include "../header/fast.hpp"
#include "../header/feature.hpp"
#include "../header/mat.hpp"
#include "../header/draw.hpp"
#include <fstream>
#include <string>
#include <cmath>
#include <iostream>
#include <map>

FAST::FAST(Mat &_image, std::vector<Keypoint> &_keypoints, Mat &_descriptor)
    : image(_image), keypoints(_keypoints), descriptor(_descriptor) {}

void FAST::detectKeypoints()
{
    int size = (image.rows * image.cols * 3);
    for (int i = 0; i < size - 2; i += 3)
    {
        // identify the intensity of the pixel of interest
        float I = image.data[i];
        // calculate the position of the pixels using the fact that position in 1D = (cols * y + x)
        float x = (i / 3) % (image.cols); // horizontal
        float y = (i / 3) / (image.cols); // vertical
        if (x >= 3 && x < (image.cols - 3) && y >= 3 && y < (image.rows - 3))
        {
            std::pair<bool, unsigned char> keypointStatus = isPixelKeypoint(x, y, I);
            std::vector<Keypoint> allKeypoints;
            if (keypointStatus.first)
            {
                // append the detected pixel to the keypoints vector
                Keypoint keypoint = Keypoint(x, y, keypointStatus.second);
                keypoints.emplace_back(keypoint);
            }
            // //perform Non Maximum Suppression to remove redundant and adjacent keypoints
            // int radius = 3;
            // for (size_t i = 0; i < allKeypoints.size()-1; ++i)
            // {
            //     for (size_t j = i+1; j < allKeypoints.size(); ++j)
            //     {
            //         if (std::pow((allKeypoints[i].pt.x - allKeypoints[j].pt.x),2) + std::pow((allKeypoints[i].pt.y - allKeypoints[j].pt.y),2) < std::pow(radius, 2))
            //         {
            //             if (allKeypoints[i].response > allKeypoints[j].response)
            //             {
            //                 keypoints.emplace_back(allKeypoints[i]);
            //             }
            //             else if (allKeypoints[i].response == allKeypoints[j].response)
            //             {
            //                 keypoints.emplace_back(allKeypoints[i]);
            //                 keypoints.emplace_back(allKeypoints[j]);
            //             }
            //             else
            //             {
            //                 keypoints.emplace_back(allKeypoints[j]);
            //             }
            //         }
            //     }
            // }
        }
    }
}

void FAST::detectDescriptor()
{
}

std::pair<bool, unsigned char> FAST::isPixelKeypoint(int x, int y, float I)
{
    // Radius of the circle around the pixel = 3
    float threshold = 0.20 * I; // the intensity threshold to be considered an interest point
    // the pixels the circle touches are numbered from 1 to 16
    // to identify position of the pixel in a 1D matrix: (cols * y + x)
    // initially consider the pixels 1,5,9,13
    int cols = image.cols * 3;
    unsigned char intensityAt1 = image.data[cols * (y - 3) + x];
    unsigned char intensityAt5 = image.data[cols * y + (x + 3)];
    unsigned char intensityAt9 = image.data[cols * (y + 3) + x];
    unsigned char intensityAt13 = image.data[cols * y + (x - 3)];
    // if atleast 3 of them are above or below the intensity (+/-) threshold then continue, else return false
    unsigned char count = 0;
    if (intensityAt1 > I + threshold || intensityAt1 < I - threshold)
    {
        count++;
    }

    if (intensityAt5 > I + threshold || intensityAt5 < I - threshold)
    {
        count++;
    }

    if (intensityAt9 > I + threshold || intensityAt9 < I - threshold)
    {
        count++;
    }

    if (intensityAt13 > I + threshold || intensityAt13 < I - threshold)
    {
        count++;
    }

    if (count >= 3)
    {
        // consider the other pixels, now that atleast 3 surpass the threshold intensity
        unsigned char intensityAt2 = image.data[cols * (y - 3) + (x + 1)];
        unsigned char intensityAt3 = image.data[cols * (y - 2) + (x + 2)];
        unsigned char intensityAt4 = image.data[cols * (y - 1) + (x + 3)];
        unsigned char intensityAt6 = image.data[cols * (y + 1) + (x + 3)];
        unsigned char intensityAt7 = image.data[cols * (y + 2) + (x + 2)];
        unsigned char intensityAt8 = image.data[cols * (y + 3) + (x + 1)];
        unsigned char intensityAt10 = image.data[cols * (y + 3) + (x - 1)];
        unsigned char intensityAt11 = image.data[cols * (y + 2) + (x - 2)];
        unsigned char intensityAt12 = image.data[cols * (y + 1) + (x - 3)];
        unsigned char intensityAt14 = image.data[cols * (y - 1) + (x - 3)];
        unsigned char intensityAt15 = image.data[cols * (y - 2) + (x - 2)];
        unsigned char intensityAt16 = image.data[cols * (y - 3) + (x - 1)];
        // if there are atleast 12 contiguous pixels that surpass the threshold, return true, else false

        std::vector<unsigned char> circleIntensity({intensityAt1, intensityAt2, intensityAt3, intensityAt4,
                                                    intensityAt5, intensityAt6, intensityAt7, intensityAt8,
                                                    intensityAt9, intensityAt10, intensityAt11, intensityAt12,
                                                    intensityAt13, intensityAt14, intensityAt15, intensityAt16});

        for (int x = 0; x < 16; x++)
        {
            bool darker = true;
            bool brighter = true;

            for (int y = 0; y < 9; y++)
            {
                int circlePixel = circleIntensity[(x + y) % 16];

                if (!(circlePixel > I + threshold))
                {
                    brighter = false;
                    if (!darker)
                    {
                        break;
                    }
                }

                if (!(circlePixel < I - threshold))
                {
                    darker = false;
                    if (!brighter)
                    {
                        break;
                    }
                }
            }

            if (brighter || darker)
            {
                // sum of Absolute difference between the intensity I and the 16 adjacent pixels
                unsigned char V = abs(I - intensityAt1) + abs(I - intensityAt2) + abs(I - intensityAt3) + abs(I - intensityAt4) +
                                  abs(I - intensityAt5) + abs(I - intensityAt6) + abs(I - intensityAt7) + abs(I - intensityAt8) +
                                  abs(I - intensityAt9) + abs(I - intensityAt10) + abs(I - intensityAt11) + abs(I - intensityAt12) +
                                  abs(I - intensityAt13) + abs(I - intensityAt14) + abs(I - intensityAt15) + abs(I - intensityAt16);
                return {true, V};
            }
        }
        return {false, 0};
    }
    else
        return {false, 0};
}

std::vector<std::pair<float, std::vector<Keypoint>>> computePyramid(Mat &image)
{
    float scale[4] = {1, 0.5, 0.25, 0.125};
    std::vector<std::pair<float, std::vector<Keypoint>>> allKeypointsWithScale;
    for (int i = 0; i <= 3; i++)
    {
        Mat resized = resize(scale[i], image);
        Mat descriptor;
        std::vector<Keypoint> keypoints;
        FAST fast = FAST(resized, keypoints, descriptor);
        fast.detectKeypoints();
        allKeypointsWithScale.push_back({scale[i], keypoints});
        Draw draw = Draw();
        const std::string filename = "resized" + std::to_string(i) + ".ppm";
        draw.drawKeypoints(resized, fast.keypoints, filename);
    }
    return allKeypointsWithScale;
}

std::vector<std::pair<int,int>> FAST::detectAndCompute()
{
    std::vector<std::pair<float, std::vector<Keypoint>>> allKeypoints = computePyramid(image);
    std::vector<std::pair<int,int>> commonKeypoints;
    std::map<std::pair<int,int>,int> recurringKeypoints;
    for( auto& keypoints : allKeypoints){
        float scale = keypoints.first;
        for(auto& keypoint : keypoints.second){
            int x = keypoint.pt.x / scale;
            int y = keypoint.pt.y / scale;
            std::pair<int,int> coordinate = std::make_pair(x,y);
            recurringKeypoints[coordinate]++;
        }
    }
    for(auto& keypoint : recurringKeypoints){
        if(keypoint.second == 4){
            commonKeypoints.push_back(keypoint.first);
        }
    }
    return commonKeypoints;
}