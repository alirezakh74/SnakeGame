#include <print>
#include <vector>
#include <cmath>

#include <SDL.h>

class Vec2
{
public:
    int x, y;
    Vec2(): x(0), y(0) {}
    Vec2(int x, int y) { this->x = x; this->y = y; }
};

class Part
{
public:
    Vec2 position;
    Vec2 movement_direction;
    Part() : position(0, 0), movement_direction(0,0) {}
    Part(Vec2 pos, Vec2 movement_dir) : position(pos), movement_direction(movement_dir) {}
};

class Snake
{
public:
    std::vector<Part> m_parts;

    Snake()
    {
        Part head;
        head.position = Vec2(10, 10);
        m_parts.push_back(head);
    }

    void move()
    {
        //move bodies: all part of bodies get position of prevous part
        for (auto i = m_parts.size() - 1; i > 0; i--)
        {
            m_parts.at(i).position = m_parts.at(i - 1).position;
            m_parts.at(i).movement_direction = m_parts.at(i - 1).movement_direction;
        }

        //move head: change position based on current direction
        m_parts.at(0).position.x = m_parts.at(0).position.x + m_parts.at(0).movement_direction.x;
        m_parts.at(0).position.y = m_parts.at(0).position.y + m_parts.at(0).movement_direction.y;
    }

    void addBody()
    {
        Part body;
        body.position.x = (m_parts.at(m_parts.size() - 1).position.x) - (m_parts.at(m_parts.size() - 1).movement_direction.x);
        body.position.y = (m_parts.at(m_parts.size() - 1).position.y) - (m_parts.at(m_parts.size() - 1).movement_direction.y);
        m_parts.push_back(body);
    }
};

class Apple
{
public:
    Vec2 position;
    Apple() : position(8, 0) {}
    Apple(Vec2 pos) : position(pos) {}
};

bool collideWithApple(Vec2 headPos, Vec2 applePos)
{
    if (headPos.x == applePos.x && headPos.y == applePos.y)
    {
        return true;
    }
    return false;
}

bool collideWithBody(Vec2 headPos, Vec2 BodyPos)
{
    if (headPos.x == BodyPos.x && headPos.y == BodyPos.y)
    {
        return true;
    }
    return false;
}

void changeApplePosition(Vec2& applePos)
{
    int rand_x = rand() % 20;
    int rand_y = rand() % 15;
    applePos.x = rand_x;
    applePos.y = rand_y;
}

