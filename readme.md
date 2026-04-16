# 🏰 Labyrinthe - Jeu d'aventure en C (ncurses)

Ce projet est un jeu d'exploration de labyrinthe en mode console, développé en C avec la bibliothèque ncurses.  
Il a été réalisé dans le cadre de ma 3ème année de Licence Informatique à l'Université Paris 8.

Le joueur incarne un chevalier qui doit naviguer à travers différentes salles, gérer son inventaire de clés et déverrouiller des passages pour trouver Azatoth.

---

## 🚀 Fonctionnalités

- Génération dynamique :  
  Le labyrinthe est chargé à partir d'un fichier de plan externe (.pln) définissant les connexions entre les salles.

- Système de clés et verrous :
  - Des clés sont placées aléatoirement dans le labyrinthe.
  - Certaines portes sont verrouillées et nécessitent l'utilisation d'une clé pour passer.

- Gestion des sauvegardes :
  - Possibilité de sauvegarder l'état de la partie (position du joueur, inventaire de clés, état des objets dans les salles).
  - Chargement de la dernière sauvegarde.

- Interface interactive :  
  Utilisation de ncurses pour une gestion fluide des déplacements au clavier et un affichage dynamique dans le terminal.

---

## 🛠️ Compilation et Lancement

### 📌 Prérequis

La bibliothèque ncurses doit être installée sur votre système.

- Sur macOS :
  brew install ncurses

- Sur Linux (Debian/Ubuntu) :
  sudo apt-get install libncurses5-dev libncursesw5-dev

---

### ⚙️ Compilation

gcc labyrinthe.c -lncurses -o labyrinthe

---

### ▶️ Lancement

./labyrinthe

Au démarrage, le programme vous demandera de saisir le nom du fichier de plan (ex: plan.pln).

---

## 🎮 Commandes de jeu

- Flèches directionnelles : Se déplacer  
  (Haut = Nord, Bas = Sud, Droite = Est, Gauche = Ouest)

- P : Sauvegarder la partie actuelle

- C : Charger la dernière sauvegarde

- Q : Quitter le jeu

---

## 📂 Structure du projet

.
├── labyrinthe.c        # Code source principal du moteur de jeu  
├── plan.pln            # Fichier de configuration du labyrinthe  
├── sauvegarde.txt      # Fichier généré automatiquement pour la sauvegarde  

---

## 📝 Format du fichier Plan (.pln)

Le plan suit une syntaxe spécifique pour relier les salles :

debutPlan  
A 0.N-1.S;   // Relie la salle 0 (porte Nord) à la salle 1 (porte Sud)  
A 1.E-2.O;   // Relie la salle 1 (porte Est) à la salle 2 (porte Ouest)  
finPlan  

### Explication :

- A : Ajout d'une connexion  
- 0.N-1.S :
  - Salle 0, sortie Nord
  - reliée à salle 1, entrée Sud

---

## 💾 Remarque

Le fichier sauvegarde.txt est généré automatiquement.  
Il est recommandé de l’ajouter dans le .gitignore.

---

## 🎯 Objectif du jeu

Explorer le labyrinthe, récupérer des clés, déverrouiller les portes et atteindre Azatoth en un minimum de déplacements.

---

## 👨‍💻 Auteur

Projet réalisé en L3 Informatique – Université Paris 8.