#ifndef CFILTER
#define CFILTER

#include <vector>
#include <cmath>

struct filter {
  double mean, sigma, eta, threshold;
  filter() : mean(10), sigma(0.2), eta(0.1), threshold(40) {}
  filter(double _mean, double _sigma, double _eta) :
    mean(_mean),
    sigma(_sigma),
    eta(_eta)
  {}

  double kernel(double x, double y) {
    double dist = x - y;
    double den = 1.0 / (sigma * sigma);
    return exp(- (dist * dist * den));
  }
  void update(double x) {
    double e = x - mean;
    // mean = mean + (eta / (sigma * sigma)) * e * kernel(x, mean);
    mean = mean + eta * e;
  }

  unsigned char eval(double x) {
    if (abs(x - mean) > threshold)
      return 200;
    return 10;
  }
};

typedef std::vector<std::vector<std::vector<filter>>> filter_mat;

#endif