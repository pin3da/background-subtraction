
#include "opencv2/opencv.hpp"
#include "./filter.cc"

using namespace std;

void update_frame(filter_mat &bg_model, cv::Mat &frame) {
  const int cols = frame.size().width;
  const int rows = frame.size().height;
  const int channels = frame.channels();

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int c = 0; c < channels; c++) {
        bg_model[i][j][c].update(frame.at<cv::Vec3b>(i, j)[c]);
      }
    }
  }
}

void get_model(filter_mat &bg_model, cv::Mat &frame) {
  const int cols = frame.size().width;
  const int rows = frame.size().height;
  const int channels = frame.channels();

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int c = 0; c < channels; c++) {
        frame.at<cv::Vec3b>(i, j)[c] = round(bg_model[i][j][c].mean);
      }
    }
  }
}

void get_background(filter_mat &bg_model, cv::Mat &ref, cv::Mat &dest) {
  const int cols = ref.size().width;
  const int rows = ref.size().height;
  const int channels = ref.channels();

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int c = 0; c < channels; c++) {
        auto x = ref.at<cv::Vec3b>(i, j)[c];
        dest.at<cv::Vec3b>(i, j)[c] = bg_model[i][j][c].eval(x);
      }
    }
  }
}