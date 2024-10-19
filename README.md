# PeiP-S4-MAP401_Project

## Description

This project is for the course MAP401 (DLST). The explication of the project can be found at Poly-cours-td.pdf and the questions for every Task1-8 are explained in detailed on the file Poly-tp.pdf. In general it is all about vectorisation and simplification of BITMAP images.

This work is licensed under the MIT license

## Notes

This library isn't in synchromisation with Turing. It requires an SFTP connection via the Cyberduck software. You can work locally on your machine and overwrite the files later on DLST files server via the SFTP connection.

Below you can find the releases of the code as well as a callendar of the latest synchronisations with the server at the university.

## Changelog

* V1.1:24/01/2023 : Files for Tache 1 were installed on Turing and all the uncompleted functiomns were completed and tested.
* V1.2:24/01/2023 : Files for Tache 2 were installed and completed all the types required from the functions
* V1.3:24/01/2023 : Files for Tache 3 were installed
* V2.1:24/01/2023 : Provided software and code from previous projects for the implementaion of the robot. Fixed any undefined parameters and variables from the code that was transcoded into this project
* V2.2:25/01/2023 : Redefined the sequnces_functions.c files provided from the chef of project and completed any linking that was required for the file to work with th rest of files
* V2.3:25/01/2023 : Completed the algorythm and fixed minor bugs at contour.c
* V2.3-2:25/01/2023 : Project was divided into new subfolders for better clarification. Any non important files were deleted and published this version on GitHub repositry.
* V3.1:27/01/2023 : Fixed an issue on the algorithm that fins the starting point (it was checikng for a free pixel on right of every specific pixel and not to the north). More functionality added as well as follows:
  * Verfication of number of arguments was added
  * Fixed minor bugs on geom2d.c
  * Updated files verifications on Tache1, Tache2 and Tache3 folders
* V3.2:27/01/2023 : Devided Tache3 on contour.c sections according to the requirements of the compte-rendu. The code for Part 1 is as a comment "//" and the rest is the functional code for the Part 2.
* V3:2.1:30:01:2023 : Corrections according to the message "Il ne faut pas utiliser directement les champs de la structure Image, mais il faut utiliser les fonctions largeur_image, hauteur_image, get_pixel_image, set_pixel_image, etc ..." for Tache 1 and Tache 3 file image.c
* V3:2.2:30:01:2023 : Automating the test for contour.c
* V4.1:30:01:2023 : Tache 4 has been completed and all the images are now created by using the test test_postscript. In this version there was also changes to the function algo_contour . Now this function only takes an image and the file_name to print out the countour. On the same time it returns the contour. It's this function that will be changed on Tache 5 in order to write multiple contours in one file (a variable will control it)
* V4.2: 31:01:2023 : Correction de la fonction de creation des images EPS pour qu'il ne sont pas affiché à l'invers
* V5.1:31:01:2023 : Commence le Tache 5. Il ya des correctiosn qu'on doit faire avec le while dans la fonction algo_contours
* V5.2:31:01:2023 : Part 1 of Task 5 has been completed and any errors has been fixed. The program was remodified to better serve and accelerate the algorithms.
* V5.3:31:01:2023 : All the task file has been compleetd. You can find the files that were asked to be created in the corresponding folder of Tache5
* V5.4:06/02/2023 : Correction par apport l'email de 6 fevrier 2023
* V6 : 21/2/2023: Tache 6 completed and added for tests on Compte Rendu Tache 6.1
* V6.3: 28/2/2023: Correction of affichage for the number of segments for Tache6.2
* V7: 28/2/2023: Started Tache7 - Partie 1
* V7.1 : 28/2/2023: Completed Tache7 - Partie 1 (fixed errors with file EPS and tested the exported images)
* V7.2: 28/2/2023: Corrections on Tache 6 partie 2 segments number and modifications and updates for Tache 7 partie 1
* update contours.c (tache 7): 2/3/2023: Corrections to the function calcul_ct_bezier2/3 from int to double for x and y
* V7.4 : 21/03/2023: Finished Tache 7.1
* V7.5: 21/03/2023: Debut Tache 7.2
* V7.6: 21/03/2023: Correctioons on Tache7.2 - verfied function approx_bezier3
* V7.7: 27/03/2023: Tache 7.2 completed
* V8.1: 28/03/2023: Tache 8 started - remaining comments, README tests verification, as well as eps files creation with the rest of the files Asterix3, lettre-L-Cursive and ColombesDeLaPaix pour les methodes degree2 et degree3. A completion of the word file for Tache 8 Partie 2 is required (trasnfrom eps to png and complete the table for ColombesDeLaPaix pour la methode de simplification).
* V8.2: 28/03/2023: Bug fices on contour.c and automatisation of testing process for test_degree2-tache8, test_degree3-tache8 and test_simplification-tache8
* V8.3: 28/03/2023: Completion of comments for Tache 8
* V8.4 : Bug fixes for Tache 8
* V9: Source code, finalisation and bug fixes
* V9.1: Comments added on the source code. It remains to create the Final Rapport
* V10.1: Avant termination du Rapport Final du projet
* V10.2 : Completion du rapport final
* V10.3 : Presentation finis

## Sync

1. 24/01/2023: Turing -> PC (V2.1)
2. 27/01/2023: PC (V3.1) -> Turing (Links for Tache 1 and 2 sent to prof)
3. 31/01/2023: PC (V4.2) -> Turing (Links for Tache 3 and 4 sent to prof)
4. 31/01/2023: PC (V5.3) -> Turing
5. 21/02/2023: Turing -> PC (Images)
6. 21/02/2023: PC(V6) -> Turing (Link for Tache 6 sent to prof)
7. 28/02/2023; PC(V7.2) -> Turing (Link for Tache 7 partie 1 sent to prof in order to check out what we have done for the calculation of the distance of a point from a point to the point of a courbe bezier)
8. 21/03/2023: PC(7.4) -> Turing (Tache 7.1)
9. 27/03/2023: PC(7.7) -> Turing (Tache 7.2)
10. 4/42023: PC(9) -> Turing (S14 + source code)

## Tests Programmes

* Tache1: test_image
* Tache2: test_geom
* Tache3: test_contour
* Tache4: test_postscript
* Tache5: test_mask
* Tache6: test_simplification
* Tache7.1: test_approx & test_degree2
* Tache7.2: test_approx3 & test_degree3
* Tache8.1: test_simplification-courbe_hilbert_7, test_simplification-courbe_hilbert_8, test_simplification-courbe_hilbert_9, test_simplification-courbe_hilbert_10, test_simplification-zebres-1000x0750, test_simplification-zebres-2000x1500, test_simplification-zebres-3000x2250, test_simplification-zebres-4000x3750(Tache 6 folder)
* Tache8.2: test_degree2-tache8, test_degree3-tache8 & test_simplification-tache8 (Tache 7 folder)

---
**PeiP-S4-Vectorisation_and_simplification_of_BITMAP** © 2023 by **Vasileios Filippos Skarelas** and **Maxime Gros** is licensed under the [Creative Commons Attribution-NonCommercial 4.0 International](https://creativecommons.org/licenses/by-nc/4.0/).
