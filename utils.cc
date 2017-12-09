
#include "opencv2/opencv.hpp"
#include "./filter.cc"

using namespace std;

template<class filter>
void update_frame(vector<vector<vector<filter>>> &bg_model, cv::Mat &frame) {
  const int cols = frame.size().width;
  const int rows = frame.size().height;
  const int channels = frame.channels();

#pragma omp parallel for
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int c = 0; c < channels; c++) {
        bg_model[i][j][c].update(frame.at<cv::Vec3b>(i, j)[c]);
      }
    }
  }
}

template<class filter>
void get_model(vector<vector<vector<filter>>> &bg_model, cv::Mat &frame) {
  const int cols = frame.size().width;
  const int rows = frame.size().height;
  const int channels = frame.channels();

#pragma omp parallel for
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int c = 0; c < channels; c++) {
        frame.at<cv::Vec3b>(i, j)[c] = round(bg_model[i][j][c].mean);
      }
    }
  }
}


template<class filter>
void init_model(vector<vector<vector<filter>>> &bg_model, cv::Mat &frame) {
  const int cols = frame.size().width;
  const int rows = frame.size().height;
  const int channels = frame.channels();

#pragma omp parallel for
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int c = 0; c < channels; c++) {
        bg_model[i][j][c].mean = frame.at<cv::Vec3b>(i, j)[c];
      }
    }
  }
}


const cv::Vec3b white(255, 255, 255);
const cv::Vec3b black(0, 0, 0);

template<class filter>
void get_background(vector<vector<vector<filter>>> &bg_model, cv::Mat &ref, cv::Mat &dest) {
  const int cols = ref.size().width;
  const int rows = ref.size().height;
  const int channels = ref.channels();

#pragma omp parallel for
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int sum = 0;
      for (int c = 0; c < channels; c++) {
        auto x = ref.at<cv::Vec3b>(i, j)[c];
        sum += bg_model[i][j][c].eval(x);
      }
      if (sum >= 400)
        dest.at<cv::Vec3b>(i, j) = white;
      else
        dest.at<cv::Vec3b>(i, j) = black;
    }
  }
}