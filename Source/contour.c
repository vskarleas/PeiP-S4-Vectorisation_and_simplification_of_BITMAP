#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "geom2d.h"
#include "contour.h"
#include "image.h"

/* On utilise un robot pour faire la lecture des bits sur l'image pbm. 
Il a besoin les coordonees x, y et l'orientation*/
void init_robot(Robot *r, int x, int y, Orientation o)
{
    r->x = x;
    r->y = y;
    r->o = o;
}

/* faire avancer le robot d'une case */
void avancer(Robot *r)
{
    switch (r->o)
    {
    case Nord:
        r->y--;
        break;
    case Sud:
        r->y++;
        break;
    case Ouest:
        r->x--;
        break;
    case Est:
        r->x++;
    }
}

/* faire tourner le robot à gauche */
void tourner_a_gauche(Robot *r)
{
    switch (r->o)
    {
    case Nord:
        r->o = Ouest;
        break;
    case Ouest:
        r->o = Sud;
        break;
    case Sud:
        r->o = Est;
        break;
    case Est:
        r->o = Nord;
        break;
    }
}

/* faire tourner le robot à droite */
void tourner_a_droite(Robot *r)
{
    switch (r->o)
    {
    case Nord:
        r->o = Est;
        break;
    case Ouest:
        r->o = Nord;
        break;
    case Sud:
        r->o = Ouest;
        break;
    case Est:
        r->o = Sud;
        break;
    }
}

/* recupere la position de la case (pixel) du robot */
void position(Robot *r, int *x, int *y)
{
    *x = r->x;
    *y = r->y;
}

/* recupere la position en abscisse de la case du robot */
int abscisse(Robot *r)
{
    return r->x;
}

/* recupere la position en ordonnee de la case du robot */
int ordonnee(Robot *r)
{
    return r->y;
}

/* recupere l'orientation du robot */
Orientation orient(Robot *r)
{
    return r->o;
}

/* recupere la position de la case (pixel) devant le robot */
void position_devant(Robot *r, int *x, int *y)
{
    switch (r->o)
    {
    case Nord:
        *x = r->x;
        *y = r->y - 1;
        break;
    case Sud:
        *x = r->x;
        *y = r->y + 1;
        break;
    case Ouest:
        *x = r->x - 1;
        *y = r->y;
        break;
    case Est:
        *x = r->x + 1;
        *y = r->y;
    }
}

/* Modification de l'orinetation par rapport le prochaine pixel noir [qui est toujours à droit du robot]*/
void nouvelle_orientation(Robot *r, int x, int y, Image I)
{
    Pixel pG;
    Pixel pD;
    switch (r->o)
    {
        //On traite les differents cas pour chaque orintation par rapport la position du prochaine pixel noir 
    case (Est):
        //Formule pour le calcul du voisins d’un pixel (x, y).
        //La logique est la meme pour chaque orientation
        pG = get_pixel_image(I, x + 1, y);
        pD = get_pixel_image(I, x + 1, y + 1);
        if (pG == NOIR)
        {
            r->o = Nord;
            break;
        }
        else if (pD == BLANC)
        {
            r->o = Sud;
            break;
        }
        else
        {
            break;
        }
    case (Nord):
        pG = get_pixel_image(I, x, y);
        pD = get_pixel_image(I, x + 1, y);
        if (pG == NOIR)
        {
            r->o = Ouest;
            break;
        }
        else if (pD == BLANC)
        {
            r->o = Est;
            break;
        }
        else
        {
            break;
        }
    case (Sud):
        pG = get_pixel_image(I, x + 1, y + 1);
        pD = get_pixel_image(I, x, y + 1);
        if (pG == NOIR)
        {
            r->o = Est;
            break;
        }
        else if (pD == BLANC)
        {
            r->o = Ouest;
            break;
        }
        else
        {
            break;
        }
    case (Ouest):
        pG = get_pixel_image(I, x, y + 1);
        pD = get_pixel_image(I, x, y);
        if (pG == NOIR)
        {
            r->o = Sud;
            break;
        }
        else if (pD == BLANC)
        {
            r->o = Nord;
            break;
        }
        else
        {
            break;
        }
    }
}

/*Detection du premier pixel noir avec un pixel blanc au nord de celui ci*/
Point trouver_pixel_depart(Image I)
{
    int hauteur = hauteur_image(I);
    int largeur = largeur_image(I);
    Point depart;
    Pixel voisin;
    Pixel A;
    for (int i = 1; i <= hauteur; i++)
    {
        for (int j = 1; j <= largeur; j++)
        {
            A = get_pixel_image(I, j, i);
            switch (A)
            {
            case (BLANC):
                break;
            case (NOIR):
                if (j != largeur)
                {
                    voisin = get_pixel_image(I, j, i - 1);
                    if (voisin == BLANC)
                    {
                        depart = set_point(j, i);
                        return depart;
                    }
                }
            }
        }
    }
    //Traitment en cas ou aucun pixel est trouvé
    printf("Can't a pixel to get initialised. The return point is (-1.0 , -1.0)");
    //Si on detecte des nombres negatives dans les demarches suivantes, on arrete l'operation
    depart = set_point(-1.0, -1.0);
    return depart;
}

