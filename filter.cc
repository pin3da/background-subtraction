#ifndef CFILTER
#define CFILTER

#include <vector>
#include <cmath>
#include <iostream>
#include <memory>

#include "debug.cc"

const double pi = acos(-1);

double gaussian_dist(double mean, double sig, double x) {
  const double norm = 1.0 / (sqrt(2 * pi ) * sig);

  double dist = mean - x;
  return norm * exp(- (dist * dist) / (2 * sig * sig));
}

struct lms_filter {
  double mean, sigma, eta, threshold;
  lms_filter() : mean(10), sigma(0.2), eta(0.2), threshold(50) {}
  lms_filter(double mean, double sigma, double eta, double threshold) :
    mean(mean),
    sigma(sigma),
    eta(eta),
    threshold(threshold)
  {}

  void update(double x) {
    double e = x - mean;
    mean = mean + eta * e;
  }

  unsigned char eval(double x) {
    if (abs(x - mean) > threshold)
      return 200;
    return 10;
  }
};

struct corr_filter {
  double mean, sigma, eta, threshold;

  corr_filter() : mean(100), sigma(100), eta(0.1), threshold(0.003) {}
  corr_filter(double mean, double sigma, double eta, double threshold) :
    mean(mean),
    sigma(sigma),
    eta(eta),
    threshold(threshold)
  {}

  double kernel(double x, double y) {
    double dist = x - y;
    double den = 1.0 / (2 * sigma * sigma);
    return exp(- (dist * dist * den));
  }

  void update(double x) {
    double e = x - mean;
    // TODO: Fix learning rate, eta / (sigma * sigma)
    mean = mean + eta * e * kernel(x, mean);
  }

  unsigned char eval(double x) {
    if (gaussian_dist(mean, sigma, x) < threshold)
      return 200;
    return 10;
  }
};

#endif