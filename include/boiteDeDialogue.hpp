#ifndef __BOITE__DE__DIALOGUE__HPP__
#define __BOITE__DE__DIALOGUE__HPP__

#include "../include/texte.hpp"
#include "../include/singleton_systeme.hpp"

#include <vector>

//texte + carré
//event : quand le texte est entièrement affiché, clic ou touche Entrée quitte la boite de dialogue
class BoiteDeDialogue
{
    public:
        BoiteDeDialogue(SDL_Color couleur, SDL_Rect position, std::string textes, int taille_police, SDL_Renderer* rendu, std::string name);

        std::vector<Texte> textes_defilement;
        SDL_Rect position;
        SDL_Texture* texture;
        std::string name;
        SDL_Color couleur;
        Mix_Chunk* click_sound;
        SDL_Rect petit_carre;
        bool un_dialogue_fini;
        bool tous_dialogues_finis;
        Uint8 alpha_carre;
        long long unsigned int indice_texte_courant;

        std::vector<std::string> split(std::string s, std::string delimiter);
        void interactionBoite(SingletonSysteme* sing_syst);
        void draw(SDL_Renderer* rendu);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        void update(Uint32& timeStep);
        bool collision(SDL_Rect dest, int x, int y);

    protected:

    private:
};

#endif // __BOITE__DE__DIALOGUE__HPP__
