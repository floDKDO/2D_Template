#ifndef __EN__JEU__HPP__
#define __EN__JEU__HPP__

#include "singleton_systeme.hpp"
#include "joueur.hpp"
#include "tuile.hpp"
#include "carte.hpp"
#include "boiteDeDialogue.hpp"

#include <vector>

class EnJeu
{
    public:
        EnJeu(SingletonSysteme* sing_syst);

        Carte* carte_actuelle;
        SDL_Rect carre_noir;
        bool enTransition;
        int alpha;
        Joueur joueur;
        BoiteDeDialogue* b;
        bool dialogue;
        bool une_fois; //pour son collision : collision entre une tuile et le joueur

        void playSoundCollision(Uint32& timeStep, SingletonSysteme* sing_syst);
        bool estACote(Joueur joueur, SDL_Rect position);
        bool estDedans(SDL_Rect dest_joueur, SDL_Rect position);
        bool replace(std::string& str, const std::string& from, const std::string& to);
        void checkCollisionsPlayerMap(Uint32& timeStep, SingletonSysteme* sing_syst);
        void checkPlayerTakesObjects(SingletonSysteme* sing_syst);
        void gereJoueurPrendWarp();
        void gereJoueurSortExtremite();
        void gereSelonTransition(Uint32& timeStep, SingletonSysteme* sing_syst);
        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __EN__JEU__HPP__
