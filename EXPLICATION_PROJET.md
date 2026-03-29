# Guide d'Explication et de Soutenance : Mini-Projet Jeu du Pendu en C

Ce guide a pour but de vous expliquer de A à Z le fonctionnement de votre projet, les concepts clés du langage C, et de vous préparer aux questions de votre professeur.

---

## 1. Explication de l'Environnement et de l'IDE (Setup)
Pour développer en langage C, nous avons besoin de deux choses principales :
1. **Un Éditeur de texte (IDE) :** Comme *Visual Studio Code (VS Code)*, qui sert à écrire le code.
2. **Un Compilateur :** Comme *GCC* (MinGW sur Windows), qui traduit le code que nous écrivons (compréhensible par un humain) en langage machine (des 0 et des 1 que l'ordinateur peut exécuter).

**Configuration du projet :**
Le projet utilise **Raylib**, une bibliothèque qui permet de dessiner facilement des choses à l'écran, de jouer des sons et de gérer la souris/clavier. Le compilateur prend vos fichiers `.c` et les lie avec les fichiers de Raylib pour créer un fichier exécutable (`.exe`).

---

## 2. Explication de tous les Fichiers du Projet
Voici à quoi sert chaque fichier et dossier dans votre projet :

* **`src/main.c`** : C'est le cœur de votre programme. Il contient la logique du jeu du pendu, l'affichage graphique, et la fonction `main()` qui est le point d'entrée du programme (là où l'exécution commence).
* **`src/msvcrt_compat.c`** : C'est un fichier de compatibilité technique souvent utilisé sur Windows pour s'assurer que certaines fonctions standard du C (comme celles utilisées pour l'affichage ou la mémoire) fonctionnent correctement selon la version de Windows.
* **`raylib/` et `raylib-5.0/`** : Ce sont les dossiers contenant tout le code de la bibliothèque graphique "Raylib". Ils fournissent les outils pour ouvrir une fenêtre et dessiner avec (comme `DrawText`, `InitWindow`).
* **`Makefile`** : C'est un "scénario" d'assemblage. Au lieu de taper une longue ligne de commande `gcc ...` à chaque fois, le Makefile contient les instructions pour dire à l'ordinateur exactement comment compiler le jeu.
* **`setup.bat`** : C'est un script pour Windows (un fichier batch) qui automatise l'installation ou la configuration initiale (par exemple, télécharger les bonnes versions de Raylib au besoin).
* **`README.md`** : C'est le manuel d'utilisation de votre projet. C'est le premier fichier que les gens (ou votre professeur) lisent sur GitHub. Il explique comment installer et jouer au jeu.
* **`header.h` (Concept - Ex: `raylib.h`)** : Un fichier `.h` (Header) contient les "signatures" ou "sommaires" des fonctions. Il ne contient pas le code en lui-même, mais dit au programme "*Hey, ces fonctions existent, et voici comment elles s'appellent*". Cela permet de partager des fonctions entre plusieurs fichiers `.c` sans tout copier-coller.

---

## 3. Le Langage C de A à Z (Niveau Débutant)
Le C est un langage "procédural". Le code est lu et exécuté ligne par ligne, de haut en bas.
* **Variables :** Ce sont des "boîtes" dans lesquelles on stocke des données. 
  * `int tries = 0;` (une boîte nommée 'tries' qui contient un nombre entier).
  * `char letter = 'A';` (une boîte qui contient un seul caractère).
* **Structures (`struct`) :** C'est une façon de regrouper plusieurs variables ensemble. Dans votre code, `GameState` regroupe toutes les infos du jeu (le mot, la difficulté, les essais restants) dans une seule "grosse boîte".
* **Conditions (`if / else`) :** Permettent de faire des choix. *"Si (if) le joueur trouve la lettre, on la révèle, sinon (else) on perd un essai."*
* **Boucles (`while / for`) :** Permettent de répéter des actions. La boucle principale `while (!WindowShouldClose())` tourne environ 60 fois par seconde pour dessiner les images du jeu tant qu'on ne ferme pas la fenêtre.
* **Tableaux (`Arrays`) :** Une suite de boîtes du même type. Un mot comme "TEST" en C est un tableau de 5 lettres : `['T', 'E', 'S', 'T', '\0']` (le `\0` indique la fin du mot).

---

## 4. Gestion de la Mémoire et Pointeurs (Très Important !)
Votre professeur vous posera sûrement des questions là-dessus. En C, on doit gérer la mémoire vive (RAM) de l'ordinateur nous-mêmes.

**C'est quoi un pointeur ?**
Imaginez que la RAM est une grande rue avec des maisons (la mémoire). 
* Une **variable**, c'est ce qu'il y a *à l'intérieur* de la maison.
* Un **pointeur**, c'est *l'adresse* de la maison (ex: "15 rue de la Paix"). Le pointeur ne contient pas la valeur, il indique **où** elle se trouve. En C, on le note avec une étoile `*` (ex: `GameState* game`).

**Allocation Dynamique (`malloc`, `calloc`) :**
Quand vous avez écrit `InitGame()` :
```c
GameState* game = (GameState*)malloc(sizeof(GameState));
game->guessedWord = (char*)calloc(MAX_WORD_LENGTH, sizeof(char));
```
* **`malloc` (Memory Allocation) :** Demande à Windows : *"Réserve-moi un espace dans la mémoire de la taille exacte d'un `GameState`"*.
* **`calloc` (Contiguous Allocation) :** Pareil que malloc, mais il a l'avantage de mettre toutes les cases à zéro (nettoie la mémoire) pour éviter les bugs.
* **Pourquoi on fait ça ?** Au lieu de créer la mémoire temporairement à chaque frame du jeu, on crée un bloc persistant dans la mémoire que l'on manipule en lui envoyant notre "adresse" (le pointeur).
* **Règle d'or :** Tout ce qui est alloué avec `malloc`/`calloc` doit être détruit avec `free(game);` à la fin, sinon c'est ce qu'on appelle une **fuite de mémoire (memory leak)**. L'ordinateur finit par saturer sa RAM et planter.

---

## 5. Logique Algorithmique du Pendu
La logique principale repose sur de grands blocs (ou "États") :
1. **Initialisation :** Choisir un mot au hasard selon la difficulté sélectionnée. Le mot est rempli de tirets `_ _ _ _`.
2. **La Boucle de jeu (Frame Loop) :**
   * Elle vérifie si une touche du clavier est pressée.
   * Si la touche (ex: 'A') est dans le mot caché, on remplace le `_` par `A`.
   * Sinon, on augmente le compteur d'erreurs (`game->tries++`).
3. **Conditions de Fin (Game Over / Win) :**
   * Si `tries` arrive à la limite (6), c'est Perdu (`game->gameOver = true`).
   * Si le mot dévoilé est complètement égal au mot secret, c'est Gagné (`game->gameWon = true`).

---

## 6. Raylib vs GTK
* **Raylib :** C'est une bibliothèque conçue principalement pour faire des **Jeux Vidéo**. Elle dessine directement sur la "carte graphique" pixel par pixel. Elle est simple, légère, et parfaite pour dessiner des formes (des ronds, des carrés, le bonhomme du pendu) et gérer 60 images par seconde.
* **GTK :** C'est une bibliothèque conçue pour créer des **Logiciels de Bureau** (Bureautique). Avec GTK, on crée des boutons "natifs", des listes déroulantes, des menus cliquables. Ce n'est pas fait pour afficher 60 images par secondes ni pour créer de la logique de jeu fluide, c'est fait pour que l'interface ressemble aux applications système (comme Word ou le gestionnaire de fichiers).

---

## 7. Questions Possibles du Professeur et Réponses

### Niveau Facile (Niveau "Est-ce qu'il comprend son code ?")
**Q1 : Quelle est la fonction principale par laquelle le programme commence ?**
*Réponse :* C'est la fonction `main()`. C'est le point de départ de tout programme C.

**Q2 : À quoi sert l'instruction `#include "raylib.h"` au début de ton fichier ?**
*Réponse :* Elle charge l'en-tête de la bibliothèque Raylib, ce qui me donne le droit d'utiliser des fonctions de la bibliothèque comme `DrawText` ou `InitWindow`.

**Q3 : Comment sais-tu que le joueur a perdu ?**
*Réponse :* Je vérifie si la variable `game->tries` a atteint la constante `MAX_TRIES` (qui vaut 6).

### Niveau Intermédiaire (Niveau "Est-ce qu'il comprend la logique ?")
**Q1 : Pourquoi as-tu utilisé une `struct` pour l'état du jeu (`GameState`) ?**
*Réponse :* Pour regrouper toutes les informations liées au jeu (Le mot, les essais, l'état victoire/défaite) dans un seul "objet". C'est plus propre que d'avoir 10 variables éparpillées partout dans mon code. 

**Q2 : Quelle est la différence entre `malloc` et `calloc` ?**
*Réponse :* `malloc` (Memory Allocation) donne un bloc de mémoire mais ne le nettoie pas (il peut y avoir des "déchets" résiduels d'anciens programmes). `calloc` (Contiguous Allocation) donne un bloc de mémoire et initialise tout à zéro. J'utilise `calloc` pour mes lettres devinées afin d'être sûr que mon tableau est bien vide au début.

### Niveau Difficile (Niveau "Maitrise Totale")
**Q1 : Tu as alloué de la mémoire dynamiquement dans `InitGame()`, as-tu utilisé `free()` ? Que se passe-t-il si tu ne le fais pas ?**
*Réponse :* Oui, il faut libérer la mémoire à la fin du programme avec `free(game)`. Si on ne le fait pas, cela crée une "fuite de mémoire" (Memory Leak). L'ordinateur croira que cette RAM est toujours utilisée même après la fermeture du jeu.

**Q2 : Pourquoi utiliser des pointeurs (`GameState* game`) au lieu d'une simple variable locale (`GameState game`) ?**
*Réponse :* Parce qu'on veut que notre état de jeu persiste tout au long de l'éxécution du programme et qu'on veut pouvoir le modifier de n'importe où dans différentes fonctions sans avoir à le copier entièrement à chaque appel de fonction (ce qui consommerait beaucoup plus de mémoire et ralentirait le programme).

**Q3 : Pourquoi Raylib est plus adapté que GTK pour ton Pendu ?**
*Réponse :* Raylib tourne sur une "gameloop" (boucle infinie) continue, idéale pour gérer des graphismes en temps réel et des entrées clavier très rapides dans un contexte de jeu de type "canevas". GTK est "event-driven" (piloté par l'événement), ce qui veut dire qu'il ne s'actualise que quand on clique, ce qui est parfait pour un logiciel, mais très lourd et inadapté pour un jeu.