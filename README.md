# Monte-Carlo-Option-Pricing

A C++ implementation of the Black-Scholes model for option pricing using Monte Carlo simulations, featuring single-threaded and multi-threaded variants.

---

## Overview

- **Purpose:** Compute Call and Put option prices with Monte Carlo methods under Black-Scholes.
- **Highlight:** Multi-threading delivers up to 6.9x speedup (85% time reduction) at 10M simulations.
- **Language:** C++ (100% pure performance).

---

## Performance Highlights

| Simulations | Single-Threaded (Call/Put) | Multi-Threaded (Call/Put) | Speedup |
|-------------|----------------------------|---------------------------|---------|
| 100         | 1 ms / 1 ms               | 2 ms / 1 ms              | 0.5x / 1x  |
| 10,000      | 54 ms / 63 ms             | 9 ms / 9 ms              | 6x / 7x    |
| 1,000,000   | 4.55 s / 4.03 s           | 0.68 s / 0.71 s          | 6.7x / 5.7x |
| 10,000,000  | 44.25 s / 42.45 s         | 6.45 s / 6.28 s          | 6.9x / 6.8x |

> **Convergence:** Call price ~10.44, Put price ~5.57 (±0.09%) at 1M+ simulations.

---

## Installation

1. Clone the repo:
   ```bash
   git clone git@github.com:iammarvin7/Monte-Carlo-Option-Pricing.git
