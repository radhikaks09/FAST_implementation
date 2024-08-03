# Implementation of FAST Algorithm

## Description
This project aims to implement the FAST (Feature from Accelerated Segment Test) Algorithm in Image Processing from scratch using C++.
The FAST Algorithm is commonly used to identify key features in an image, using a corner detection method. These extracted features can then be used to track and map objects across multiple images. The original goal was to use this implementation of the technique to identify common keypoints across different images and stitch them together to help in recognising the bigger picture.

## Project Structure
This project is organised into two sections:
1. header
2. source

The header dir contains the following header files:
1. mat.hpp
2. feature.hpp
3. fast.hpp
4. draw.hpp

The source dir contains the following implementation files:
1. mat.cpp
2. feature.cpp
3. fast.cpp
4. draw.cpp
5. main.cpp

## Functionality
#### MAT
This class contains constructors and methods for reading and storing .ppm images to a matrix format and writing them back to a .ppm format. Further, it contains methods for processing images such as converting to grayscale and resizing the images in different scales.
#### FEATURE
This class contains definitions and constructors for a point and keypoints.
#### FAST
This class contains methods and implementations to identify the keypoints in an input image and determine if the same keypoints persist when the image is scaled in different ratios. The keypoints common to all the scales are presented as the final set of keypoints.
#### DRAW
This class contains a method that draws the identified keypoints on the input image.

## Running the Project
1. Compile the project using one of the following methods:
    1. cd into source dir then run the compilation command <br/>
    `cd source` <br/>
    `g++ -std=c++11 main.cpp mat.cpp stitch.cpp fast.cpp feature.cpp` <br/>
   or
    2. run the following command from the main project directory <br/>
    `g++ -std=c++11 source/*.cpp`
2. Run the project using the following command: </br>
    `./a.out <filename>.ppm` <br/>
    Make sure the image given as input in the commandline is in the .ppm (P3) format.

## Output
The output to be expected from running this project, giving the desired image as input, is the same image with the identified keypoints drawn onto the image.