/*Detection de tous les pixels noir avec un pixel blanc au nord de celui ci (utilisé dans la detection des contours de l'image)*/
Image mask_image(Image I)
{
    int hauteur = hauteur_image(I);
    int largeur = largeur_image(I);

    Image mask;
    mask = creer_image(largeur, hauteur);

    Pixel voisin;
    Pixel A;
    for (int i = 1; i <= hauteur; i++)
    {
        for (int j = 1; j <= largeur; j++)
        {
            A = get_pixel_image(I, j, i);
            switch (A)
            {
            case (BLANC):
                break;
            case (NOIR):
                voisin = get_pixel_image(I, j, i - 1);
                if (voisin == BLANC)
                {
                    set_pixel_image(mask, j, i, NOIR);
                }
            }
        }
    }
    return mask;
}

/* Verification si l'image est vide (il y a que des pixels blancs) - utilisé pour finir la recherche des contours */
bool image_blache(Image I)
{
    Pixel A;
    for (int i = 1; i <= largeur_image(I); i++)
    {
        for (int h = 1; h <= hauteur_image(I); h++)
        {
            A = get_pixel_image(I, i, h);
            switch (A)
            {
            case (NOIR):
                return false; // false si et seulment si n'est pas blanche
            default:
                break;
            }
        }
    }
    return true;
}

// Returns the contour and also writes the contour <file>.txt - This function will be modfied in order to write multiple contours in the <file>.tx
Contour algo_contour(Image I, char *file_name)
{
    // Managing specific contour
    Point depart = trouver_pixel_depart(I);
    if (depart.x == -1 && depart.y == -1)
    {
        fprintf(stderr, "No starting point found");
        exit(-1);
    }
    else
    {
        // Initialisation du position du robot
        Point rb = set_point(depart.x - 1, depart.y - 1);

        /* Code for Partie 1 is commented since Partie 2 is what is required for the rest of the project*/
        Point original_position = set_point(depart.x - 1, depart.y - 1);
        Robot robot;
        init_robot(&robot, rb.x, rb.y, Est);

        // Initialisation de la sequence des points
        Contour c;
        c = creer_liste_Point_vide();

        // Parcours du robot pour la creation du contour
        bool repeat = true;
        while (repeat)
        {
            // Faire partie du Partie 1
            ajouter_element_liste_Point(&c, rb);
            avancer(&robot);
            rb = set_point(robot.x, robot.y);
            nouvelle_orientation(&robot, rb.x, rb.y, I);
            if ((robot.o == Est) && (original_position.x == rb.x) && (original_position.y == rb.y))
            {
                repeat = false;
            }
        }
        ajouter_element_liste_Point(&c, rb);

        // Managing file writing
        // File in which will be writted the contour
        FILE *fptr;

        // use appropriate location if you are using MacOS or Linux
        fptr = fopen(file_name, "w");
        if (fptr == NULL)
        {
            printf("TXT File Error!");
            exit(1);
        }

        fprintf(fptr, "1\n");

        Tableau_Point TP = sequence_points_liste_vers_tableau(c);
        int k;
        int nP = TP.taille;
        fprintf(fptr, "\n");
        fprintf(fptr, "%d\n", nP);
        for (k = 0; k < nP; k++)
        {
            Point P = TP.tab[k];
            fprintf(fptr, "%.1f %.1f\n", P.x, P.y);
        }
        free(TP.tab);
        return c;
    }
}

/* Returns the liste of countours for an image */
Liste_Contours algo_contours(Image I)
{
    //Initialisation de la liste des contours (type liste chainee)
    Liste_Contours liste;
    liste = creer_liste_Contours_vide();

    Image mask;
    //Initilisation de l'image mask
    mask = mask_image(I);
    //Tant que la nouvelle image n'est pas vide, continue de chercher pour des contours 
    while (!image_blache(mask))
    {
        Point depart = trouver_pixel_depart(mask);
        Point rb = set_point(depart.x - 1, depart.y - 1);

        Robot robot;
        init_robot(&robot, rb.x, rb.y, Est);

        Contour c;
        c = creer_liste_Point_vide();

        Point original_position = set_point(depart.x - 1, depart.y - 1);
        bool repeat = true;
        while (repeat)
        {
            ajouter_element_liste_Point(&c, rb);
            // Retrouver le pixel par rapport l'orientation du robot
            switch (robot.o)
            {
            case (Est):
                set_pixel_image(mask, robot.x + 1, robot.y + 1, BLANC);
                break;
            case (Ouest):
                set_pixel_image(mask, robot.x, robot.y + 1, BLANC);
                break;
            default:
                break;
            }
            avancer(&robot);
            rb = set_point(robot.x, robot.y);
            nouvelle_orientation(&robot, rb.x, rb.y, I);

            if ((robot.o == Est) && (original_position.x == rb.x) && (original_position.y == rb.y))
            {
                repeat = false;
            }
        }
        ajouter_element_liste_Point(&c, rb);
        ajouter_element_liste_Contours(&liste, c);
    }
    return liste;
}

