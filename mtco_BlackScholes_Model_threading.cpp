#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <locale>
#include <thread>
#include <mutex>


using namespace std;

mutex mtx;




double randomNum(int seed){

      random_device rd;
      mt19937_64 mtEngine(rd() + seed);
      normal_distribution<> nd(0.0, 1.0);

      return nd(mtEngine);
}

double blackScholeModel(double s, double k, double t, double r, double sig, int sims, bool isCall){
      double price = 0;
      for(int i = 0; i < sims; i++){
            double val1 = (r - ((sig*sig)/2))*t;
            double val2 = randomNum(i) * sig * sqrt(t);

            double st = s * exp(val1 + val2);
            double payoff = isCall ? max(st - k, 0.0) : max(k - st, 0.0);
            price += payoff;

      }
      return (price/sims) * exp(-r * t);
}

void partialBlackSchole(double s, double k, double t, double r, double sig, int start, int end, bool isCall, double& price){
      double localPrice = 0.0;
      for(int i = start; i < end; i++){
            double val1 = (r - ((sig*sig)/2))*t;
            double val2 = randomNum(i) * sig * sqrt(t);

            double st = s * exp(val1 + val2);
            double payoff = isCall ? max(st - k, 0.0) : max(k - st, 0.0);
            localPrice += payoff;
      }

      lock_guard<mutex> lock(mtx);
      price += localPrice;
}

double blackScholeModelWithThreading(double s, double k, double t, double r, double sig, int sims, bool isCall){
      double price = 0;
      int num_threads = thread::hardware_concurrency(); //number of cpu cores
      int sims_per_thread = sims / num_threads;

      vector<thread> threads; 

      for(int i = 0; i < num_threads; i++){
            int start = i * sims_per_thread;
            int end = (i == num_threads - 1) ? sims : start + sims_per_thread;

            threads.emplace_back(partialBlackSchole, s, k, t, r, sig, start, end, isCall, std::ref(price));
      }

      for(auto& th : threads){
            th.join();
      }
      return (price/sims) * exp(-r * t);
}


int main(){

      double S = 100;
      double k = 100;
      double T = 1;
      double r = 0.05;
      double sig = 0.2;
      int sims = 10000000;

      // cout.imbue(locale("en_US.UTF-8"));




      auto start = chrono::high_resolution_clock::now();
      double call_T = blackScholeModelWithThreading(S, k, T, r, sig, sims, true);
      auto end = chrono::high_resolution_clock::now();
      auto calltime_T = chrono::duration_cast<chrono::milliseconds> (end - start);

      start = chrono::high_resolution_clock::now();
      double put_T = blackScholeModelWithThreading(S, k, T, r, sig, sims, false);
      end = chrono::high_resolution_clock::now();
      auto puttime_T = chrono::duration_cast<chrono::milliseconds>(end-start);

      start =chrono::high_resolution_clock::now();
      double call = blackScholeModel(S, k, T, r, sig, sims, true);
      end = chrono::high_resolution_clock::now();
      auto calltime = chrono::duration_cast<chrono::milliseconds>(end-start);


      start =chrono::high_resolution_clock::now();
      double put = blackScholeModel(S, k, T, r, sig, sims, false);
      end = chrono::high_resolution_clock::now();
      auto puttime = chrono::duration_cast<chrono::milliseconds>(end-start);




      cout << "Call Price (" << sims <<" simulations) with Threading: $" << call_T << " [took " << calltime_T.count()/1000.0 << "s (" << calltime_T.count() << " ms)]" << endl;
      cout << "Put Price (" << sims <<" simulations) with Threading: $" << put_T << " [took " << puttime_T.count()/1000.0 << " s (" << puttime_T.count() << " ms)]" << endl;
      cout<<endl;

      cout << "Call Price (" << sims <<" simulations) without Threading: $" << call << " [took " << calltime.count()/1000.0 << "s (" << calltime.count() << " ms)]" << endl;
      cout << "Put Price (" << sims <<" simulations) without Threading: $" << put << " [took " << puttime.count()/1000.0 << " s (" << puttime.count() << " ms)]" << endl;
      cout<<endl;

      double final = ((double)sims)/calltime_T.count();
      double initial = ((double)sims)/calltime.count();
      cout << "The time improved by " << 100*(final - initial)/initial<< "%"<<endl;

      cout<<endl;
      cout<<"-------end-------"<<endl;
      return 0;
}