#include <iostream>
#include <iomanip>
#include <cstring>
#include "BlackScholes.h"

using namespace std;
using namespace bs;

void print_usage() {
    cout << "Usage: black_scholes_cli --type <call|put> --S <spot> --K <strike> --r <rate> --sigma <vol> --T <time>" << endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        print_usage();
        return 0;
    }

    OptionType type = OptionType::Call;
    double S = -1, K = -1, r = 0.0, sigma = -1, T = -1;

    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "--type" && i + 1 < argc) {
            string v = argv[++i];
            if (v == "call") type = OptionType::Call;
            else if (v == "put") type = OptionType::Put;
            else { cerr << "Unknown type: " << v << '\n'; return 1; }
        } else if (a == "--S" && i + 1 < argc) S = stod(argv[++i]);
        else if (a == "--K" && i + 1 < argc) K = stod(argv[++i]);
        else if (a == "--r" && i + 1 < argc) r = stod(argv[++i]);
        else if (a == "--sigma" && i + 1 < argc) sigma = stod(argv[++i]);
        else if (a == "--T" && i + 1 < argc) T = stod(argv[++i]);
        else if (a == "--help") { print_usage(); return 0; }
    }

    try {
        double p = BlackScholes::price(type, S, K, r, sigma, T);

        cout << fixed << setprecision(6);
        cout << "Price: " << p << endl;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 2;
    }

    return 0;
}
