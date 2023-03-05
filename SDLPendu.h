#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <stdbool.h>
#include "graphics.h"

///========================== VARIABLES ========================\\\

FILE* MOT = NULL; //Pointeur qui va pointer sur le fichier "Mots.txt" pour séléctionner le mot que le joueur devra trouver
char MotATrouver[10]; //Chaîne qui contiendra le mot que le joueur devra trouver
char* LettreChoisie = NULL; //Lettre que le joueur a choisi
char* Lettre[2]; //Lettre que le joueur a choisi et qui s'affichera
char* NbVictoires[4]; //Chaîne de caractère qui contiendra le nombre de victoires d'affilées
bool MotTrouve = false; //True si le joueur à gagné.
bool Multiple2 = false; //True si il y a 2 lettres multiples (Z dans PUZZLE)
bool Multiple3 = false; //True si il y a 3 lettres multiples (C dans ACCROCHER)
bool Multi = false; //True si on joue en Multi
int MultiplePosition = 0; //Position de la deuxième lettre multiple
int MultiplePosition3 = 0; //Position de la troisième lettre multiple
int i = 0; //Variable qui servira d'incrémentation
int TailleMot; //Variable qui stockera la taille du mot à trouver
int Erreur = 0; //Nombre d'erreurs
int EndroitLettre = 0; //L'endroit de la lettre trouvee dans le mot
int Victoires = 0; //Nombre de parties gagnées d'affilées
int choix = 0; //Variable qui stockera le choix que le joueur va effectuer dans le menu principal

///========================== POINTS ===========================\\\

    POINT Graphics; //Point qui va être le point en haut à gauche de l'image
    POINT Taille2; //Point q    POINT P;ui va faire afficher le mot à trouver si il est composé de 2 lettres
    POINT Taille3; //Point qui va faire afficher le mot à trouver si il est composé de 3 lettres
    POINT Taille4; //Point qui va faire afficher le mot à trouver si il est composé de 4 lettres
    POINT Taille5; //Point qui va faire afficher le mot à trouver si il est composé de 5 lettres
    POINT Taille6; //Point qui va faire afficher le mot à trouver si il est composé de 6 lettres
    POINT Taille7; //Point qui va faire afficher le mot à trouver si il est composé de 7 lettres
    POINT Taille8; //Point qui va faire afficher le mot à trouver si il est composé de 8 lettres
    POINT Taille9; //Point qui va faire afficher le mot à trouver si il est composé de 9 lettres
    POINT Taille10; //Point qui va faire afficher le mot à trouver si il est composé de 10 lettres
    POINT EffacerMot1; //Point pour effacer et renouveler le mot si lettre trouvée
    POINT EffacerMot2; //Identique qu'au dessus
    POINT EffVictoire1; //Point pour effacer et renouveler le nb de victoires après une victoire
    POINT EffVictoire2; //Pareil qu'en haut
    POINT Clic; //Où le joueur à cliqué
    POINT Victoire; //Point qui va servir pour afficher le nombre de victoires d'affilées sur l'écran
    POINT Dessin; //Point en haut à gauche de la zone où l'image du pendu s'affichera
    POINT EffDessin; //Celui-ci est le point en bas à droite pour effacer l'image quand le joueur aura gagné pour remettre les erreurs à 0

///========================== FONCTIONS ========================\\\

void Menu()
{
    Graphics.x = 0; Graphics.y = 700; //Coordonnées du point cité ci-dessus
    affiche_image("Data/Img/Menu", Graphics, WIDTH, HEIGHT); //On affiche le menu

    while(choix == 0)
    {
        Clic = wait_clic(); //On attends un clic sur l'un des trois boutons
        if(((Clic.x > 368) && (Clic.x < 920)) && ((Clic.y > 364) && (Clic.y < 500))){choix = 1;} //Solo
        if(((Clic.x > 370) && (Clic.x < 918)) && ((Clic.y > 196) && (Clic.y < 340))){choix = 2;} //Multi
        if(((Clic.x > 370) && (Clic.x < 916)) && ((Clic.y > 28) && (Clic.y < 172))){exit(0);} //Quitter
    }

}

