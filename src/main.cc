#include <string>
#include <random>
#include <chrono>
#include "BoardRunner.h"

int main(){
	auto seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rng{ seed };

	std::string setupfilename = "resources/config_files/editorsetup.txt";
	BoardRunner boardrunner{ setupfilename };
	boardrunner.run(rng);
}