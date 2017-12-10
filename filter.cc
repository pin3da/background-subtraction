#ifndef CFILTER
#define CFILTER

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

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
  double ans = 0;
  double den = 1.0 / double(x.size() - 1);
  for (auto it : x)
    ans += (it - mean) * (it - mean) * den;
  return sqrt(ans);
}

inline double estimate_kurtosis(std::deque<double> x, double mean, double sigma) {
  double ans = 0;
  double den = 1.0 / double(x.size());
  for (auto i : x) {
    ans += m_pow((i - mean) / sigma, 4) * den;
  }
  return ans;
}

struct lms_filter {
  double mean, sigma, eta;
  lms_filter() : mean(10), eta(0.05) {}
  lms_filter(double mean, double eta) :
    mean(mean),
    eta(eta)
  {}

  void update(double x) {
    double e = x - mean;
    mean = mean + eta * e;
  }

  double eval(double x) {
    return 1.0 - (abs(x - mean) / 255.0);
  }
};

struct corr_filter {
  double mean, sigma, eta, lambda;
  size_t T;

  std::deque<double> error, data;

  corr_filter() :
    mean(100),
    sigma(40),
    eta(0.05 * 40 * 40),  // eta * sigma ** 2
    lambda(0.99),
    T(200),
    error(),
    data()
  {}

  double kernel(double x, double y) {
    double dist = x - y;
    double den = 1.0 / (2 * sigma * sigma);
    return exp(- (dist * dist * den));
  }

  void update_sigma() {
    if (error.size() < T)
      return;

    double mean_e = estimate_mean(error);
    double sigma_err = estimate_sigma(error, mean_e);

    double kurt_err = estimate_kurtosis(error, mean_e, sigma_err);
    double kurt_model = estimate_kurtosis(data, mean, sigma);

    sigma = lambda * sigma + (1.0 - lambda) * sigma_err * sqrt(kurt_model / kurt_err);
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

  double eval(double x) {
    return gaussian_dist(mean, sigma, x);
  }
};

#endif