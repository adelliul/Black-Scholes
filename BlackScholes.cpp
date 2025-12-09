#include "BlackScholes.h"
#include <cmath>
#include <limits>


namespace bs {


static constexpr double INV_SQRT_2 = 0.70710678118654752440; // 1 / sqrt(2)


double BlackScholes::norm_cdf(double x) noexcept {
    // Using std::erf: N(x) = 0.5 * (1 + erf(x / sqrt(2)))
    return 0.5 * (1.0 + std::erf(x * INV_SQRT_2));
}


double BlackScholes::norm_pdf(double x) noexcept {
    static const double INV_SQRT_2PI = 0.3989422804014327; // 1/sqrt(2*pi)
    return INV_SQRT_2PI * std::exp(-0.5 * x * x);
}


static void check_inputs(double S, double K, double sigma, double T) {
    if (!(S > 0.0)) throw std::invalid_argument("Spot price S must be > 0");
    if (!(K > 0.0)) throw std::invalid_argument("Strike price K must be > 0");
    if (!(sigma >= 0.0)) throw std::invalid_argument("Volatility sigma must be >= 0");
    if (!(T >= 0.0)) throw std::invalid_argument("Time to maturity T must be >= 0");
}

static void d1d2(double S, double K, double r, double sigma, double T, double &d1, double &d2) {
    if (T <= 0.0 || sigma == 0.0) {
    // Handle immediate expiry / zero vol safely
    d1 = d2 = std::numeric_limits<double>::infinity();
    return;
    }
    double sqrtT = std::sqrt(T);
    d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrtT);
    d2 = d1 - sigma * sqrtT;
}


double BlackScholes::price(OptionType type, double S, double K, double r, double sigma, double T) {
    check_inputs(S, K, sigma, T);
    if (T == 0.0) {
        // At expiry: payoff
        if (type == OptionType::Call) return std::max(0.0, S - K);
        else return std::max(0.0, K - S);
}


double d1, d2;
d1d2(S, K, r, sigma, T, d1, d2);


if (!std::isfinite(d1) || !std::isfinite(d2)) {
    // fallback for extremely small T or sigma
    if (type == OptionType::Call) return std::max(0.0, S - K * std::exp(-r * T));
    else return std::max(0.0, K * std::exp(-r * T) - S);
}


double Nd1 = norm_cdf((type == OptionType::Call) ? d1 : -d1);
double Nd2 = norm_cdf((type == OptionType::Call) ? d2 : -d2);


if (type == OptionType::Call) {
    return S * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
} else {
    return K * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
}
}

double BlackScholes::delta(OptionType type, double S, double K, double r, double sigma, double T) {
    check_inputs(S, K, sigma, T);
    if (T == 0.0) {
        if (type == OptionType::Call) return S > K ? 1.0 : 0.0;
        else return S < K ? -1.0 : 0.0;
    }
    double d1, d2;
    d1d2(S, K, r, sigma, T, d1, d2);
    if (type == OptionType::Call) return norm_cdf(d1);
    else return norm_cdf(d1) - 1.0;
}


double BlackScholes::gamma(double S, double K, double r, double sigma, double T) {
    check_inputs(S, K, sigma, T);
    if (T == 0.0) return 0.0;
    double d1, d2;
    d1d2(S, K, r, sigma, T, d1, d2);
    double sqrtT = std::sqrt(T);
    return norm_pdf(d1) / (S * sigma * sqrtT);
}

double BlackScholes::vega(double S, double K, double r, double sigma, double T) {
    check_inputs(S, K, sigma, T);
    if (T == 0.0) return 0.0;
    double d1, d2;
    d1d2(S, K, r, sigma, T, d1, d2);
    return S * norm_pdf(d1) * std::sqrt(T);
}


double BlackScholes::theta(OptionType type, double S, double K, double r, double sigma, double T) {
    check_inputs(S, K, sigma, T);
    if (T == 0.0) return 0.0;
    double d1, d2;
    d1d2(S, K, r, sigma, T, d1, d2);
    double pdf_d1 = norm_pdf(d1);
    double sqrtT = std::sqrt(T);
    double term1 = - (S * pdf_d1 * sigma) / (2.0 * sqrtT);
    double term2_call = r * K * std::exp(-r * T) * norm_cdf(d2);
    double term2_put = r * K * std::exp(-r * T) * norm_cdf(-d2);
    if (type == OptionType::Call) return term1 - term2_call;
    else return term1 + term2_put;
}

double BlackScholes::rho(OptionType type, double S, double K, double r, double sigma, double T) {
    check_inputs(S, K, sigma, T);
    if (T == 0.0) return 0.0;
    double d1, d2;
    d1d2(S, K, r, sigma, T, d1, d2);
    if (type == OptionType::Call) return K * T * std::exp(-r * T) * norm_cdf(d2);
    else return -K * T * std::exp(-r * T) * norm_cdf(-d2);
}


} // namespace bs