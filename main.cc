#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace cv;

int main(int argc, char **argv) {
    VideoCapture cap("videos/Video_001.avi");

    namedWindow("test", 1);
    while (true) {
        Mat frame;
        cap >> frame;
        imshow("test", frame);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
