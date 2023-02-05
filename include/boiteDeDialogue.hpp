#ifndef __BOITE__DE__DIALOGUE__HPP__
#define __BOITE__DE__DIALOGUE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "texte.hpp"

#include "singleton_systeme.hpp"

//texte + carré
//event : quand le texte est entièrement affiché, clic ou touche Entrée quitte la boite de dialogue
class BoiteDeDialogue
{
    public:
        BoiteDeDialogue(SDL_Color couleur, SDL_Rect position, std::string texte, int taille_police, SDL_Renderer* rendu, std::string name);

        Texte texte_defilement;
        SDL_Rect position;
        SDL_Texture* texture;
        std::string name;
        SDL_Color couleur;
        Mix_Chunk* click_sound;

        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        void Update(Uint32& timeStep);
        bool collision(SDL_Rect dest_joueur, int x, int y);

    protected:

    private:
};

#endif // __BOITE__DE__DIALOGUE__HPP__
