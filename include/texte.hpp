#ifndef __TEXTE__HPP__
#define __TEXTE__HPP__

#include "../include/singleton_systeme.hpp"

class Texte
{
    public:
        Texte(std::string texte, std::string police, int taille_police, SDL_Color couleur, SDL_Rect position, SDL_Renderer* rendu, std::string name, bool isDialogue);

        SDL_Rect position;
        std::string texte;
        //si le texte est un dialogue avec un défilement
        std::string texteDefilement;
        bool isDialogue;
        unsigned int indice;
        Uint32 wrapLength;
        ////////////////////////////////////////////////
        std::string name;
        TTF_Font* police;
        SDL_Color couleur;
        SDL_Surface* surface;
        SDL_Texture* texture;
        int taille_police;

        void draw(SDL_Renderer* rendu);
        void update(Uint32& timeStep);
        void editText(std::string nouveau_texte, SDL_Renderer* rendu);

    protected:

    private:
};

#endif // __TEXTE__HPP__
