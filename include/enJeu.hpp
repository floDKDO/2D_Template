#ifndef __EN__JEU__HPP__
#define __EN__JEU__HPP__

#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"
#include "joueur.hpp"
#include "tuile.hpp"
#include <vector>

class EnJeu
{
    public:
        EnJeu(SingletonSysteme* sing_syst);
        Joueur joueur;
        std::vector<Tuile> tuiles;

        void initTuiles(void);
        void draw(SingletonSysteme* sing_syst);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __EN__JEU__HPP__
