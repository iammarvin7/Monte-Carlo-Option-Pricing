#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

using namespace std;

double monte_carlo_call_price(int num_sims, double S, double K, double r, double v, double T) {

    random_device rd;
    mt19937 mtEngine(rd());
    normal_distribution<> nd(0, 1);

    double sum = 0.0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < num_sims; ++i) {
        double gauss = nd(mtEngine);
        double S_T = S * exp((r - 0.5 * v * v) * T + v * sqrt(T) * gauss);
        sum += exp(-r * T) * max(0.0, S_T - K); // payoff
    }

// #pragma omp parallel for reduction(+:sum)
//     for (int i = 0; i < num_sims; ++i) {
//         sum += results[i];
//     }

    return sum / num_sims;
}

int main() {
    int num_sims = 10000000; // 10M sims
    double S = 100.0, K = 100.0, r = 0.05, v = 0.2, T = 1.0;

    double start = omp_get_wtime();
    double call_price = monte_carlo_call_price(num_sims, S, K, r, v, T);
    double end = omp_get_wtime();
    cout << "Call Price: " << call_price << " [Time: " << (end - start) << " s]\n";
    return 0;
}