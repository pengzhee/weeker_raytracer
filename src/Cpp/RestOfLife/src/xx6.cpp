#include "random.h"

#include "vec3.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

vec3 random_on_unit_sphere() {
  vec3 p;
  do {
    p = 2.0*vec3(random_double(),random_double(),random_double()) - vec3(1,1,1);
  } while (dot(p,p) >= 1.0);
  return unit_vector(p);
}

inline double pdf(const vec3& p) {
  return 1 / (4*M_PI);
}

int main() {
  int N = 1000000;
  double sum;
  for (int i = 0; i < N; i++) {
    vec3 d = random_on_unit_sphere();
    double cosine_squared = d.z()*d.z();
    sum += cosine_squared / pdf(d);
  }
  std::cout << "I = " << sum/N << "\n";
}