void Decor()
{
    affiche_image("Data/Img/Graphics", Graphics, WIDTH, HEIGHT); //On affiche le décor
    EffacerMot1.x = 287; EffacerMot1.y = 655; //Initialisation de la zone du mot à trouver
    EffacerMot2.x = 849; EffacerMot2.y = 400; //Identique à au-dessus
    EffVictoire1.x = 660; EffVictoire1.y = 290; //Initialisation de la zone du nb de victoires
    EffVictoire2.x = 850; EffVictoire2.y = 210; //Pareil qu'en haut
    Victoire.x = 660; Victoire.y = 290; //Initialisation de la zone où le nombre de victoires sera affiché
    sprintf(NbVictoires, "%d", Victoires); //On crée une chaîne de caractère qui aura dedans le nombres de victoires
    aff_pol(NbVictoires,70,Victoire,noir); //On affiche le nombre (Donc la chaîne de caractères)
}

void PiocherMot()
{
    MOT = fopen("Data/Words", "r"); //On ouvre le fichier en lecture seule.
    int QuelMot = alea_int(820); //La ligne du mot choisi sera au harsard (0 à 834) [1 mot par ligne]
    QuelMot++; //La ligne 0 n'existe pas, donc on fait +1 (Si 0) donc ce sera de 1 à 835. (Il y a de plus 835 mots pas 834).

    for(i = 0; i < (QuelMot - 1); i++) //On zappe toutes les lignes jusqu'à qu'on tombe sur la ligne qui fut tirée au sort
    {
        fscanf(MOT,"%s \n", &MotATrouver); //A chaque ligne, on saute une ligne car ce n'est pas la ligne désirée
    }

    fscanf(MOT,"%s", &MotATrouver); //Si on est là, c'est que le pointeur est sur la bonne ligne: On lit donc le mot. Pour le mettre dans la variable "MotATrouver"
    fclose(MOT); //On ferme le fichier Mots.txt car on en aura plus besoin

    TailleMot = strlen(MotATrouver); //On récupère la taille du mot pour écrire les "_ _ _ _"
}

void AfficherMot()
{
    draw_fill_rectangle(EffacerMot1, EffacerMot2,0xC3C3C3); //On retire le mot précédent si le joueur viens de gagner

    if(TailleMot == 2) //Si le mot fait 2 letttres, bah on affichera que deux "_" et ainsi de suite jusqu'à 10...
    {
        Taille2.x = 320; Taille2.y = 725;
        aff_pol("_ _", 250, Taille2, noir);
    }

    if(TailleMot == 3) //...
    {
        Taille3.x = 320; Taille3.y = 640;
        aff_pol("_ _ _", 150, Taille3, noir);
    }
    POINT P;
    if(TailleMot == 4)
    {
        Taille4.x = 300; Taille4.y = 600;
        aff_pol("_ _ _ _", 125, Taille4, noir);
    }

    if(TailleMot == 5)
    {
        Taille5.x = 300; Taille5.y = 570;
        aff_pol("_ _ _ _ _", 100, Taille5, noir);
    }

    if(TailleMot == 6)
    {
        Taille6.x = 320; Taille6.y = 540;
        aff_pol("_ _ _ _ _ _", 75, Taille6, noir);
    }

    if(TailleMot == 7)
    {
        Taille7.x = 310; Taille7.y = 520;
        aff_pol("_ _ _ _ _ _ _", 68, Taille7, noir);
    }

    if(TailleMot == 8)
    {
        Taille8.x = 300; Taille8.y = 540;
        aff_pol("_ _ _ _ _ _ _ _", 60, Taille8, noir);
    }

    if(TailleMot == 9)
    {
        Taille9.x = 300; Taille9.y = 530;
        aff_pol("_ _ _ _ _ _ _ _ _", 52, Taille9, noir);
    }

    if(TailleMot == 10)
    {
        Taille10.x = 300; Taille10.y = 520;
        aff_pol("_ _ _ _ _ _ _ _ _ _", 46, Taille10, noir);
    }
}

