
#include <iostream>
#include <random>
#include <chrono>
#include "BoardRunner.h"
using namespace std;

int main(){
	auto seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rng{ seed };

	string setupfilename = "resources/config_files/v2editorsetup.txt";
	BoardRunner boardrunner{ setupfilename };
	boardrunner.run(rng);
}