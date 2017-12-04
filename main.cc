#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

#include "./filter.cc"
#include "./utils.cc"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
  VideoCapture cap("videos/Video_001.avi");

  namedWindow("original");
  namedWindow("background subtraction");
  namedWindow("background model");

  Mat frame;
  cap >> frame;

  const int cols = frame.size().width;
  const int rows = frame.size().height;
  const int channels = frame.channels();
  const int warm_up = 30;

  filter_mat bg_model(rows, vector<vector<filter>>(cols, vector<filter>(channels)));

  for (int k = 0; k < warm_up; k++) {
    cap >> frame;
    update_frame(bg_model, frame);
  }

  Mat dest = frame.clone();
  Mat bg = frame.clone();

  while (true) {
    cap >> frame;

    update_frame(bg_model, frame);
    get_model(bg_model, dest);
    get_background(bg_model, frame, bg);

    imshow("original", frame);
    imshow("background subtraction", bg);
    imshow("background model", dest);
    if (waitKey(30) >= 0)
      break;
  }

  return 0;
}
