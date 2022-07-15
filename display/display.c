#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Event e;

uint16_t SCREEN_WIDTH  = 512;
uint16_t SCREEN_HEIGHT = 512;

bool init_video()
{

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    //Create window
    gWindow = SDL_CreateWindow( "Delay measure", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    SDL_GL_SetSwapInterval(1); // enable vsync with 1
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );

    return true;
}

void video_close()
{
    SDL_DestroyRenderer( gRenderer);
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool handle_input() {
    bool key_pressed = false;
    bool had_event = false;
    while( SDL_PollEvent( &e ) != 0 )
    {
        had_event = true;
        //User requests quit
        if( e.type == SDL_QUIT ) {
            return true;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                return true;
            }
            if (e.key.keysym.sym == SDLK_SPACE) {
                key_pressed = true;
            }
        }
    }

    if (had_event) {
        if (key_pressed) {
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        } else {
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
        }
    }
    return false;
}

void render() {
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(0);
}

int main(int argc, char* argv[]) {
    if(!init_video()) {
        return 1;
    }
    while(!handle_input()) {
        render();
    }
    video_close();
}
