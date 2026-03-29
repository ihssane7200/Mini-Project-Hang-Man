# Guide d'Explication et de Soutenance : Mini-Projet Jeu du Pendu en C

Ce guide est un manuel ultra-détaillé conçu pour vous préparer à toute question de votre professeur. Il explique chaque concept de fond en comble, le "pourquoi" de chaque chose, la gestion de la mémoire, et fournit une large banque de questions/réponses.

---

## 1. Explication de l'Environnement et de l'IDE (Setup)

Pour développer en langage C, nous avons besoin de deux choses principales :
1. **Un Éditeur de texte (IDE - Integrated Development Environment) :** Comme *Visual Studio Code (VS Code)*. C'est votre "cahier de brouillon" intelligent. Il colore le code pour le rendre lisible et souligne vos erreurs de syntaxe.
2. **Un Compilateur :** Comme *GCC* (via MinGW sur Windows). L'ordinateur ne comprend pas le langage C, il ne comprend que le binaire (des `0` et des `1`). Le compilateur est le "traducteur" qui prend votre texte (code source) et le transforme en un fichier exécutable (.exe) compréhensible par la machine.

**Pourquoi un `Makefile` ?**
Imaginez que vous deviez taper `gcc src/main.c src/msvcrt_compat.c -o hangman.exe -I./raylib/include -L./raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm` à chaque fois que vous modifiez une ligne de code. C'est long et source d'erreurs. Le `Makefile` est un fichier de recette. Il dit à l'ordinateur : "Quand je tape 'make' ou que je lance la tâche, utilise cette longue commande pour tout assembler automatiquement".

---

## 2. Explication de tous les Fichiers du Projet

* **`src/main.c`** : C'est le cœur de votre programme. Il contient la logique (les règles du jeu), l'affichage, et l'entrée principale (`main()`).
* **`src/msvcrt_compat.c`** : Un fichier technique. Parfois, les versions de Windows ont des problèmes avec certaines fonctions de base du C (comme celles utilisées pour afficher du texte). Ce fichier est un "pansement" pour s'assurer que ça marche partout.
* **`raylib/` et `raylib-5.0/`** : Ce sont les bibliothèques. Plutôt que de réinventer comment dessiner un pixel sur un écran ou comment lire une pression sur une touche (ce qui prendrait des milliers de lignes très complexes), on utilise le code de quelqu'un d'autre qui a déjà fait ce travail.
* **`setup.bat`** : Un script Windows qui prépare le terrain (téléchargement/décompression de fichiers utiles au besoin).
* **`README.md`** : La vitrine de votre projet sur GitHub. Il explique à n'importe quel visiteur à quoi sert ce projet, comment l'installer et comment y jouer. *L'extension .md veut dire "Markdown", un format pour mettre en gras, créer des listes et des titres facilement.*
* **`*.h` (Fichiers Header)** : Un fichier d'en-tête (comme `raylib.h`). Imaginez que c'est le "sommaire" ou le "menu" d'un restaurant. Il liste toutes les fonctions existantes (ex: `DrawText`) sans contenir la recette (le code exact de la fonction). Cela permet à votre fichier `.c` de savoir que ces fonctions existent.

---

## 3. Le Langage C de A à Z (Niveau Débutant, Explications Détaillées)

Le C est un langage **impératif et procédural**. L'ordinateur lit votre code de haut en bas, ligne par ligne, strictement dans l'ordre que vous avez écrit.

