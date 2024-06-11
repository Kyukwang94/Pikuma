#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    Particle* smallBall = new Particle(50, 100, 1.0);
    smallBall->radius = 4;
    particles.push_back(smallBall);

    Particle* bigBall = new Particle(50, 100, 3.0);
    bigBall->radius = 12;
    particles.push_back(bigBall);
    // TODO: setup objects in the scene

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event; 
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Particle* particle = new Particle(x, y, 1.0);
                    particle->radius = 5;
                    particles.push_back(particle);
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {

    static int timePreviousFrame;
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
    {
        deltaTime = 0.016;
    }
    timePreviousFrame = SDL_GetTicks();

    for (auto particle : particles)
    {
        Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
        particle->AddForces(weight);
        if (particle->position.y >= liquid.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.03);
            particle->AddForces(drag);
            Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0f * PIXELS_PER_METER);
            particle -> AddForces(friction);
        }
    }
    for (auto particle : particles) {
        particle->AddForces(pushForce);
    }

    for (auto particle : particles)
    {
        particle->Integrate(deltaTime);
    }
    for (auto particle : particles)
    {
        if (particle->position.y + particle->radius > Graphics::windowHeight)
        {
            particle->position.y = Graphics::windowHeight - particle->radius;
            particle->velocity.y *= -1;
        }
        else if (particle->position.y - particle->radius <= 0)
        {
            particle->position.y = particle->radius;
            particle->velocity.y *= -1;
        }
        if (particle->position.x + particle->radius > Graphics::windowWidth)
        {
            particle->position.x = Graphics::windowWidth - particle->radius;
            particle->velocity.x *= -1;
        }
        else if (particle->position.x - particle->radius <= 0)
        {
            particle->position.x = particle->radius;
            particle->velocity.x *= -1;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h,0xFF6E3713);
    for (auto particle : particles)
    {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    // TODO: destroy all objects in the scene
    for(auto particle : particles)
    {
        delete particle;
    }
    Graphics::CloseWindow();
}