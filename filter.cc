#ifndef CFILTER
#define CFILTER

#include <vector>
#include <cmath>
#include <iostream>
#include <memory>
#include <deque>

#include "debug.cc"

const double pi = acos(-1);

double gaussian_dist(double mean, double sig, double x) {
  const double norm = 1.0 / (sqrt(2 * pi ) * sig);

  double dist = mean - x;
  return norm * exp(- (dist * dist) / (2 * sig * sig));
}

double m_pow(double base, int e) {
  double ans = 1;
  while (e > 0) {
    if (e & 1)
      ans *= base;
    base *= base;
    e >>= 1;
  }
  return ans;
}

inline double estimate_mean(std::deque<double> x) {
  double ans = 0;
  for (auto it : x)
    ans += it;
  return ans / double(x.size());
}

inline double estimate_sigma(std::deque<double> x, double mean) {
  double ans = 1e-9;
  for (auto it : x)
    ans += (it - mean) * (it - mean);
  return sqrt(ans / double(x.size()));
}

inline double estimate_kurtosis(std::deque<double> x, double mean, double sigma) {
  double ans = 0;
  for (auto i : x) {
    ans += m_pow((i - mean) / sigma, 4);
  }
  return ans / double(x.size());
}

struct lms_filter {
  double mean, sigma, eta, threshold;
  lms_filter() : mean(10), sigma(0.2), eta(0.01), threshold(50) {}
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
  double mean, sigma, eta, threshold, lambda;
  size_t T;

  std::deque<double> error, data;

  corr_filter() : corr_filter(100, 1, 0.5, 0.004, 0.5, 20) {}
  corr_filter(double mean, double sigma, double eta, double threshold,
              double lambda, size_t T) :
    mean(mean),
    sigma(sigma),
    eta(eta),
    threshold(threshold),
    lambda(lambda),
    T(T),
    error(),
    data()
  {}

  double kernel(double x, double y) {
    double dist = x - y;
    double den = 1.0 / (2 * sigma * sigma);
    return exp(- (dist * dist * den));
  }

  void update_sigma() {
    if (error.size() != T)
      return;

    double mean_err = estimate_mean(error);
    double sigma_err = estimate_sigma(error, mean_err);

    double kurt_err = estimate_kurtosis(error, mean_err, sigma_err);
    double kurt_model = estimate_kurtosis(data, mean, sigma);

    sigma = lambda * sigma + (1 - lambda) * sigma_err * sqrt(kurt_model / kurt_err);
  }

  void update(double x) {
    double e = x - mean;
    error.push_back(e);
    if (error.size() > T)
      error.pop_front();

    data.push_back(x);
    if (data.size() > T)
      data.pop_front();

    update_sigma();
    mean = mean + (eta / (sigma * sigma)) * e * kernel(x, mean);
  }

  unsigned char eval(double x) {
    if (gaussian_dist(mean, sigma, x) < threshold)
      return 200;
    return 10;
  }
};

#endif