### 3.1 La syntaxe de base (Les symboles)
* **Les instructions se terminent par un point-virgule `;`** : C'est comme le point à la fin d'une phrase. Si vous l'oubliez, l'ordinateur lira la ligne suivante comme formant une seule phrase avec la précédente, et il ne comprendra plus rien (Erreur Syntaxique).
* **Les accolades `{ }`** : Elles définissent un "bloc" de code. Tout ce qui est entre `{` et `}` est un groupe. Par exemple, le contenu complet d'une fonction ou les actions d'une boucle doivent être contenus entre accolades.
* **`#include <stdio.h>`** : Le `#` veut dire que c'est une "directive de préprocesseur". Avant même de traduire le code, l'ordinateur va chercher le fichier `stdio.h` (qui contient les fonctions standard d'Input/Output, comme `printf`) et l'inclure dans votre programme. Sans cela, vous n'avez pas d'outils de base.

### 3.2 Les Variables (Les Boîtes)
Pourquoi on les utilise ? Pour mémoriser des choses pendant que le programme tourne. 
* **`int` (Integer)** : Pour les nombres entiers (ex: `int essais = 6;`).
* **`char` (Character)** : Pour stocker UNE SEULE lettre (ex: `char lettre = 'A';`).
* **`bool` (Boolean)** : Pour stocker une réponse "Vrai" ou "Faux" (`true` ou `false`). Très utile pour savoir si la partie est terminée (`bool gameWon = false;`).
* **`float` / `double`** : Pour les nombres à virgule.

### 3.3 Les Tableaux (Arrays) et Chaînes de caractères (Strings)
* **Tableau (`int notes[5]`)** : Au lieu de créer 5 variables différentes (`note1`, `note2`, etc.), on crée une seule armoire à 5 tiroirs.
* **Les mots en C (`char mot[] = "Salut";`)** : Le langage C n'a pas de vrai type "Texte". Pour lui, un texte est juste une suite (un tableau) de lettres (`char`). 
* **La règle du `\0` (Caractère nul)** : Comment l'ordinateur sait-il qu'un mot est terminé ? Il ajoute toujours un caractère invisible appelé le terminateur (`\0`) à la fin. "CHAT" prend en fait 5 cases : `['C', 'H', 'A', 'T', '\0']`.

### 3.4 Les Conditions (La Prise de Décision)
Pourquoi on les utilise ? Parce qu'un bon programme ne fait pas toujours la même chose. Il réagit (au clavier, aux erreurs).
* **`if` (Si)** : "Si la lettre tapée est bonne, ajoute-la au mot."
* **`else if` / `else` (Sinon si / Sinon)** : "Sinon, diminue le nombre de vies."
* **`switch / case`** : C'est une alternative au "if" quand on a beaucoup de possibilités précises à vérifier (ex: un menu avec Bouton 1, Bouton 2, Bouton 3).

### 3.5 Les Boucles (Répétition Automatique)
Pourquoi on les utilise ? Pour ne pas écrire 1000 fois la même ligne, ou pour faire tourner le jeu en continu.
* **`while (...)` (Tant que)** : Répète une action TANT QU'une condition est vraie. *Ex : `while (!WindowShouldClose())` -> Tant qu'on n'a pas cliqué sur la croix rouge de la fenêtre, continue de dessiner le jeu 60 fois par seconde.* C'est ce qu'on appelle la **Game Loop**.
* **`for (...)` (Pour)** : Pensez-y comme une boucle pour COMPTER. "Pour i allant de 0 à la longueur du mot, vérifie chaque lettre".

### 3.6 Les Fonctions
Pourquoi on les utilise ? Pour ne pas avoir un fichier `.c` de 5000 lignes impossible à lire. On regroupe un ensemble logique d'instructions (ex: `StartSession()`, `DrawModernButton()`) auquel on donne un nom. Ensuite, il suffit d'appeler ce nom. Ça rend le code lisible et réutilisable.

### 3.7 Les Structures (`struct`)
Notre `struct GameState` est comme la création d'un NOUVEAU type de donnée personnalisé. C'est créer un "Sac à dos" global qui contient toutes les poches nécessaires : Le mot à trouver (char*), les essais restants (int), l'état du jeu (bool). Au lieu de trimballer 10 variables séparées dans toutes nos fonctions, on ne passe que le sac à dos `GameState`.

---

## 4. Gestion de la Mémoire (Stack vs Heap) et Pointeurs

C'est LE sujet principal en C, et c'est ce qui le différencie de langages plus simples comme Python ou Java.

### La Mémoire "Stack" (La pile - Automatique)
Quand vous créez une variable normale `int a = 5;` dans une fonction, elle va dans la mémoire "Stack". C'est petit, rapide, et surtout : **elle est détruite automatiquement** dès que la fonction se termine.

### La Mémoire "Heap" (Le tas - Manuelle)
Quand on a besoin de stocker de gros éléments (comme les informations globales du jeu) qui doivent survivre même quand la fonction d'initialisation se termine, on utilise le "Heap". Mais dans le Heap, c'est **vous** le patron : l'ordinateur ne supprime jamais rien tout seul.

### Les Pointeurs (`*`)
Puisque la variable est dans le "Heap", pour la retrouver depuis n'importe où, il nous faut son "adresse postale" dans la RAM (ex: emplacement `0x7A4F2`). Un pointeur n'est rien d'autre que ça : une variable spéciale qui stocke une **adresse mémoire** plutôt qu'une valeur.
Exemple : `GameState* game` -> "game" pointe vers l'adresse d'un espace contenant notre partie.

### Les Outils de la Mémoire Manuelle
1. **`malloc` (Memory Allocation) :**
   * *Rôle :* Allouer de l'espace. On dit : "Donne-moi X octets de mémoire dans le Heap".
   * *Problème :* Il vous donne le bloc mais ne le nettoie pas. S'il y avait des résidus d'anciens programmes, ils y sont encore ("garbage memory").
2. **`calloc` (Contiguous Allocation) :**
   * *Rôle :* Fait la même chose que malloc, mais il passe un "coup de balai" et met toutes les cases à ZÉRO.
   * *Où l'a-t-on utilisé ?* Pour le `guessedWord` et `guessedLetters`. On veut être absolument sûr que ces tableaux sont vides de tout résidu avant de commencer à compter les fautes.
3. **`realloc` (Re-Allocation) :**
   * *Rôle :* Vous avez demandé 10 cases mémoires, mais finalement il vous en faut 20 (Imaginez un jeu où vous rajoutez des ennemis à l'infini). L'ordinateur ne peut pas juste ajouter 10 cases à côté (la place pourrait être occupée par le navigateur web). `realloc` prend votre bloc, trouve un endroit plus grand où il y a 20 cases de libres, copie tout dans le nouvel endroit, et libère l'ancien.
   * *Exemple :* `ptr = realloc(ptr, nouvelle_taille);`
4. **`free` (La libération) :**
   * Le passage obligatoire ! Tout bloc demandé par `malloc`, `calloc` ou `realloc` DOIT être rendu au système avec `free(pointeur)` avant de fermer le programme. Si vous ne le faites pas, ça crée une **Fuite de Mémoire (Memory Leak)**. Votre jeu consommera de la RAM à l'infini jusqu'à faire planter l'ordinateur.

---

## 5. Logique Algorithmique du Pendu (L'algorithme étape par étape)

1. **Le Tirage au sort :** `srand(time(NULL))` génère une "graine" aléatoire basée sur l'heure de votre ordinateur. Ensuite, `rand() % 8` choisit un chiffre entre 0 et 7 pour sélectionner un mot au hasard dans nos tableaux.
2. **La Saisie Utilisateur :** La boucle lit le clavier (`GetCharPressed()`).
3. **Le Traitement (La Logique) :**
   * Est-ce que cette lettre fait partie du mot secret ?
   * Si **OUI** : On parcourt tout le mot secret, et on remplace les "_" dans l'affichage par la lettre tapée.
   * Si **NON** : On ajoute +1 à la variable "fautes / essais" (`game->tries++`).
4. **Les Conditions de Victoire :**
   * On compare le mot que l'utilisateur a rempli avec le mot secret (avec `strcmp`). S'ils sont identiques (résultat = 0), `gameWon = true`.
   * Si les "essais" atteignent la constante globale `MAX_TRIES` (6), le bonhomme est entièrement dessiné, `gameOver = true`.

---

## 6. Raylib vs GTK

* **Raylib (Temps Réel / Multimédia) :** Raylib tourne en boucle infinie (environ 60 ou 144 "tours" par seconde). À chaque tour, on efface l'écran et on redessine tout à zéro (le texte, le pendu, les boutons). C'est parfait pour un jeu vidéo pour animer des éléments et capter le clavier à la vitesse de la lumière. Elle accède directement à l'accélération matérielle (OpenGL).
* **GTK (Logiciels / Interfaces Événementielles) :** GTK (GIMP Toolkit) sert à dessiner des fenêtres "normales" (comme Word, Excel ou votre explorateur). Elle ne s'actualise pas 60 fois par seconde. Elle "dort" jusqu'à ce que vous cliquiez sur un bouton (on appelle ça de la programmation pilotée par l'événement "Event-Driven"). Pour un pendu en console ou en "logiciel", GTK est possible mais pour du dessin dynamique et fun, Raylib est 100x mieux adapté.

---

## 7. Préparation à la Soutenance : +25 Questions et Réponses Possibles

### Niveau Facile (Bases du C)
1. **Que fait `#include` ?**
   *Il importe des fonctions prédéfinies depuis d'autres fichiers/bibliothèques pour qu'on puisse les utiliser, comme inclure des outils de base dans une boîte.*
2. **C'est quoi la fonction `main()` ?**
   *C'est le point d'entrée unique de mon programme. Sans elle, Windows ne saurait pas par où commencer.*
3. **À quoi servent les accolades `{}` ?**
   *Elles délimitent un bloc de code. Elles disent au compilateur que toutes ces lignes appartiennent au même `if`, à la même fonction ou à la même boucle.*
4. **Pourquoi as-tu utilisé `int` pour les "tries" (essais) au lieu de `float` ?**
   *Parce que le nombre d'essais est obligatoirement un nombre entier. On ne peut pas commettre 2.5 erreurs.*
5. **Quelle est la différence entre `=` et `==` ?**
   *Un seul `=` sert à "affecter" (donner) une valeur (ex: `vies = 5`). Le double `==` sert à COMPARER deux choses dans une condition (ex: `if (vies == 0)`).*
6. **Que signifie l'opérateur `||` et `&&` ?**
   *`||` signifie "OU" (l'une ou l'autre condition doit être vraie). `&&` signifie "ET" (les deux à la fois).*
7. **Que fait une boucle `while` ?**
   *Elle répète l'exécution d'un bloc de code tant qu'une condition reste vraie.*
8. **Pourquoi as-tu défini des constantes avec `#define MAX_TRIES 6` ?**
   *C'est plus facile à lire et à modifier. Si je veux passer à 10 essais, je change juste le haut du fichier au lieu de chercher tous les "6" éparpillés dans mon code.*

### Niveau Intermédiaire (Logique, Tableaux, C standard)
9. **Quelle est la différence entre un tableau (array) et un pointeur ?**
   *Un tableau est une réservation fixe de cases mémoires consécutives. Un pointeur est juste une variable qui stocke l'adresse d'une case mémoire (qui peut pointer vers le début d'un tableau).*
10. **Dans tes mots cachés, les lettres sont des tableaux de caractères. Que se cache-t-il à la fin de chaque mot et pourquoi ?**
    *Le caractère Nul (`\0`). Il est indispensable pour dire aux fonctions comme `printf` ou `strcmp` : "Arrête-toi, le texte est fini ici".*
11. **Que fait la fonction `strcmp` ?**
    *(String Compare). Elle prend deux chaînes de caractères et les compare. Si elles sont identiques, elle renvoie 0.*
12. **Comment l'ordinateur fait-il du hasard ? Que fait `srand(time(NULL))` ?**
    *L'ordinateur n'a pas de vrai hasard, il génère des suites de nombres via des maths. `srand(time(NULL))` donne l'heure exacte en secondes (depuis 1970) comme "graine de départ" pour être sûr d'avoir un tirage "aléatoire" différent à chaque partie.*
13. **Comment passes-tu l'état du jeu (`GameState`) aux fonctions ?**
    *Je le passe par "pointeur" (`StartSession(GameState* game)`). Cela permet à la fonction de modifier le "vrai" état du jeu directement dans la mémoire, et ça m'évite de recréer une grosse copie très lourde.*
14. **Qu'est-ce qu'une Structure (`struct`) ?**
    *C'est la création d'un "type" de données composé. Cela regroupe plusieurs variables différentes (int, char, bool) de manière logique sous un même toit.*
15. **Qu'est-ce que "l'allocation dynamique" ?**
    *C'est demander au système d'exploitation de nous allouer de la mémoire dans le "Tas" (Heap) PENDANT que le programme tourne, contrairement à l'allocation statique qui est définie à la compilation.*
16. **Si j'utilise `fgets` ou `scanf("%s")`, pourquoi il y a un problème avec les espaces ou le dernier caractère ?**
    *`scanf("%s")` s'arrête au premier bloc d'espace, et ça peut laisser un retour à la ligne (`\n`) dans le "buffer" (la zone d'attente du clavier), ce qui crée des bugs dans la lecture suivante.*

### Niveau Difficile (Mémoire avancée et architecture)
17. **C'est quoi la différence profonde entre `malloc` et `calloc` ?**
    *`malloc` réserve la mémoire brut sans y toucher (plus rapide mais potentiellement sale). `calloc` prend du temps supplémentaire pour écrire des zéros dans toute la zone réservée (sécurisé pour éviter les bugs).*
18. **Explique le fonctionnement exact de `realloc` dans le système.**
    *Si je demande un `realloc(pointeur, taille_plus_grande)`, le système regarde s'il y a de la place libre juste après le bloc actuel. S'il n'y en a pas, il cherche un tout nouvel espace ailleurs, copie mon ancien bloc dans ce nouvel espace, supprime l'ancien, et me donne la nouvelle adresse.*
19. **Qu'est-ce qu'une fuite de mémoire (Memory Leak) ?**
    *C'est allouer de l'espace avec `malloc` ou `calloc`, et oublier d'utiliser `free()` avant de fermer le programme. Cette mémoire devient "inatteignable" par notre code, mais l'OS la croit utilisée, ce qui baisse la RAM disponible de l'ordinateur.*
20. **Que se passe-t-il si on utilise `free()` sur un bloc, puis qu'on cherche à y accéder à nouveau ?**
    *C'est ce qu'on appelle un **"Dangling Pointer"** (Pointeur fou/Pendant). L'accès à une zone libérée mène généralement un crash instantané du programme (Segmentation Fault ou Segfault).*
21. **Si on utilise `free(ptr)` deux fois de suite, que se passe-t-il ? (Double free)**
    *Faire un "Double Free" corrompt la gestion de la mémoire du système et provoque généralement un crash brutal de l'application (Segfault).*
22. **Quelle est la "Pile" (Stack) vs le "Tas" (Heap) ?**
    *La Pile stocke l'historique d'appels des fonctions et les variables locales, c'est très rapide mais sa taille est limitée (~1 à 8 MO) et la destruction est automatique. Le Tas est la mémoire géante (des Gigaoctets de RAM), gérée manuellement par le programmeur (`malloc`).*
23. **Pouvais-tu faire ton jeu sans pointeur pour struct ? Pourquoi t'embêter avec `malloc` ici ?**
    *Techniquement, pour un jeu si petit, un `GameState` déclaré en variable locale ou globale (statique) aurait fonctionné. Mais utiliser `malloc` montre que je maîtrise la gestion complète du cycle de vie d'un objet (Instanciation, Modification, Destruction), indispensable pour des projets plus massifs.*
24. **C'est quoi un "Segmentation Fault" ou "Violation d'accès" ?**
    *C'est l'erreur la plus commune en C. Cela arrive quand mon programme essaie d'accéder à une case mémoire (pointeur) qui ne lui appartient pas, ou qui est nulle (`NULL`), ou déjà détruite. Le système d'exploitation tue le programme par sécurité.*
25. **Dans la boucle Raylib, que font `BeginDrawing()` et `EndDrawing()` ?**
    *Dans un jeu vidéo, on est dans l'architecture matérielle de la carte graphique (appelée le Double Buffering). `BeginDrawing` prévient qu'on commence un nouveau cadre (Frame) en cache. `EndDrawing` dit à la carte graphique d'afficher d'un seul coup tout ce qu'on vient de dessiner vers l'écran de l'utilisateur. Ca évite le clignotement de l'écran.*