int main(int argc, char* argv[])
{
    const int WINDOW_WIDTH = 740;
    const int WINDOW_HEIGHT = 580;
    const int cellSize = 32;
    const int row_numbers = 15;
    const int column_numbers = 20;

    const int offset_start_size = 64;

    bool running = true;

    Snake snake;
    Apple apple;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::println("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        running = false;
        std::println("create window problem! {}", SDL_GetError());
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        running = false;
        std::println("create renderer problem! {}", SDL_GetError());
    }

    SDL_Rect myrect;
    myrect.x = 0;
    myrect.y = 0;
    myrect.w = cellSize;
    myrect.h = cellSize;

    unsigned int frameTime = SDL_GetTicks();
    int deltaTime = 0;
    unsigned int frameCount = 0;
    unsigned int elapsedTime = 0;
    const int TARGET_FPS = 4;
    const int TIME_PER_FRAME = 1000 / TARGET_FPS;

    SDL_Event e;
    while (running)
    {
        //frameCount++;
        //std::println("SDL_GetTicks() {}", SDL_GetTicks());
        elapsedTime = SDL_GetTicks();

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
                break;
            }

            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (snake.m_parts.at(0).movement_direction.y != 1)
                    {
                        snake.m_parts.at(0).movement_direction.y = -1;
                        snake.m_parts.at(0).movement_direction.x = 0;
                    }
                    break;
                case SDLK_DOWN:
                    if (snake.m_parts.at(0).movement_direction.y != -1)
                    {
                        snake.m_parts.at(0).movement_direction.y = 1;
                        snake.m_parts.at(0).movement_direction.x = 0;
                    }
                    break;
                case SDLK_RIGHT:
                    if (snake.m_parts.at(0).movement_direction.x != -1)
                    {
                        snake.m_parts.at(0).movement_direction.x = 1;
                        snake.m_parts.at(0).movement_direction.y = 0;
                    }
                    break;
                case SDLK_LEFT:
                    if (snake.m_parts.at(0).movement_direction.x != 1)
                    {
                        snake.m_parts.at(0).movement_direction.x = -1;
                        snake.m_parts.at(0).movement_direction.y = 0;
                    }
                    break;
                case SDLK_a:
                    snake.addBody();
                    break;
                default:
                    break;
                }
            }
        }

        // update and render after every 60 FPS or x second
        /*elapsedTime += frameTime;
        if (elapsedTime > 1000)
        {
            float currentFps = static_cast<float>(frameCount) / (elapsedTime / 1000.0f);
            std::println("fps: {}", currentFps);
            frameCount = 0;
            elapsedTime = 0;
        }*/

        //std::println("frameTime: {}", frameTime);
        frameTime = SDL_GetTicks() - frameTime;
        deltaTime += frameTime;
        std::println("frameTime: {}", frameTime);
        std::println("deltaTime: {}", deltaTime);
        if (deltaTime >= TIME_PER_FRAME)
        {
            //std::println("deltaTime: {}", deltaTime);
            deltaTime = 0;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // draw board of game
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            for (size_t i = 0; i < row_numbers; i++)
            {
                for (size_t j = 0; j < column_numbers; j++)
                {
                    myrect.x = j * cellSize + offset_start_size;
                    myrect.y = i * cellSize + offset_start_size;
                    SDL_RenderDrawRect(renderer, &myrect);
                    //SDL_RenderFillRect(renderer, &myrect);
                }
            }

            // draw apple
            myrect.x = apple.position.x * cellSize + offset_start_size;
            myrect.y = apple.position.y * cellSize + offset_start_size;
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &myrect);

            // check collision head and bodies
            for (int i = 1; i < snake.m_parts.size(); i++)
            {
                if (collideWithBody(snake.m_parts.at(0).position, snake.m_parts.at(i).position))
                {
                    running = false;
                    std::println("You Lose!");
                }
            }

            // check collision head and apple
            if (collideWithBody(snake.m_parts.at(0).position, apple.position))
            {
                snake.addBody();
                changeApplePosition(apple.position);
                std::println("You Eat an apple!");
            }

            // check borders movement
            if (snake.m_parts.at(0).position.x < 0)
            {
                snake.m_parts.at(0).position.x = column_numbers - 1;
            }
            else if (snake.m_parts.at(0).position.x >= column_numbers)
            {
                snake.m_parts.at(0).position.x = 0;
                //std::println("x = {}", snake.m_parts.at(0).position.x);
            }
            if (snake.m_parts.at(0).position.y < 0)
            {
                snake.m_parts.at(0).position.y = row_numbers - 1;
            }
            else if (snake.m_parts.at(0).position.y >= row_numbers)
            {
                snake.m_parts.at(0).position.y = 0;
                //std::println("y = {}", snake.m_parts.at(0).position.y);
            }

            // draw snake head and bodies
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for (int i = 0; i < snake.m_parts.size(); i++)
            {
                myrect.x = snake.m_parts.at(i).position.x * cellSize + offset_start_size;
                myrect.y = snake.m_parts.at(i).position.y * cellSize + offset_start_size;
                if (i == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                }
                SDL_RenderFillRect(renderer, &myrect);
            }

            // check win
            if (snake.m_parts.size() == row_numbers * column_numbers)
            {
                running = false;
                std::println("You Win");
            }

            snake.move();

            //std::println("x = {}", snake.m_parts.at(0).position.x);
            //std::println("y = {}", snake.m_parts.at(0).position.y);

            SDL_RenderPresent(renderer);

            frameTime = SDL_GetTicks() - frameTime;
            deltaTime += frameTime;

            if (frameTime < TIME_PER_FRAME)
            {
                frameTime = 0;
                SDL_Delay(TIME_PER_FRAME - frameTime);
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    return 0;
}