# Introduction

This library provides an ellipse detection method that can be used stably in industry.The detection of standard, obvious, complete, and larger ellipses in the image is very good and very fast. The implementation of this library is based on the paper https://arxiv.org/pdf/1810.03243v4.pdf .

Online experience: http://43.154.37.202



# Setup
## Ubuntu
### Install opencv
OpenCV needs to be installed through the source code, Ubuntu OpenCV installation can refer to the blog https://blog.csdn.net/Arthur_Holmes/article/details/100146463, Note that the version of OpenCV3 must be installed in this project, otherwise there may be compatibility problems.The server version does not require the installation of an image-related library.

### Install lapack
Lapack is a library of matrix operations and requires source code installation.
Download [lapack](https://github.com/Reference-LAPACK/lapack/archive/refs/tags/v3.9.1.tar.gz)，Lapack is written by gfortran, and you need to `sudo apt-get install gfortran` to compile properly.

Lapack is a library of matrix operations and requires source code installation.



```
tar -xzvf lapack-3.9.1.tar.gz && cd lapack-3.9.1
mkdir build && cd build
cmake ..
make -j7
sudo make install
sudo ldconfig
cd ..
sudo cp LAPACKE/include/*.h /usr/local/include/
```

### Install ellipse-detection
```
git clone https://github.com/memory-overflow/standard-ellipse-detection.git
cd standard-ellipse-detection
mkdir build && cd build
cmake ..
make
sudo make install
```

# Interface & Manual
`#include "ellipse_detection/detect.h"`

Interface
```
bool detectEllipse(const uint8_t *image, int height, int width,
                   std::vector<std::shared_ptr<Ellipse> > &ells,
                   int polarity = 0, double line_width = 2.0);
```
- Input Parameter:
    - image : raw image, gray map, color map convert to gray map, and again to 1D array input
    - height
    - width
    - polarity : -1, 0, 1 (default 0)
    - line_width : Ellipse Width, Unit Pixel (default recommended)
- Output
    - ells : Detected Ellipse List

Ellipse Structure
```
Pointd o; // Ellipse Center
double a, b; // short half-axis len, long half-axis len
double phi; // elliptical deflection, (radian)
int polarity; // elliptical polarity
double goodness; // elliptical rating
double coverangle; // elliptical angle completeness
std::vector<Pixel> inliers; // pixels that make up the pixel
```



# Test
A test tool is provided to see the effectiveness of the algorithm.The desktop version of the operating system is required to display the picture, and if it is the server version of the operating system, you need to comment out the imshow section.
```
cmake3 .. -DBUILD_TESTING=ON
make
./bin/testdetect [image_dir1] [image_dir2] [image_dir3] ...
```




If you have any questions, please contact zhengaohong@gmail.com, wechat: islands___
