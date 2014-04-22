#include <SDL.h>
#include <iostream>
#include <vector>
#include "GraphicsSystem.h"
#include "KeyBoardInputComponent.h"
#include "EventSystem.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include <map>
#include <functional>
#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 100


const int FPS = 60;
const float FRAMEDURATION = (1.0 / 60) * 1000; // frame duration in ms


void test_function()
{
    std::cout<<"The right button was pressed"<<std::endl;;
}

void initialize_components(ComponentManager *mgr)
{
    mgr->register_component(KeyboardInput::id(),
            []() {
                return new KeyboardInput();
            });

    mgr->register_component(Graphics::id(),
	    []() {
                return new Graphics();
	    });

    mgr->register_component(Position::id(),
	    []() {
                return new Position();
	    });
}

int main()
{
    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER |
                SDL_INIT_AUDIO) < 0) {
        std::cout << "Could not initialize SDL. SDL error: " <<
            SDL_GetError() << std::endl;
        return -1;
    }

    ComponentManager component_mgr;
    initialize_components(&component_mgr);
    EntityManager e(&component_mgr);

    std::map<SDL_Keycode, std::function<void()>> keys;
    std::function<void()> test = test_function;
    keys[SDLK_w] = test; 


    // TODO: this bool pointer that gets passed around to two classes sucks
    bool running = true;
    SystemManager manager(60, &running);

    GraphicsSystem *graphics = new GraphicsSystem();
    EventSystem *events = new EventSystem(&running);

    component_mgr.register_system(graphics, Graphics::id());
    component_mgr.register_system(graphics, Position::id());
    component_mgr.register_system(events, KeyboardInput::id());

    std::vector<type_index> player_components = {KeyboardInput::id(), Graphics::id()};
    unsigned int player = e.add(player_components);

    static_cast<KeyboardInput*>(e.get(player, KeyboardInput::id()))->keybinds = keys;

    /* graphics stuff */
    SDL_Texture* t = graphics->loadTexture("../images/ball.bmp");
    static_cast<Graphics*>(e.get(player, Graphics::id()))->texture = t;

    static_cast<Position*>(e.get(player, Position::id()))->x = 5;
    static_cast<Position*>(e.get(player, Position::id()))->y = 5;
    static_cast<Position*>(e.get(player, Position::id()))->rotation = 0;

    manager.add(graphics);
    manager.add(events);

    manager.loop();

    SDL_Quit();

    return 0;
}
