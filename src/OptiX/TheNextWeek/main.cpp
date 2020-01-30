// std
#include <iostream>
#include <chrono>

// optix
#include <optix.h>
#include <optixu/optixpp.h>

// Executive Director
#include "Director.h"

int main(int argc, char* argv[])
{
  Director optixSingleton = Director();

  auto start = std::chrono::system_clock::now();
  optixSingleton.init(1200, 600);
  optixSingleton.createScene();
  optixSingleton.renderFrame();
  auto stop = std::chrono::system_clock::now();
  auto time_seconds = std::chrono::duration<float>(stop - start).count();
  std::cerr << "Took " << time_seconds << " seconds.\n";

  optixSingleton.printPPM();

  optixSingleton.destroy();
}