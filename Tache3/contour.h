
#ifndef _CONTOUR_H_
#define _CONTOUR_H_

#include "geom2d.h"
#include "image.h"
#include "sequence_point.h"

typedef enum {Nord, Est, Sud, Ouest} Orientation;

typedef struct {
  int x, y;
  Orientation o;
} Robot;

/* initialiser le robot r en position (x,y) et orientation o */
void init_robot(Robot *r, int x, int y, Orientation o);

/* faire avancer le robot d'une case */
void avancer(Robot *r);

/* faire tourner le robot à gauche */
void tourner_a_gauche(Robot *r);

/* faire tourner le robot à droite */
void tourner_a_droite(Robot *r);

/* recupere la position de la case du robot */
void position(Robot *r, int *x, int *y);

/* recupere la position en abscisse de la case du robot */
int abscisse(Robot *r);

/* recupere la position en ordonnee de la case du robot */
int ordonnee(Robot *r);

/* recupere l'orientation du robot */
Orientation orient(Robot *r);

/* recupere la position de la case devant le robot */
void position_devant(Robot *r, int *x, int *y);

/* Logic de calcul de la nouvelle orientation */
void nouvelle_orientation(Robot *r, int x, int y, Image I);

/* Fonction qui renvoi les coordonees du pixel de depart */
Point trouver_pixel_depart(Image I);

Contour algo_contour(Image I, char *file_name);

#endif /* _CONTOUR_H_ */