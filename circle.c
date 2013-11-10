#include "circle.h"


TTF_Font* font = NULL;
SDL_Color fontColor = {255, 255, 255};

void initCircle(Circle* circle, cpSpace* space, SDL_Surface* surface)
{
    circle->c[0] = rand() % 26 + 97;
    circle->c[1] = '\0';

    cpFloat radius = 22;
    cpFloat mass = 2;
    circle->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, radius * 2 + 1,
                                           radius * 2, 32, 0xFF000000,
                                           0x00FF0000, 0x0000FF00, 0x000000FF);
    circle->color = SDL_MapRGB(circle->surface->format, 0, 0, 255);

    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    circle->body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(circle->body, cpv(rand() % 360 + 120, 50));
    circle->shape = cpSpaceAddShape(space, cpCircleShapeNew(circle->body,
                                    radius, cpvzero));
    cpShapeSetFriction(circle->shape, 0.8);
    cpShapeSetElasticity(circle->shape, 0.9);
    circle->shape->collision_type = 0;
    circle->affected = 0;
    circlesNumber++;
}

void freeCircle(Circle* circle)
{
    cpShapeFree(circle->shape);
    cpBodyFree(circle->body);
    circlesNumber--;
}

void renderCircle(SDL_Surface* surface, Circle* circle)
{
    cpFloat radius = cpCircleShapeGetRadius(circle->shape);
    SDL_Rect rect;
    cpVect pos = cpBodyGetPos(circle->body);
    if(circle->affected)
    {
        filledCircleColor(circle->surface, radius, radius, radius,
                          SDL_MapRGB(circle->surface->format, 0, 200, 255));
    }
    else
    {
        filledCircleColor(circle->surface, radius, radius, radius, circle->color);
    }
    SDL_Surface* c = TTF_RenderText_Blended(font, circle->c, fontColor);
    cpFloat angle = -cpBodyGetAngle(circle->body);
    SDL_Surface* rotatedSurface = rotozoomSurface(c,
                                  angle * TO_DEGREES, 1, SMOOTHING_ON);
    rect.x = (2 * radius - rotatedSurface->w) / 2;
    rect.y = (2 * radius - rotatedSurface->h) / 2;
    SDL_BlitSurface(rotatedSurface, NULL, circle->surface, &rect);
    rect.x = pos.x - radius;
    rect.y = pos.y - radius;
    SDL_BlitSurface(circle->surface, NULL, surface, &rect);
    SDL_FreeSurface(c);
    SDL_FreeSurface(rotatedSurface);
}

void circleInit()
{
    font = TTF_OpenFont("UbuntuMono-R.ttf", 32);
}

void circleQuit()
{
    TTF_CloseFont(font);
}