void Clavier() //Fonction qui va s'occuper des clics sur le clavier
{
    if(((Clic.x > 0) && (Clic.x < 100)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'A';} //Si l'endroit du clic est sur la touche du A alors la lettre choisie sera A
    if(((Clic.x > 100) && (Clic.x < 200)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'B';} //Etc...
    if(((Clic.x > 200) && (Clic.x < 300)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'C';}
    if(((Clic.x > 300) && (Clic.x < 400)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'D';}
    if(((Clic.x > 400) && (Clic.x < 500)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'E';}
    if(((Clic.x > 500) && (Clic.x < 600)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'F';}
    if(((Clic.x > 600) && (Clic.x < 700)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'G';}
    if(((Clic.x > 700) && (Clic.x < 800)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'H';}
    if(((Clic.x > 800) && (Clic.x < 900)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'I';}
    if(((Clic.x > 900) && (Clic.x < 1000)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'J';}
    if(((Clic.x > 1000) && (Clic.x < 1100)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'K';}
    if(((Clic.x > 1100) && (Clic.x < 1200)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'L';}
    if(((Clic.x > 1200) && (Clic.x < 1300)) && ((Clic.y > 98) && (Clic.y < 196))){LettreChoisie = 'M';}
    if(((Clic.x > 0) && (Clic.x < 100)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'N';}
    if(((Clic.x > 100) && (Clic.x < 200)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'O';}
    if(((Clic.x > 200) && (Clic.x < 300)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'P';}
    if(((Clic.x > 300) && (Clic.x < 400)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'Q';}
    if(((Clic.x > 400) && (Clic.x < 500)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'R';}
    if(((Clic.x > 500) && (Clic.x < 600)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'S';}
    if(((Clic.x > 600) && (Clic.x < 700)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'T';}
    if(((Clic.x > 700) && (Clic.x < 800)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'U';}
    if(((Clic.x > 800) && (Clic.x < 900)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'V';}
    if(((Clic.x > 900) && (Clic.x < 1000)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'W';}
    if(((Clic.x > 1000) && (Clic.x < 1100)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'X';}
    if(((Clic.x > 1100) && (Clic.x < 1200)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'Y';}
    if(((Clic.x > 1200) && (Clic.x < 1300)) && ((Clic.y > 0) && (Clic.y < 98))){LettreChoisie = 'Z';}
}

void Jeu()
{
    int MotTableau[TailleMot]; //Un tableau qui va répertorier les lettres trouvées: false si non, true si oui.
    char MotAffiche[10] = {NULL}; //Au début ce sera _ _  (Si mot à 2 lettres) et se complétera au fil du temps
    MotTrouve = false; //Si le joueur viens de gagner, il aura un nouveau mot donc, non trouvé
    Erreur = 0; //On remet les coups à 0 si le joueur viens de gagner
    draw_fill_rectangle(Dessin,EffDessin,blanc); //On retire le dessin si le joueur viens de gagner
    i=0;

    while(i < (strlen(MotATrouver))) //On initialise le tableau du mot: Si 0 = Lettre non trouvée, si 1, lettre trouvée
    {
        MotTableau[i] = 0; //On met toutes les cases à 0 du coup
        i++;
    }

    i=0;

    while((MotTrouve == false) && (Erreur < 11)) //Tant que le joueur n'a pas perdu et le mot n'a pas été  trouvé
    {
        Multiple2 = false; //Si 2 lettre multiple, true
        Multiple3 = false; //Si 3 lettre multiple, true
        bool passe = false; //passe et passe2 vont nous servir pour les lettres multiples, vous verrez
        bool passe2 = false;
        MultiplePosition = 0; //Enregistrer la position de la 2ème lettre multiple
        MultiplePosition3 = 0; //Enregistrer la position de la 3eme lettre multiple
        Clic = wait_clic(); //On attends un clic de la part du joueur
        Clavier(); //On appelle la fonction qui va s'occuper de voir quelle lettre le joueur à choisi

        if(Clic.y < 196) //Si une touche a été appuyée
        {
            if(strchr(MotATrouver, LettreChoisie) != NULL) //Si la lettre choisie est une lettre du mot
            {
                i = 0;

                while(i < (strlen(MotATrouver))) //Recherche de la position de la lettre dans le mot
                {

                    if(LettreChoisie == MotATrouver[i]) //Si c'est cette position
                    {
                        EndroitLettre = i; //On sauvegarde la position de la lettre
                        MotTableau[i] = 1; //On met 1 dans le tableau à la colonne i du mot à trouver car on a trouvé la lettre à la position i
                        break; //Stop! On a trouvé la position, donc on stop la boucle
                    }

                    i++; //On incrémente i, si ce n'est pas la bonne position, donc on avance dans le mot vers la droite pour trouver la position
                }

                i = 0;
                int Pos = 0, Pos2 = 0; //Variables pour enregistrer une mémoire (Genre "je suis passé par là, tu peux passer à l'étape suivante")

                while(i < (strlen(MotATrouver))) //Recherche si il y a des lettres multiples
                {

                    if(LettreChoisie == MotATrouver[i]) //Déjà on refait le test si y'a bien une lettre identique dans le mot
                    {
                        passe = true; //Si oui, on a passé le premier test: Il y a bien une lettre.
                        Pos++;
                        Pos2++;

                        if((passe == true) && (passe2 == false) && (Pos == 2)) //Maintenant on regarde si il y en a pas une deuxième identique en continuant
                        {
                            Multiple2 = true; //Si oui, alors la variable Multiple2 est vraie: Il y a 2 lettres multiples
                            MultiplePosition = i; //On garde la position de la deuxième lettre multiple
                            passe2 = true; //Et on passe au test supérieur: Si il y a 3 lettres multiples
                            Pos2++;
                        }

                        if((passe2 == true) && (Pos2 == 4)) //Maintenant on regarde si il y en a pas une troisième identique en continuant
                        {
                            Multiple3 = true; //Si oui, alors la variable Multiple3 est vraie: Il y a 3 lettres multiples
                            MultiplePosition3 = i; //On garde la position de la troisième lettre multiple
                        }
                    }

                    i++; //Evidemment, on incrémente
                }

                i = 0; //On remet i à zéro pour le while suivant

                while(i < (strlen(MotATrouver))) //Maintenant ajoutons la lettre dans le "_ _ _ _"
                {
                    if(MotTableau[i] == 1)  //En revanche, si ici on a trouvé une lettre
                    {
                        MotAffiche[i] = MotATrouver[i]; //On remplace "_" par la lettre trouvée
                    }
                    else //Si à cet endroit, on a pas trouvé la lettre
                    {
                        MotAffiche[i] = '_'; //Eh bien la lettre reste inconnue
                    }

                    i++;
                }

                if(Multiple2 == true) //Si il y a 2 lettres multiples (ou plus)
                {
                    MotAffiche[MultiplePosition] = MotATrouver[MultiplePosition]; //On ajoute la lettre dans le mot qui va s'afficher car quand on trouve E dans "METTRE" bah ça nous mettera _E___E
                    MotTableau[MultiplePosition] = 1; //Et donc on met 1 car la lettre multiple sera trouvée
                }

                if(Multiple3 == true) //Si il y a 3 lettres multiples (ou plus)
                {
                    MotAffiche[MultiplePosition3] = MotATrouver[MultiplePosition3]; //Pareil comme au dessus (Ligne 240)
                    MotTableau[MultiplePosition3] = 1; //Et donc on met 1 car la lettre multiple sera trouvée
                }

                draw_fill_rectangle(EffacerMot1,EffacerMot2,0xC3C3C3); //On efface le mot précédant pour renouveler avec les lettres trouvées
                if(TailleMot == 2) {aff_pol(MotAffiche, 250, Taille2, noir);} //Si la taille du mot est de deux lettres, alors les " _ _ " avec les lettres affichés aura une taille appropriée à sa taille du mot
                if(TailleMot == 3) {aff_pol(MotAffiche, 150, Taille3, noir);} //Pareil...
                if(TailleMot == 4) {aff_pol(MotAffiche, 125, Taille4, noir);}
                if(TailleMot == 5) {aff_pol(MotAffiche, 100, Taille5, noir);}
                if(TailleMot == 6) {aff_pol(MotAffiche, 75, Taille6, noir);}
                if(TailleMot == 7) {aff_pol(MotAffiche, 68, Taille7, noir);}
                if(TailleMot == 8) {aff_pol(MotAffiche, 60, Taille8, noir);}
                if(TailleMot == 9) {aff_pol(MotAffiche, 52, Taille9, noir);}
                if(TailleMot == 10) {aff_pol(MotAffiche, 46, Taille10, noir);}
            }
            else //Si la lettre n'est pas dans le mot
            {
                Erreur++; //Eh bien, le nombre d'erreurs augmente (Coups étant le nombre d'erreurs)
                Erreurs(); //On fait appel à la fonction qui va s'occuper d'afficher le dessin du pendu
            }
        }

        if((strcmp(MotAffiche, MotATrouver) == 0) && (Multi == false)) //On va tester à chaque tour si le mot entier à été trouvé, donc si la chaine de caractères des lettres trouvées est identique au mot, et c'est en mode solo
        {
            MotTrouve = true; //Si oui, le mot a été trouvé.
            Victoires++;
            draw_fill_rectangle(EffVictoire1,EffVictoire2,0xC3C3C3); //On efface le nb de victoires précédant pour mettre à jour
            sprintf(NbVictoires, "%d", Victoires); //On crée une chaîne de caractère qui aura dedans le nombres de victoires
            aff_pol(NbVictoires,70,Victoire,noir); //On affiche le nombre (Donc la chaîne de caractères)
        }

        if((strcmp(MotAffiche, MotATrouver) == 0) && (Multi == true)) //Pareil mais il s'agit ici de la victoire en mode Multi
        {
            POINT Perd, Perd2, Perd3;
            Perd.x = 10; Perd.y = 450;
            Perd2.x = 10; Perd2.y = 400;
            Perd3.x = 10; Perd3.y = 350;
            aff_pol("Le joueur qui a devine le mot a gagne", 33, Perd, rouge); //On affiche le texte qui indique que le joueur a perdu
            aff_pol("Tandis que l'autre a perdu!", 33, Perd2, rouge);
            aff_pol("Appuyez sur ECHAP pour quitter.", 33, Perd3, rouge);
            wait_escape(); //On atteif(((Clic.x > 1096) && (Clic.x < 1300)) && ((Clic.y > 0) && (Clic.y < 98))){Supprimer = true;}nds que le joueur tape echap pour que le jeu se ferme
        }
    }
}

void Erreurs() //Fonction qui dessine le dessin du pendu en fonction du nombre d'erreurs
{
    Dessin.x = 860; Dessin.y = 600; //Initialisation de ses coordonnées (Point en haut à gauche je rappelle)
    EffDessin.x = 1282; EffDessin.y = 250;

    if(Erreur == 1) {affiche_image("Data/Img/Progress/1", Dessin, WIDTH, HEIGHT);} //Si on a fait une erreur, on dessine le dessin du pendu à la 1ère étape
    if(Erreur == 2) {affiche_image("Data/Img/Progress/2", Dessin, WIDTH, HEIGHT);} //Etc...
    if(Erreur == 3) {affiche_image("Data/Img/Progress/3", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 4) {affiche_image("Data/Img/Progress/4", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 5) {affiche_image("Data/Img/Progress/5", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 6) {affiche_image("Data/Img/Progress/6", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 7) {affiche_image("Data/Img/Progress/7", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 8) {affiche_image("Data/Img/Progress/8", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 9) {affiche_image("Data/Img/Progress/9", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 10) {affiche_image("Data/Img/Progress/10", Dessin, WIDTH, HEIGHT);}
    if(Erreur == 11) {affiche_image("Data/Img/Progress/Lose", Dessin, WIDTH, HEIGHT);}
}

void Perdu()
{
    POINT Perd, Perd2, Perd3;
    Perd.x = 10; Perd.y = 450;
    Perd2.x = 10; Perd2.y = 400;
    Perd3.x = 10; Perd3.y = 350;
    draw_fill_rectangle(EffacerMot1,EffacerMot2,0xC3C3C3);
    if(TailleMot == 2) {aff_pol(MotATrouver, 250, Taille2, noir);} //On affiche le mot que le joueur n'a pas pu trouvé si il fait 2 lettres (Pour la taille)
    if(TailleMot == 3) {aff_pol(MotATrouver, 150, Taille3, noir);} //Ainsi de suite pour les différentes tailles du mot
    if(TailleMot == 4) {aff_pol(MotATrouver, 125, Taille4, noir);}
    if(TailleMot == 5) {aff_pol(MotATrouver, 100, Taille5, noir);}
    if(TailleMot == 6) {aff_pol(MotATrouver, 75, Taille6, noir);}
    if(TailleMot == 7) {aff_pol(MotATrouver, 68, Taille7, noir);}
    if(TailleMot == 8) {aff_pol(MotATrouver, 60, Taille8, noir);}
    if(TailleMot == 9) {aff_pol(MotATrouver, 52, Taille9, noir);}
    if(TailleMot == 10) {aff_pol(MotATrouver, 46, Taille10, noir);}

    if(Multi == true) //Si on est en Multi
    {
        aff_pol("Le joueur qui a cree le mot a gagne", 33, Perd, rouge); //On affiche le texte qui indique que le joueur a perdu
        aff_pol("Tandis que l'autre a perdu!", 33, Perd2, rouge);
        aff_pol("Appuyez sur ECHAP pour quitter.", 33, Perd3, rouge);
    }
    else //Si on est en solo
    {
        aff_pol("Vous avez perdu...", 50, Perd, rouge); //On affiche le texte qui indique que le joueur a perdu
        aff_pol("Appuyez sur ECHAP pour", 50, Perd2, rouge);
        aff_pol("quitter.", 50, Perd3, rouge);
    }

    wait_escape(); //On atteif(((Clic.x > 1096) && (Clic.x < 1300)) && ((Clic.y > 0) && (Clic.y < 98))){Supprimer = true;}nds que le joueur tape echap pour que le jeu se ferme
}

void DemandeMot()
{
    char MotAffiche[100] = {NULL};
    int TableauMot[100] = {0};
    affiche_image("Data/Img/Multi", Graphics, WIDTH, HEIGHT); //On affiche l'image où il devra choisir le mot qu'il fera deviner
    bool MotChoisi = false; //True quand le joueur aura fini de choisir le mot qu'il fera deviner à l'autre joueur
    bool DerniereLettre = false; //Sert à reconnaitre quand on est au dernier caractère si on veut le supprimer
    POINT Mot, EffMot; //Point qui va permettre l'écriture du mot en temps réel (Quand touche appuyé, la lettre s'affiche, et EffMot pour l'effacer ensuite si il efface une lettre
    Mot.x = 339; Mot.y = 345;
    EffMot.x = 1031; EffMot.y = 266;
    i = 0;

    while(MotChoisi == false)
    {
        bool Confirmer = false; //True si le joueur a clique sur le bouton "Confirmer"
        bool Supprimer = false; //True si le joueur a clique sur le bouton "supprimer lettre"
        bool YaLettre = true; //Sert à reconnaître que il n'y a pas de lettre (Si faire supprimer alors que y'en a pas), false, si c'est le cas
        bool DerniereCase = false; //Sert à reconnaitre que le curseur des lettres est à la dernière lettre, true si le curseur est à la dernière
        Clic = wait_clic();
        Clavier();
        if(((Clic.x > 1096) && (Clic.x < 1300)) && ((Clic.y > 202) && (Clic.y < 302))){Supprimer = true;} //Si cliqué sur Supprimer lettre
        if(((Clic.x > 1096) && (Clic.x < 1300)) && ((Clic.y > 303) && (Clic.y < 390))){Confirmer = true;} //Si cliqué sur confirmer

        if(Clic.y < 196) //Si une touche lettre a été appuyée
        {
            MotAffiche[i] = LettreChoisie; //La lettre choisie dans le clavier sera mise à la position i dans le mot (i change en fonction si on supprime etc, genre le curseur des lettres)
            draw_fill_rectangle(Mot,EffMot,0xC3C3C3); //On met à jour la modification du mot à l'écran pour que le "choissiseur" se repère
            aff_pol(MotAffiche, 70, Mot, noir); //Comme en haut
            TableauMot[i] = 1; //Une lettre, donc on met 1
        }

        if(Supprimer == true) //Si "supprimer" a était choisi
        {
            if(i < 1) //Si il n'y a pas de lettres, donc position 0
            {
                YaLettre = false; //On met en false, pour pas que ça fasse plus d'espace
            }

            if(i < 10) //Si on a pas encore atteint la limite (Limite = 9 car le 0 est compté)
            {
                i--; //On retourne sur le i d'avant pour supprimer le lettre précédente
            }
            if(YaLettre == true) //Si il y a déjà des lettres entrées
            {
                if(i == 8) //Si on en est à la dernière lettre
                {
                    MotAffiche[9]= ' ';
                    MotAffiche[10]= ' '; //Alors on supprime la dernière lettre (dû à des bugs j'ai du faire ça)
                    TableauMot[10] = 0; //Et on met des 0 car y'a pas de lettres
                    TableauMot[9] = 0;
                }

                MotAffiche[i] = ' '; //Si on en est pas au dernier caractère, on supprime, toujours de manière normale on met un espace, le NULL bug.
                TableauMot[i] = 0;
                i--; //On reretourne en arrière pour que l'incrémentation du i aille donc sur la lettre suivante où le joueur pourra mettre une lettre à l'endroit où il a supprimé donc encore précédent donc on décrémente i
                draw_fill_rectangle(Mot,EffMot,0xC3C3C3); //On met à jour la modification du mot à l'écran pour que le "choissiseur" se repère
                aff_pol(MotAffiche, 70, Mot, noir); //Comme en haut
            }
        }

        if(Confirmer == true) //Si "confirmer" a était choisi
        {
            i=0;
            while(TableauMot[i] == 1) //Pour la taille du mot et le mettre dans la chaine "Mot à trouver"
            {
                MotATrouver[i] = MotAffiche[i]; //Donc tant que il y a une lettre (1), on ajoute dans Mot à trouver
                i++;
            }
            break; //Si il y a un 0 donc pas de lettre, on casse la boucle et "Mot à trouver
        }

        if((Supprimer == true) || (Confirmer == true) || (Clic.y < 196)){i++;} //On incrémente si le joueur à soit supprimer, confirmer ou choisi une lettre, sinon on fait rien car il a rien fait

        if(i == 10) //Si i est à 10 (Taille maximale d'un mot)..
        {
            i--; //..On le remet à 9 car le joueur ne doit pas écrire un mot supérieur à 10 caractères
        }
    }
}

void PerduMulti()
{
    POINT Perd, Perd2, Perd3;
    Perd.x = 10; Perd.y = 450;
    Perd2.x = 10; Perd2.y = 400;
    Perd3.x = 10; Perd3.y = 350;
    draw_fill_rectangle(EffacerMot1,EffacerMot2,0xC3C3C3);
    if(TailleMot == 2) {aff_pol(MotATrouver, 250, Taille2, noir);} //On affiche le mot que le joueur n'a pas pu trouvé si il fait 2 lettres (Pour la taille)
    if(TailleMot == 3) {aff_pol(MotATrouver, 150, Taille3, noir);} //Ainsi de suite pour les différentes tailles du mot
    if(TailleMot == 4) {aff_pol(MotATrouver, 125, Taille4, noir);}
    if(TailleMot == 5) {aff_pol(MotATrouver, 100, Taille5, noir);}
    if(TailleMot == 6) {aff_pol(MotATrouver, 75, Taille6, noir);}
    if(TailleMot == 7) {aff_pol(MotATrouver, 68, Taille7, noir);}
    if(TailleMot == 8) {aff_pol(MotATrouver, 60, Taille8, noir);}
    if(TailleMot == 9) {aff_pol(MotATrouver, 52, Taille9, noir);}
    if(TailleMot == 10) {aff_pol(MotATrouver, 46, Taille10, noir);}
    aff_pol("Vous avez perdu...", 50, Perd, rouge); //On affiche le texte qui indique que le joueur a perdu
    aff_pol("Appuyez sur ECHAP pour", 50, Perd2, rouge);
    aff_pol("quitter.", 50, Perd3, rouge);
    wait_escape(); //On atteif(((Clic.x > 1096) && (Clic.x < 1300)) && ((Clic.y > 0) && (Clic.y < 98))){Supprimer = true;}nds que le joueur tape echap pour que le jeu se ferme
}
