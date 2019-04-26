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
  srand(static_cast<unsigned int>(clock()));
  ofstream myfile;
  while (1){
    myfile.open("data_signal1.txt", ios::out | ios::trunc);
    if (myfile.fail()){
      cout << "could not open file data_signal1" << endl;
      return 1;
    }
    for (int j = 0; j < 1024; j++){
      double y = sin(PI / 50 * j) + 0.5 + doubleRand();
      double x = j;
      if (x != 0 && y !=0){
        myfile << x << " " << y << '\n';
      }
    }
    myfile.close();
    sleep(1);
  }
  return 0;
}
