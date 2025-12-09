# Introduction
This project demonstrates how the Black–Scholes model can be used to estimate the fair value of financial options. Instead of relying on intuition or guesswork, the model applies mathematical assumptions about how markets behave to produce a consistent price. The focus is on European call and put options, which can only be exercised at expiration. The program is implemented in C++, organized into separate source and header files, and provides a command-line tool for calculating option prices from user-supplied inputs. This makes it possible to quickly evaluate how factors such as asset price, strike price, interest rate, volatility, and time to maturity influence option value.

# Concept Overview
The Black–Scholes model is based on the idea that the price of an asset moves randomly but follows a predictable statistical pattern over time. By combining current market data with assumptions about future uncertainty, the model estimates how likely it is that an option will finish profitably at expiration. The result is a fair value calculation for both call and put options.

In practical terms, the model weighs two ideas:
- what the market looks like now
- how much movement (risk) is expected before expiration

Options become more valuable when there is greater uncertainty, because there is more opportunity for large future price changes. The C++ implementation captures this logic in a class that computes call and put prices, and a command-line program allows users to input real values and receive a calculated price. To know more please read Black_Scholes.pdf