/* Creation du fichier .txt des contours  */
void ecrire_fichier_contours(Liste_Contours c, char *file_name)
{
    FILE *fptr;
    fptr = fopen(file_name, "w");
    if (fptr == NULL)
    {
        printf("TXT File Error!");
        exit(1);
    }
    int id = 0;

    Cellule_Liste_Contours *el;
    el = c.first;
    if (el == NULL)
    {
        return;
    }
    else
    {
        while (el != NULL)
        {
            id++;
            fprintf(fptr, "%d\n", id);
            Tableau_Point TP = sequence_points_liste_vers_tableau(el->data);
            int k;
            int nP = TP.taille;
            fprintf(fptr, "\n");
            fprintf(fptr, "%d\n", nP);
            for (k = 0; k < nP; k++)
            {
                Point P = TP.tab[k];
                //Ecriture des proints sur le fichier, en ordre
                fprintf(fptr, "%.1f %.1f\n", P.x, P.y);
            }
            free(TP.tab);
            fprintf(fptr, "\n");
            fprintf(fptr, "\n");
            el = el->suiv;
        }
    }
}

/* Aficher sur le terminal nombre des segments totals, ainsi que 
le nombre des contours pour une liste des contours. La notion des segments 
est la notion des segments annoncè avant tache 5*/
void contours_data(Liste_Contours c)
{
    Cellule_Liste_Contours *el;
    el = c.first;

    //Initialisation des compteurs
    int nb = 0;
    int nb_points;
    int segments = 0;
    //Parcour de chaque liste de point
    while (el != NULL)
    {
        nb++;
        nb_points = 0;
        Cellule_Liste_Point *e;
        e = (el->data).first;
        //Parcour de points de la liste des points en question
        while (e != NULL)
        {
            nb_points++; //compteur des points totaux de tous les contours
            e = e->suiv;
        }
        //Calcule des segments 
        segments = segments + (nb_points - 1);
        el = el->suiv;
    }
    printf("Nombre des contours: %d\n", nb);
    printf("Nombre des segments totals: %d\n", segments);
    printf("\n");
}

/* Aficher sur le terminal nombre de segments totals, ainsi que 
le nombre des contours pour une liste des contours. On l'utilise aux 
differents endroits, avant et apres la methode de simplification par segments par exemple.
Ici la ntion des segments, est la notion annoncé à partir de tache 5*/
void contours_data_simplification(Liste_Contours c)
{
    Cellule_Liste_Contours *el;
    el = c.first;

    //Initialisation des compteurs
    int nb = 0;
    int nb_points;
    int segments = 0;
    //Parcour de chaque liste des points
    while (el != NULL)
    {
        nb++; //compteur des contours
        nb_points = 0;
        Cellule_Liste_Point *e;
        e = (el->data).first;
        //Parcour de points de la liste des points en question
        while (e != NULL)
        {
            nb_points++; //compteur des points totals de tous les contours
            e = e->suiv;
        }
        segments = segments + nb_points; //Calcul des segments
        el = el->suiv;
    }
    //Affichage sur le terminal
    printf("Nombre des contours: %d\n", nb);
    printf("Nombre des segments totals: %d\n", segments / 2);
    printf("\n");
}

/* Affichage de nombre des contours et nombre des courbes de bezier de degree 2 en terminal*/
void contours_data_bezier(Liste_Contours c)
{
    Cellule_Liste_Contours *el;
    el = c.first;
    //Initialisation des compteurs
    int nb = 0;
    int nb_beziers = 0;
    //Parcour de chaque liste des points
    while (el != NULL)
    {
        nb++;
        Cellule_Liste_Point *e;
        e = (el->data).first;
        //Parcour de points de la liste des points en question
        while (e != NULL)
        {
            //Chaque trois points correspondent en une courbe de bezier de degree 2.
            e = e->suiv;
            e = e->suiv;
            e = e->suiv;
            nb_beziers++;
        }
        el = el->suiv;
    }
    //Affichage sur le terminal
    printf("Nombre des contours: %d\n", nb);
    printf("Nombre des bezier totals: %d\n", nb_beziers);
    printf("\n");
}

