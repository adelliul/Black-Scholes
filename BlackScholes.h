#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H


#include <cmath>
#include <stdexcept>


namespace bs {


enum class OptionType { Call, Put };


class BlackScholes {
public:
    // Cumulative standard normal distribution
    static double norm_cdf(double x) noexcept;
    static double norm_pdf(double x) noexcept;


    // Price: option type, spot, strike, risk-free rate, volatility, time to maturity
    static double price(OptionType type, double S, double K, double r, double sigma, double T) ;


    // Greeks
    static double delta(OptionType type, double S, double K, double r, double sigma, double T);
    static double gamma(double S, double K, double r, double sigma, double T);
    static double vega(double S, double K, double r, double sigma, double T);
    static double theta(OptionType type, double S, double K, double r, double sigma, double T);
    static double rho(OptionType type, double S, double K, double r, double sigma, double T);
};


} // namespace bs


#endif // BLACK_SCHOLES_H