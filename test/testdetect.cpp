/*
 *Copyright: Copyright (c) 2019
 *Created on 2019-6-31
 *Author:zhengaohong@zgheye.cc
 *Version 1.0.1
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ellipse_detection/types.hpp"
#include "ellipse_detection/detect.h"

using namespace std;
using namespace zgh;

std::vector<std::string> listFilesInDirectory(const std::string& path);
std::string createNewFilePath(const std::string& originalPath, const std::string& newDir, const std::string& suffix);
std::string createNewFilePath(const std::string& originalPath, const std::string& newDir, const std::string& suffix, const std::string& extension);

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cout << "[Usage]: testdetect [dataset_directory]" << std::endl;
        return -1;
    }

    // Directory Initialize
    std::string directoryPath = argv[1];
    std::string newDir = "detect";
    auto fileList = listFilesInDirectory(directoryPath);



    // Main Detection Loop
    for (const auto& file : fileList) {
        // [1] Directory Initialize
            // master directory Path
        std::cout << file << std::endl;
        std::string originalFilePath = directoryPath + "/" + file;
        std::string detectPath = directoryPath + "/" + newDir;
            // Result image Path
        std::string newFilePath = createNewFilePath(originalFilePath, newDir, "_detect");
            // Result text Path
        std::string newTextFilePath = createNewFilePath(originalFilePath, newDir, "_detect", ".txt");
        std::ofstream outFile(newTextFilePath);
        if (!outFile.is_open()) {
            std::cout << "Could not open file for writing: " << newTextFilePath << "\n" << std::endl;
        }

        // [2] Image Open
        cv::Mat board = cv::imread(originalFilePath);
        cv::Mat image = cv::imread(originalFilePath ,cv::IMREAD_GRAYSCALE);
        if (image.empty()) {
            std::cout << "Could not open or find the image: " << originalFilePath << std::endl;
            continue;
        }

        // [3] Image Detection
        vector<shared_ptr<Ellipse> > ells;
        int row = image.rows;
        int col = image.cols;
        double width = 2.0;
        FuncTimerDecorator<int>("detectEllipse")(detectEllipse, image.data, row, col, ells, NONE_POL, width);
        cout << "Find " << ells.size() << " ellipse(s)" << endl;
        outFile << (int)ells.size() << "\n";
        for (int i = 0; i < (int)ells.size(); ++i) {
            auto ell = ells[i];
            std::cout << "centerX : " << ell->o.x
                      << "\tcenterY : " << ell->o.y
                      << "\tshort : " << ell->a
                      << "\tlong : " << ell->b
                      << "\tpi/2-phi(deg) : " << rad2angle(PI_2 - ell->phi)
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
            outFile << ell->o.x << "\t" << ell->o.y << "\t";
            outFile << ell->a   << "\t" << ell->b   << "\t";
            outFile << rad2angle(PI_2 - ell->phi)   << "\n";
        }

        // [4] Detection File Save Path inspection
        if (opendir(detectPath.c_str()) == NULL) {  // Directory exist ensure
            if (mkdir(detectPath.c_str(), 0777) == -1) {    // if not exist, attempt to create
                std::cerr << "Failed to create directory: " << detectPath << std::endl;
                continue;
            }
        }

        // [5] Result Save
        if (cv::imwrite(newFilePath, board)) {
            std::cout << "Saved: " << newFilePath << std::endl;
        } else {
            std::cout << "Could not save the image: " << newFilePath << std::endl;
        }

        outFile.close();
        std::cout << "Saved ellipse information to: " << newTextFilePath << "\n" << std::endl;
    }

    return 0;
}

std::vector<std::string> listFilesInDirectory(const std::string& path) {
    std::vector<std::string> fileList;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) { // Check if it's a regular file
                fileList.push_back(ent->d_name);
            }
        }
        closedir(dir);
    } else {    // Could not open directory
        perror("");
    }

    return fileList;
}

std::string createNewFilePath(const std::string& originalPath, const std::string& newDir, const std::string& suffix) {
    std::size_t lastSlashPos = originalPath.rfind('/');
    
    if (lastSlashPos == std::string::npos){
        return originalPath; // No directory found
    }
    std::string directory = originalPath.substr(0, lastSlashPos);
    std::string fileName = originalPath.substr(lastSlashPos + 1);
    std::size_t dotPos = fileName.rfind('.');
    
    if (dotPos == std::string::npos){
        return directory + "/" + newDir + "/" + fileName + suffix; // No extension found
    }
    std::string nameWithoutExtension = fileName.substr(0, dotPos);
    std::string extension = fileName.substr(dotPos);
    
    return directory + "/" + newDir + "/" + nameWithoutExtension + suffix + extension;
}

std::string createNewFilePath(const std::string& originalPath, const std::string& newDir, const std::string& suffix, const std::string& extension) {
    std::size_t lastSlashPos = originalPath.rfind('/');
    
    if (lastSlashPos == std::string::npos){
        return originalPath; // No directory found
    }
    std::string directory = originalPath.substr(0, lastSlashPos);
    std::string fileName = originalPath.substr(lastSlashPos + 1);
    std::size_t dotPos = fileName.rfind('.');
    
    if (dotPos == std::string::npos){
        return directory + "/" + newDir + "/" + fileName + suffix + extension; // No extension found
    }
    std::string nameWithoutExtension = fileName.substr(0, dotPos);
    
    return directory + "/" + newDir + "/" + nameWithoutExtension + suffix + extension;
}
