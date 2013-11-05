#include "circle.h"

void initCircle(Circle* circle, cpSpace* space, SDL_Surface* surface)
{
    circle->c = rand() % 27 + 97;

    cpFloat radius = 22;
    cpFloat mass = 2;
    circle->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, radius * 2, radius * 2
                                           , 32, 0xFF000000, 0x00FF0000,
                                           0x0000FF00, 0x000000FF);
    circle->color = SDL_MapRGB(circle->surface->format, 0, 0, 255);

    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    circle->body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(circle->body, cpv(rand() % 360 + 120, 50));
    circle->shape = cpSpaceAddShape(space, cpCircleShapeNew(circle->body,
                                    radius, cpvzero));
    cpShapeSetFriction(circle->shape, 0.8);
    cpShapeSetElasticity(circle->shape, 0.9);
}

void freeCircle(Circle* circle)
{
    cpShapeFree(circle->shape);
    cpBodyFree(circle->body);
}

void renderCircle(SDL_Surface* surface, Circle* circle)
{
    cpFloat radius = cpCircleShapeGetRadius(circle->shape);
    SDL_Rect rect;
    cpVect pos = cpBodyGetPos(circle->body);
    filledCircleColor(circle->surface, radius, radius, radius, circle->color);
    characterColor(circle->surface, 18, 18, circle->c,
                   SDL_MapRGB(circle->surface->format, 250, 0, 0));
    cpFloat angle = -cpBodyGetAngle(circle->body);
    SDL_Surface* rotatedSurface = rotozoomSurface(circle->surface,
                                  angle * TO_DEGREES, 1, SMOOTHING_ON);
    int xOffset = (rotatedSurface->w - circle->surface->w) / 2;
    int yOffset = (rotatedSurface->h - circle->surface->h) / 2;
    rect.x = pos.x - radius - xOffset;
    rect.y = pos.y - radius - yOffset;
    printf("%f\n", angle);
    SDL_BlitSurface(rotatedSurface, NULL, surface, &rect);
    SDL_FreeSurface(rotatedSurface);
}
