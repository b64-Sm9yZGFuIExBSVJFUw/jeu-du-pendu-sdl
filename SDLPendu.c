#include <stdio.h>
#include <SDL/SDL.h>
#include "SDLPendu.h"
#include <stdbool.h>


int main(int argc, char *argv[])
{
    init_graphics(1300,700); //On crée le fenêtre de jeu qui aura comme résolution 1300x700
    Menu(); //Fonction qui affiche le menu et les boutons, choix etc.

    if(choix == 1) //Si le joueur a choisi de jouer seul
    {
        Decor(); //Fonction qui affiche le décor

        while(Erreur < 11) //Tant que on a pas perdu
        {
            PiocherMot(); //Fonction qui va choisir un mot au hasard
            AfficherMot(); //Fonction qui va afficher le mot en "_ _ _"
            Jeu(); //Fonction qui va s'occuper de tout le jeu (Mot, deviner, clavier etc...)
        }

        Perdu(); //Fonction qui s'occupe de lorsque le joueur a perdu
    }

    if(choix == 2) //Si le joueur a choisi de jouer à deux, sur le même PC évidemment
    {
        Multi = true;
        DemandeMot(); //Fonction qui fait demander au joueur qui va faire deviner le mot, le mot en question
        Decor(); //Fonction qui affiche le décor mais comme il définit des points pour les graphismes, je le met ici, certes il affiche le décor du mode un joueur met...
        affiche_image("Data/Img/GraphicsMulti",Graphics,WIDTH,HEIGHT); //On met direct le décor pour le mode multi. Maintenant, les points pour effacer le mot en cas de mise à jour sont définis
        TailleMot = strlen(MotATrouver); //On stocke la taille du mot dans une variable

        while(Erreur < 11) //Tant que on a pas perdu
        {
            AfficherMot(); //Fonction qui va afficher le mot en "_ _ _"
            Jeu(); //Fonction qui va s'occuper de tout le jeu (Mot, deviner, clavier etc...)
        }

        Perdu();
    }

}