/* Affichage de nombre des contours et nombre des courbes de bezier de degree 3 en terminal*/
void contours_data_bezier3(Liste_Contours c)
{
    Cellule_Liste_Contours *el;
    el = c.first;
    int nb = 0;
    int nb_beziers = 0;
    //Parcour de chaque liste des points
    while (el != NULL)
    {
        nb++;
        Cellule_Liste_Point *e;
        e = (el->data).first;
        //Parcour de points de la liste des points en question
        while (e != NULL)
        {
            //Chaque quadruplet des points correspondent en une courbe de bezier de degree 3.
            e = e->suiv;
            e = e->suiv;
            e = e->suiv;
            e = e->suiv;
            nb_beziers++;
        }
        el = el->suiv;
    }
    //Affichage sur le terminal
    printf("Nombre des contours: %d\n", nb);
    printf("Nombre des bezier totals: %d\n", nb_beziers);
    printf("\n");
}

/* Creation du fichier .eps pour la methide de simplification par segments (stroke)*/
void create_postscript(Contour c, char *file_name, int hauteur, int largeur)
{
    // Extension managment
    char *no_extension = strtok(file_name, ".");
    char *with_extension = malloc(strlen(no_extension) + 4);
    strcpy(with_extension, no_extension);
    strcat(with_extension, ".eps"); // concantenation

    //File managemnent
    FILE *fptr;
    fptr = fopen(with_extension, "w");
    if (fptr == NULL)
    {
        printf("EPS File Error!");
        exit(1);
    }

    //File redaction
    fprintf(fptr, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(fptr, "%%%%BoundingBox:   %d   %d   %d   %d\n", 0, 0, largeur, hauteur);

    Cellule_Liste_Point *el;
    el = c.first;
    fprintf(fptr, "%.0f %.0f moveto ", el->data.x, hauteur - el->data.y);
    el = el->suiv;
    //Parcour des pints de la liste des points passe en argument
    while (el != NULL)
    {
        fprintf(fptr, "%.0f %.0f lineto ", el->data.x, hauteur - el->data.y);
        el = el->suiv;
    }
    fprintf(fptr, "\n");
    fprintf(fptr, "0.0 setlinewidth\n");
    //Writing and visualisation policy applied [stroke]
    fprintf(fptr, "stroke\n");
    fprintf(fptr, "showpage\n");
    fclose(fptr);
    return;
}

/* Creation du fichier .eps pour la methide de simplification par segments (fill)*/
void create_postscript_fill(Contour c, char *file_name, int hauteur, int largeur)
{
    // Extension managment
    char *no_extension = strtok(file_name, ".");
    char *with_extension = malloc(strlen(no_extension) + 4);
    strcpy(with_extension, no_extension);
    strcat(with_extension, ".eps");

    //File redaction
    FILE *fptr;
    fptr = fopen(with_extension, "w");
    if (fptr == NULL)
    {
        printf("EPS File Error!");
        exit(1);
    }

    fprintf(fptr, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(fptr, "%%%%BoundingBox:   %d   %d   %d   %d\n", 0, 0, largeur, hauteur);

    Cellule_Liste_Point *el;
    el = c.first;
    fprintf(fptr, "%.0f %.0f moveto ", el->data.x, hauteur - el->data.y);
    el = el->suiv;
    //Parcour des pints de la liste des points passe en argument
    while (el != NULL)
    {
        fprintf(fptr, "%.0f %.0f lineto ", el->data.x, hauteur - el->data.y);
        el = el->suiv;
    }
    fprintf(fptr, "\n");
    //Writing and visualisation policy applied [fill]
    fprintf(fptr, "fill\n");
    fprintf(fptr, "showpage\n");
    fclose(fptr);
    return;
}

/* Creation du fichier .eps pour la methode de simplification par segments (mode remplisage [fill])*/
/* On traite la lecture d'une liste des contours et pas une seule contour qu'on a fair ci-dessous */
void create_postscript_contours(Liste_Contours c, char *file_name, int hauteur, int largeur) // Mode remplisage uniquement
{
    // Extension managment
    char *no_extension = strtok(file_name, ".");
    char *with_extension = malloc(strlen(no_extension) + 4);
    strcpy(with_extension, no_extension);
    strcat(with_extension, ".eps"); // concantenation

    //File managemnt
    FILE *fptr;
    fptr = fopen(with_extension, "w");
    if (fptr == NULL)
    {
        printf("EPS File Error!");
        exit(1);
    }

    //File redaction
    fprintf(fptr, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(fptr, "%%%%BoundingBox:   %d   %d   %d   %d\n", 0, 0, largeur, hauteur);
    fprintf(fptr, "\n");
    Cellule_Liste_Contours *al;
    al = c.first;
    //Parcour de la liste des contours
    while (al != NULL)
    {
        Cellule_Liste_Point *el;
        el = (al->data).first;
        fprintf(fptr, "%.0f %.0f moveto ", el->data.x, hauteur - el->data.y);
        el = el->suiv;
        //Parcour des point de chaqye contour en question
        while (el != NULL)
        {
            fprintf(fptr, "%.0f %.0f lineto ", el->data.x, hauteur - el->data.y);
            el = el->suiv;
        }
        fprintf(fptr, "\n");
        al = al->suiv;
    }
    fprintf(fptr, "fill\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "showpage\n");
    fclose(fptr);
    return;
}

/* Simplifcaion d'une image par segments en maniere recursif */
Contour simplification_douglas_peucker(Tableau_Point T, int j1, int j2, double d)
{
    // Segment creation
    Segment S;
    S.A = T.tab[j1];
    S.B = T.tab[j2];

    // Variable initialisations
    double distance;
    double max_distance = 0;
    int far_away;

    //Douglas Peucker algorithme
    for (int i = j1 + 1; i < j2; i++)
    {
        distance = distance_point_segment(T.tab[i], S);
        if (max_distance < distance)
        {
            max_distance = distance;
            far_away = i;
        }
    }

    if (max_distance <= d)
    {
        Contour L;
        L = creer_liste_Point_vide();
        ajouter_element_liste_Point(&L, S.A);
        ajouter_element_liste_Point(&L, S.B);
        return L;
    }
    else
    {
        //Recursvite 
        Contour L1;
        L1 = creer_liste_Point_vide();
        L1 = simplification_douglas_peucker(T, j1, far_away, d);

        Contour L2;
        L2 = creer_liste_Point_vide();
        L2 = simplification_douglas_peucker(T, far_away, j2, d);

        return concatener_liste_Point(L1, L2);
    }
}

// Tache 7
/* Calcul le point d'une courbe de bezier de degree 2 */
Point calcul_ct_bezier2(Bezier2 b2, double t)
{
    Point A;
    double x, y;
    x = ((1 - t) * (1 - t) * (b2.A.x)) + (2 * t * (1 - t) * (b2.B.x)) + (t * t * (b2.C.x));
    y = ((1 - t) * (1 - t) * (b2.A.y)) + (2 * t * (1 - t) * (b2.B.y)) + (t * t * (b2.C.y));
    A = set_point(x, y);
    return A;
}

/* Calcul le point d'une courbe de bezier de degree 3 */
Point calcul_ct_bezier3(Bezier3 b3, double t)
{
    Point A;
    double x, y;
    x = ((1 - t) * (1 - t) * (1 - t) * (b3.A.x)) + (3 * t * (1 - t) * (1 - t) * (b3.B.x)) + (3 * t * t * (1 - t) * (b3.C.x)) + (t * t * t * (b3.D.x));
    y = ((1 - t) * (1 - t) * (1 - t) * (b3.A.y)) + (3 * t * (1 - t) * (1 - t) * (b3.B.y)) + (3 * t * t * (1 - t) * (b3.C.y)) + (t * t * t * (b3.D.y));
    A = set_point(x, y);
    return A;
}

/* Convertion de la courbe bezier de degree 2 en degree 3 pour la redaction du postscript  */
Bezier3 conversion_bezier2_to_bezier3(Bezier2 b2)
{
    Bezier3 b3;
    // Point C0
    b3.A = b2.A;

    // Point C3
    b3.D = b2.C;

    double x1, y1, x2, y2;
    // Point C1
    x1 = b2.A.x;
    y1 = b2.A.y;

    x2 = b2.B.x;
    y2 = b2.B.y;

    Point total;
    total = set_point((x1 + (2 * x2)) / 3, (y1 + (2 * y2)) / 3);
    b3.B = total;

    // Point C2
    x1 = b2.B.x;
    y1 = b2.B.y;

    x2 = b2.C.x;
    y2 = b2.C.y;

    total = set_point(((2 * x1) + x2) / 3, ((2 * y1) + y2) / 3);
    b3.C = total;

    return b3;
}

// Tache 7.1
/* Approximation d'un contour vers une courbe de besier 2*/
Bezier2 approx_bezier2(Contour c, int j1, int j2)
{
    Bezier2 b2;
    int n = j2 - j1;

    Tableau_Point T = sequence_points_liste_vers_tableau(c);
    Point C0, C2;
    C0 = T.tab[j1];
    C2 = T.tab[j2];

    if (n == 1)
    {
        Point C1;
        C1 = set_point((C0.x + C2.x) / 2, (C0.y + C2.y) / 2);

        // Declaration de la courbe bezier
        b2.A = C0;
        b2.B = C1;
        b2.C = C2;
        return b2;
    }
    else if (n >= 2)
    {
        double n_double;
        n_double = (double)(n);

        // Calcul a et b
        double a, b;
        a = (3 * n_double) / ((n_double * n_double) - 1);
        b = ((1 - (2 * n_double)) / (2 * (n_double + 1)));

        double x = 0;
        double y = 0;
        Point id;
        for (int i = j1 + 1; i < j2; i++)
        {
            id = T.tab[i];
            x = x + id.x;
            y = y + id.y;
        }

        // Transformner x et y en double
        double res_x, res_y;

        res_x = a * x + b * ((double)(C0.x) + (double)(C2.x));
        res_y = a * y + b * ((double)(C0.y) + (double)(C2.y));

        Point C1;
        C1 = set_point(res_x, res_y);

        //Enregistre des points vers la courbe bezier b2
        b2.A = C0;
        b2.B = C1;
        b2.C = C2;
        return b2;
    }
    else
    {
        //Enregistre des points vers la courbe bezier b2
        b2.A = set_point(-1, -1);
        b2.B = set_point(-1, -1);
        b2.C = set_point(-1, -1);
        printf("Error witht the approximation to courbe Bezier2");
        return b2;
    }
}

/* Calcul de la distance entre un point et une coube bezier de degree 2 */
double distance_point_bezier2(Point P1, Bezier2 b2, double ti)
{
    double result;
    Point A;

    A = calcul_ct_bezier2(b2, ti);
    result = distance(P1, A);
    return result;
}

/* Simpliifcation en utilisant la methode de coubes de bezier de degree 2*/
Contour simplification_douglas_peucker_bezier2(Contour C, int j1, int j2, double d)
{
    int n = j2 - j1;

    // Creation de la courbe de Bezier
    Bezier2 b2;
    b2 = approx_bezier2(C, j1, j2);

    Tableau_Point T = sequence_points_liste_vers_tableau(C);

    // Variable initialisations
    double distance, ti;
    double max_distance = 0; // dmax
    int far_away, j;

    //Trouver le point le plus loin
    for (int i = j1 + 1; i < j2; i++)
    {
        j = i - j1;
        ti = (double)(j) / (double)(n);
        distance = distance_point_bezier2(T.tab[i], b2, ti);
        if (max_distance < distance)
        {
            max_distance = distance;
            far_away = i;
        }
    }

    if (max_distance <= d)
    {
        Contour L;
        L = creer_liste_Point_vide();
        //Enregistre des points trouve sur la liste des points (contour)
        ajouter_element_liste_Point(&L, b2.A);
        ajouter_element_liste_Point(&L, b2.B);
        ajouter_element_liste_Point(&L, b2.C);
        return L;
    }
    else
    {
        //Recursivite appliqué
        Contour L1;
        L1 = creer_liste_Point_vide();
        L1 = simplification_douglas_peucker_bezier2(C, j1, far_away, d);

        Contour L2;
        L2 = creer_liste_Point_vide();
        L2 = simplification_douglas_peucker_bezier2(C, far_away, j2, d);

        return concatener_liste_Point(L1, L2);
    }
}
/* Creation du fichier .eps pour la methode de simplification des courbes de bezier de degree 2 */
void create_postscript_contours_bezier2(Liste_Contours c, char *file_name, int hauteur, int largeur) // Mode remplisage uniquement
{
    // Extension managment
    char *no_extension = strtok(file_name, ".");
    char *with_extension = malloc(strlen(no_extension) + 4);
    strcpy(with_extension, no_extension);
    strcat(with_extension, ".eps"); // concantenation

    FILE *fptr;
    fptr = fopen(with_extension, "w");
    if (fptr == NULL)
    {
        printf("EPS File Error!");
        exit(1);
    }

    fprintf(fptr, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(fptr, "%%%%BoundingBox:   %d   %d   %d   %d\n", 0, 0, largeur, hauteur);
    fprintf(fptr, "\n");
    Cellule_Liste_Contours *al;
    al = c.first;
    //Parcours de contous dans une liste des contours
    while (al != NULL)
    {
        Cellule_Liste_Point *el;
        el = (al->data).first;
        Bezier3 b3;
        Bezier2 b2;
        //Initialisation des points de la courbe b2
        b2.A = el->data;
        el = el->suiv;
        b2.B = el->data;
        el = el->suiv;
        b2.C = el->data;
        b3 = conversion_bezier2_to_bezier3(b2);
        //Initialisation de la redaction du fichier eps
        fprintf(fptr, "%.3f %.3f moveto ", b3.A.x, hauteur - b3.A.y);
        fprintf(fptr, "%.3f %.3f %.3f %.3f %.3f %.3f curveto ", b3.B.x, hauteur - b3.B.y, b3.C.x, hauteur - b3.C.y, b3.D.x, hauteur - b3.D.y);
        el = el->suiv;
        //Parcours des points du contour en question
        while (el != NULL)
        {
            b2.A = el->data;
            el = el->suiv;
            b2.B = el->data;
            el = el->suiv;
            b2.C = el->data;
            b3 = conversion_bezier2_to_bezier3(b2);
            //On a besoin que les points B, C, D selon la documentation des fichiers eps quand c'est curveto qui est utilisé
            fprintf(fptr, "%.3f %.3f %.3f %.3f %.3f %.3f curveto ", b3.B.x, hauteur - b3.B.y, b3.C.x, hauteur - b3.C.y, b3.D.x, hauteur - b3.D.y);
            el = el->suiv;
        }
        fprintf(fptr, "\n 2.0 setlinewidth");
        fprintf(fptr, "\n");
        al = al->suiv;
    }
    fprintf(fptr, "fill\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "showpage\n");
    fclose(fptr);
    return;
}

/* Approximation d'un contour vers une courbe bezier de degree 3*/
Bezier3 approx_bezier3(Contour c, int j1, int j2)
{
    Bezier3 b3;
    int n = j2 - j1;

    Tableau_Point T = sequence_points_liste_vers_tableau(c);
    Point C0, C3;
    C0 = T.tab[j1];
    C3 = T.tab[j2];

    if (n == 1)
    {
        Point C1, C2;
        C1 = set_point((2 * C0.x + C3.x) / 3, (2 * C0.y + C3.y) / 3);
        C2 = set_point((C0.x + 2 * C3.x) / 3, (C0.y + 2 * C3.y) / 3);
        // Declaration de la courbe bezier
        b3.A = C0;
        b3.B = C1;
        b3.C = C2;
        b3.D = C3;
        return b3;
    }
    else if (n == 2)
    {
        Point C1, C2, P1;
        P1 = T.tab[j1 + 1];
        C1 = set_point((4 * P1.x - C3.x) / 3, (4 * P1.y - C3.y) / 3);
        C2 = set_point((4 * P1.x - C0.x) / 3, (4 * P1.y - C0.y) / 3);
        // Declaration de la courbe bezier de dgeree 3 (4 points)
        b3.A = C0;
        b3.B = C1;
        b3.C = C2;
        b3.D = C3;
        return b3;
    }
    else if (n > 2)
    {
        double n_double;
        n_double = (double)(n);

        // Calcul a et b
        double a, b, lambda;
        a = (-15 * n_double * n_double * n_double + 5 * n_double * n_double + 2 * n_double + 4) / (3 * (n_double + 2) * (3 * n_double * n_double + 1));
        b = ((10 * n_double * n_double * n_double - 15 * n_double * n_double + n_double + 2) / (3 * (n_double + 2) * (3 * n_double * n_double + 1)));
        lambda = (70 * n_double) / (3 * (n_double * n_double - 1) * (n_double * n_double - 4) * (3 * n_double * n_double + 1));
        // définir la fonction alpha(i) a faire
        double x = 0.0;
        double y = 0.0;
        Point id;
        double i_dbl, alpha;
        //Declaration de la fonction φ(χ) intermidiaire
        for (int i = 1; i < n; i++)
        {
            i_dbl = (double)(i);
            alpha = (6 * i_dbl * i_dbl * i_dbl * i_dbl) - (8 * n_double * i_dbl * i_dbl * i_dbl) + (6 * i_dbl * i_dbl) - (4 * n_double * i_dbl) + (n_double * n_double * n_double * n_double) - (n_double * n_double);
            
            id = T.tab[j1 + i];
            x = x + alpha * (id.x);
            y = y + alpha * (id.y);
        }
        double res_x, res_y;
        res_x = a * ((double)C0.x) + lambda * x + b * (double)(C3.x);
        res_y = a * ((double)C0.y) + lambda * y + b * (double)(C3.y);

        Point C1, C2;
        C1 = set_point(res_x, res_y);
        x = 0;
        y = 0;
        for (int i = 1; i < n; i++)
        {
            i_dbl = n_double - (double)(i);
            alpha = (6 * i_dbl * i_dbl * i_dbl * i_dbl) - (8 * n_double * i_dbl * i_dbl * i_dbl) + (6 * i_dbl * i_dbl) - (4 * n_double * i_dbl) + (n_double * n_double * n_double * n_double) - (n_double * n_double);
            id = T.tab[j1 + i];
            x = x + alpha * ((double)id.x);
            y = y + alpha * ((double)id.y);
        }
        res_x = b * ((double)C0.x) + lambda * x + a * (double)(C3.x);
        res_y = b * ((double)C0.y) + lambda * y + a * (double)(C3.y);
        C2 = set_point(res_x, res_y);

        //Declaration de la courble de bezier de dgeree 3 (4 points)
        b3.A = C0;
        b3.B = C1;
        b3.C = C2;
        b3.D = C3;
        return b3;
    }
    else
    {
        //Traitment des cas d'erreur
        //ON TOMBE JAMAIS DANS CET PARTIE DU CODE, MAIS SI ON EST TOMBÉ
        //ON PEUT DETECTER PLUS FACILEMENT L'ERREUR
        b3.A = set_point(-1, -1);
        b3.B = set_point(-1, -1);
        b3.C = set_point(-1, -1);
        b3.D = set_point(-1, -1);
        printf("Error with the approximation to courbe Bezier3");
        return b3;
    }
}

/* Calcul de la distance d'un point par la courbe de bezier de degree 3*/
double distance_point_bezier3(Point P1, Bezier3 b3, double ti)
{
    double result;
    Point A;

    A = calcul_ct_bezier3(b3, ti);
    result = distance(P1, A);
    return result;
}

/* Simplifcation direct par approximation des contours vers courbes de bezier de dgree 3*/
Contour simplification_douglas_peucker_bezier3(Contour C, int j1, int j2, double d)
{
    int n = j2 - j1;

    // Creation de la courbe de Bezier
    Bezier3 b3;
    b3 = approx_bezier3(C, j1, j2);

    Tableau_Point T = sequence_points_liste_vers_tableau(C);

    // Variables initialisations
    double distance, ti;
    double max_distance = 0; // dmax
    int far_away, j;

    for (int i = j1 + 1; i < j2; i++)
    {
        j = i - j1;
        ti = (double)(j) / (double)(n);
        distance = distance_point_bezier3(T.tab[i], b3, ti);
        if (max_distance < distance)
        {
            max_distance = distance;
            far_away = i;
        }
    }
    //Application de l'agorithm de Douglas
    if (max_distance <= d)
    {
        Contour L;
        L = creer_liste_Point_vide();
        ajouter_element_liste_Point(&L, b3.A);
        ajouter_element_liste_Point(&L, b3.B);
        ajouter_element_liste_Point(&L, b3.C);
        ajouter_element_liste_Point(&L, b3.D);
        return L;
    }
    else
    {

        //Recursivite appliqué
        Contour L1;
        L1 = creer_liste_Point_vide();
        L1 = simplification_douglas_peucker_bezier3(C, j1, far_away, d);

        Contour L2;
        L2 = creer_liste_Point_vide();
        L2 = simplification_douglas_peucker_bezier3(C, far_away, j2, d);

        return concatener_liste_Point(L1, L2);
    }
}

/* Creation du fichier EPS pour liste des contours simplifie par la methode des courbes de bezier de degree 3 */
void create_postscript_contours_bezier3(Liste_Contours c, char *file_name, int hauteur, int largeur) // Mode remplisage uniquement
{
    // Extension managment
    char *no_extension = strtok(file_name, ".");
    char *with_extension = malloc(strlen(no_extension) + 4);
    strcpy(with_extension, no_extension);
    strcat(with_extension, ".eps"); // concantenation

    //File access management
    FILE *fptr;
    fptr = fopen(with_extension, "w");
    if (fptr == NULL)
    {
        printf("EPS File Error!");
        exit(1);
    }

    //File introductions written
    fprintf(fptr, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(fptr, "%%%%BoundingBox:   %d   %d   %d   %d\n", 0, 0, largeur, hauteur);
    fprintf(fptr, "\n");
    Cellule_Liste_Contours *al;
    al = c.first;
    while (al != NULL)
    {
        Cellule_Liste_Point *el;
        el = (al->data).first;
        Bezier3 b3;
        //Passing points to the bezier curve
        b3.A = el->data;
        el = el->suiv;
        b3.B = el->data;
        el = el->suiv;
        b3.C = el->data;
        el = el->suiv;
        b3.D = el->data;
        //moveto is used only once
        fprintf(fptr, "%.3f %.3f moveto ", b3.A.x, hauteur - b3.A.y);
        //this is why we are adding only B, C, D points for curveto synatx
        fprintf(fptr, "%.3f %.3f %.3f %.3f %.3f %.3f curveto ", b3.B.x, hauteur - b3.B.y, b3.C.x, hauteur - b3.C.y, b3.D.x, hauteur - b3.D.y);
        el = el->suiv;
        //Repeat for every curve on the specific contour
        while (el != NULL)
        {
            b3.A = el->data;
            el = el->suiv;
            b3.B = el->data;
            el = el->suiv;
            b3.C = el->data;
            el = el->suiv;
            b3.D = el->data;
            fprintf(fptr, "%.3f %.3f %.3f %.3f %.3f %.3f curveto ", b3.B.x, hauteur - b3.B.y, b3.C.x, hauteur - b3.C.y, b3.D.x, hauteur - b3.D.y);
            el = el->suiv;
        }
        fprintf(fptr, "\n 2.0 setlinewidth");
        fprintf(fptr, "\n");
        al = al->suiv;
    }
    //Wrtting file mode
    fprintf(fptr, "fill\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "showpage\n");
    fclose(fptr);
    return;
}