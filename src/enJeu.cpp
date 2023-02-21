#include "enJeu.hpp"

EnJeu::EnJeu(SingletonSysteme* sing_syst)
:joueur(100, BLEU, {160, 160, 32, 32}, VUE_DESSUS)
{
    (void)sing_syst;
    tuiles.push_back(Tuile("./img/animated_water.png", {200, 200, 200, 200}, 8, 17));
    tuiles.push_back(Tuile("./img/animated_water.png", {400, 200, 200, 200}, 8, 17));
    tuiles.push_back(Tuile("./img/animated_water.png", {2000, 200, 200, 200}, 8, 17));
    tuiles.push_back(Tuile("./img/animated_water.png", {400, 2000, 200, 200}, 8, 17));
}



void EnJeu::draw(SingletonSysteme* sing_syst)
{
    this->joueur.draw(sing_syst->rendu);
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        this->tuiles[i].draw(sing_syst->rendu);
    }
}


void EnJeu::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    this->joueur.update(timeStep, sing_syst, tuiles);
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        this->tuiles[i].update(timeStep, sing_syst);
    }
}


void EnJeu::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    this->joueur.handleEvents(e, sing_syst);
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        //this->tuiles[i].handleEvents(e, sing_syst);
    }
}
