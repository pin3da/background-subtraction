#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

#include "./filter.cc"
#include "./utils.cc"
#include "./metrics.cc"

using namespace cv;
using namespace std;

typedef vector<vector<vector<lms_filter>>> lms_mat;
typedef vector<vector<vector<corr_filter>>> corr_mat;

int main(int argc, char **argv) {
  VideoCapture cap("videos/highway/input/in%6d.jpg");
  // gt_reader gt_cap("videos/highway/groundtruth/gt%06d.png");
  VideoCapture gt_cap("videos/highway/groundtruth/gt%06d.png");

  namedWindow("cv-original");moveWindow("cv-original", 10, 50);
  namedWindow("cv-ground truth");moveWindow("cv-ground truth", 10, 350);
  namedWindow("cv-background subtraction"); moveWindow("cv-background subtraction", 410, 50);
  namedWindow("cv-background model"); moveWindow("cv-background model", 810, 50);
  namedWindow("cv-background subtraction CORR"); moveWindow("cv-background subtraction CORR", 410, 350);
  namedWindow("cv-background model CORR"); moveWindow("cv-background model CORR", 810, 350);

  Mat frame, gt;

  for (int k = 0; k < 500; k++) {
    cap >> frame; gt_cap >> gt;
  }

  const int cols = frame.size().width;
  const int rows = frame.size().height;
  const int channels = frame.channels();
  const int warm_up = 30;

  lms_mat lms_bg_model(rows, vector<vector<lms_filter>>(cols, vector<lms_filter> (channels)));
  corr_mat corr_bg_model(rows, vector<vector<corr_filter>>(cols, vector<corr_filter> (channels)));

  cap >> frame; gt_cap >> gt;

  init_model(lms_bg_model, frame);
  init_model(corr_bg_model, frame);

  for (int k = 0; k < warm_up; k++) {
    cap >> frame; gt_cap >> gt;
    update_frame(lms_bg_model, frame);
    update_frame(corr_bg_model, frame);
  }

  Mat dest = frame.clone();
  Mat bg = frame.clone();

  cout << fixed << setprecision(7);

  while (true) {
    cap >> frame; gt_cap >> gt;

    update_frame(lms_bg_model, frame);
    get_model(lms_bg_model, dest);
    get_background(lms_bg_model, frame, bg);
    imshow("cv-original", frame);
    imshow("cv-background subtraction", bg);
    imshow("cv-background model", dest);

    cout << f1_score(gt, bg) << "\t";


    update_frame(corr_bg_model, frame);
    get_model(corr_bg_model, dest);
    get_background(corr_bg_model, frame, bg);
    imshow("cv-background subtraction CORR", bg);
    imshow("cv-background model CORR", dest);

    imshow("cv-ground truth", gt);

    cout << f1_score(gt, bg) << endl;
    if (waitKey(30) >= 0)
      break;
  }
  return 0;
}
