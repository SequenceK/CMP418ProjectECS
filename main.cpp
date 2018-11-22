#include <SDL2/SDL.h>
#include <iostream>
#include <omp.h>

using namespace std;

int SDL_main(int argc, char **argv) {

    SDL_Init(SDL_INIT_EVERYTHING);

    //while (1) {
#pragma omp parallel for
	for(int i=0;i<10;i++)
		cout << omp_get_thread_num() << endl;

    //}
    SDL_Quit();
    return 0;
}