/*
 *Copyright: Copyright (c) 2019
 *Created on 2019-6-31
 *Author:zhengaohong@zgheye.cc
 *Version 1.0.1
*/

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ellipse_detection/types.hpp"
#include "ellipse_detection/detect.h"

using namespace std;
using namespace zgh;

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cout << "[Usage]: testdetect [image_dir1] [image_dir2] [image_dir3] ..." << std::endl;
    }

    for (int i = 1; i < argc; ++i)
    {
        cv::Mat board = cv::imread(argv[i]);
        cv::Mat image = cv::imread(argv[i], cv::IMREAD_GRAYSCALE);
        // cv::resize(image, image, cv::Size(image.cols / 2, image.rows / 2));
        // cv::resize(board, board, cv::Size(board.cols / 2, board.rows / 2));
        cv::imshow("origin image", image);
        vector<shared_ptr<Ellipse>> ells;
        int row = image.rows;
        int col = image.cols;
        double width = 2.0;
        FuncTimerDecorator<int>("detectEllipse")(detectEllipse, image.data, row, col, ells, NONE_POL, width);
        cout << "Find " << ells.size() << " ellipse(s)" << endl;
        for (int i = 0; i < (int)ells.size(); ++i)
        {
            auto ell = ells[i];
            std::cout << "coverangle : " << ell->coverangle
                      << ",\tgoodness : " << ell->goodness
                      << ",\tpolarity : "
                      << ell->polarity
                      << endl;
            cv::ellipse(board,
                        cv::Point(ell->o.y, ell->o.x),
                        cv::Size(ell->a, ell->b),
                        rad2angle(PI_2 - ell->phi),
                        0,
                        360,
                        cv::Scalar(0, 255, 0),
                        width,
                        8,
                        0);
        }
        cv::imshow("detected result", board);
        cv::waitKey(0);
    }
    return 0;
}
