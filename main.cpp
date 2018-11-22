#include <SDL2/SDL.h>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char **argv) {

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window;
    window = SDL_CreateWindow("Test",
			      0,0,
			      100,100,
			      SDL_WINDOW_SHOWN
			      );
    //while (1) {
#pragma omp parallel for
	for(int i=0;i<10;i++)
		cout << omp_get_thread_num() << endl;

    //}
	SDL_Delay(1000);
    SDL_Quit();
    return 0;
}
