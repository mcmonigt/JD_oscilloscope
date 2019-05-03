#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

#define PI 3.141592
using namespace std;

double doubleRand() {
  return double(rand()) / (double(RAND_MAX) + 1.0);
}

int main(){
  ofstream voltage_scale;
  // voltage_scale.open("voltage_scale.txt", ios::out | ios::trunc);
  // if (voltage_scale.fail()){
  //   cout << "could not open file voltage_scale" << endl;
  //   return 1;
  // }
  // ofstream << "5" << 
  srand(static_cast<unsigned int>(clock()));
  ofstream data_signal1;
  while (1){
    data_signal1.open("data_signal1.txt", ios::out | ios::trunc);
    if (data_signal1.fail()){
      cout << "could not open file data_signal1" << endl;
      return 1;
    }
    for (int j = 0; j < 1024; j++){
      double y = sin(PI / 50 * j) + 0.5 + doubleRand();
      double x = j;
      if (x != 0 && y !=0){
        data_signal1 << x << " " << y << '\n';
      }
    }
    data_signal1.close();


    sleep(1);
  }
  return 0;
}
