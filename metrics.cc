
#include "opencv2/opencv.hpp"
#include "./debug.cc"

const cv::Vec3b __white(255, 255, 255);
const cv::Vec3b __black(0, 0, 0);

double true_positives(const cv::Mat &expected, const cv::Mat &current) {
    int ans = 0;
    for (int i = 0; i < expected.rows; i++) {
        for (int j = 0; j < expected.cols; j++) {
            ans += (expected.at<cv::Vec3b>(i, j) == __white && current.at<cv::Vec3b>(i, j) == __white);
        }
    }
    return double(ans);
}

double false_positives(const cv::Mat &expected, const cv::Mat &current) {
    int ans = 0;
    for (int i = 0; i < expected.rows; i++) {
        for (int j = 0; j < expected.cols; j++) {
            ans += (expected.at<cv::Vec3b>(i, j) == __black && current.at<cv::Vec3b>(i, j) == __white);
        }
    }
    return double(ans);
}

double false_negatives(const cv::Mat &expected, const cv::Mat &current) {
    int ans = 0;
    for (int i = 0; i < expected.rows; i++) {
        for (int j = 0; j < expected.cols; j++) {
            ans += (expected.at<cv::Vec3b>(i, j) == __white && current.at<cv::Vec3b>(i, j) == __black);
        }
    }
    return double(ans);
}

double precision(const cv::Mat &expected, const cv::Mat &current) {
    double tp = true_positives(expected, current);
    double fp = false_positives(expected, current);
    return tp / (tp + fp);
}

double recall(const cv::Mat &expected, const cv::Mat &current) {
    double tp = true_positives(expected, current);
    double fn = false_negatives(expected, current);
    return tp / (tp + fn);
}

double f1_score(const cv::Mat &expected, const cv::Mat &current) {
    double pre = precision(expected, current);
    double rec = recall(expected, current);
    double ans = 2.0 * pre * rec / (pre + rec);
    return ans;
}