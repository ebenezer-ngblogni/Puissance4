# Puissance 4

Un jeu de Puissance 4 développé en C avec des fonctionnalités avancées de sauvegarde et pause.

## Description

Ce projet implémente le jeu classique du Puissance 4 avec une interface en ligne de commande. Le jeu permet de jouer à deux joueurs ou contre une IA de différents niveaux de difficulté.

## Fonctionnalités

### Modes de jeu
- **Joueur vs Joueur (PvP)** : Deux joueurs humains s'affrontent avec un timer par coup
- **Joueur vs IA** : Affrontez l'intelligence artificielle avec deux niveaux de difficulté :
- **Facile** : L'IA joue des coups aléatoires
- **Difficile** : L'IA analyse la grille pour bloquer et gagner

### Système de profils
- Création et gestion de profils utilisateurs
- Sauvegarde automatique de l'historique des parties
- Statistiques par joueur
- Paramètres personnalisables par profil

### Personnalisation
- **Taille de grille configurable** (minimum 6 lignes × 7 colonnes)
- **Timer par coup personnalisable** (mode PvP uniquement)
- **Styles de pions** :
- Style 1 : Classique (X et O)
- Style 2 : Cœurs (♥)
- Style 3 : Étoiles (★)

### Sauvegarde et reprise
- **Pause en cours de partie** : Appuyez sur 'P' ou 'Q' pour sauvegarder et quitter
- **Une partie en pause par joueur** : Protection contre l'écrasement avec confirmation
- **Reprise automatique** : Restauration complète de l'état de la partie (grille, joueur actuel, timer)
- **Historique des parties** : Visualisation coup par coup des parties terminées

### Interface
- Affichage coloré (compatible Linux et Windows)
- Grille numérotée pour faciliter le jeu
- Messages clairs et retours visuels
- Timer en temps réel (mode PvP)

## Prérequis

- **Compilateur C** : GCC ou compatible
- **Système d'exploitation** : Linux ou Windows
- **Bibliothèques** :
- `stdio.h`, `stdlib.h`, `string.h`, `time.h`
- Linux : `unistd.h`, `sys/select.h`, `ctype.h`
- Windows : `conio.h`, `windows.h`

## Installation et Compilation

### Linux
```bash
gcc -o puissance4 main.c menu.c gameplay.c display.c file.c profil.c utils.c -std=c99
./puissance4
```
### Windows
```bash
gcc -o puissance4.exe main.c menu.c gameplay.c display.c file.c profil.c utils.c -std=c99
puissance4.exe
```

## Utilisation

### Démarrage

1. Lancez le programme
2. Créez un profil ou connectez-vous avec un profil existant
3. Choisissez un mode de jeu dans le menu principal

### Menu principal
1. Nouvelle Partie
	1. Jouer contre un autre joueur (PvP)
	2. Jouer contre l'IA
    	1. Facile
     	2. Difficile
2. Modifier les paramètres
3. Visualiser l'historique des parties
4. Reprendre une partie en pause
5. Changer d'utilisateur
6. Afficher les règles du jeu
7. Quitter

### Pendant une partie

- Entrez un numéro de colonne (1 à nombre de colonnes) pour jouer
- Appuyez sur 'P' ou 'Q' pour mettre la partie en pause
- Le pion tombe dans la case libre la plus basse de la colonne choisie

### Conditions de victoire

- Alignez 4 pions de votre couleur :
- Horizontalement
- Verticalement
- En diagonale (montante ou descendante)
- Si la grille est pleine sans alignement : Match nul

## 📁 Structure du projet
```
Puissance4/
├── main.c              # Point d'entrée du programme
├── menu.c/h            # Gestion du menu principal
├── gameplay.c/h        # Logique du jeu et IA
├── display.c/h         # Affichage de la grille
├── file.c/h            # Sauvegarde/chargement des parties
├── profil.c/h          # Gestion des profils utilisateurs
├── utils.c/h           # Fonctions utilitaires
├── files/              # Dossier des fichiers de sauvegarde
│   ├── *.config.txt    # Historique des parties par joueur
│   └── *.pause.txt     # Parties en pause
└── README.md           # Ce fichier
```
## 🎲 Règles du jeu

### Objectif

Être le premier à aligner 4 pions de sa couleur horizontalement, verticalement ou en diagonale.

### Déroulement

1. Les joueurs jouent à tour de rôle
2. Le joueur connecté (symbolisé par X ou le premier symbole) commence toujours
3. Chaque joueur choisit une colonne où placer son pion
4. Le pion tombe dans la case vide la plus basse de la colonne

### Mode PvP

- Chaque joueur dispose d'un temps limité par coup (configurable, 20 secondes par défaut)
- Si le temps est écoulé sans jouer, le tour passe automatiquement à l'adversaire

### Mode PvIA

- Pas de limite de temps pour le joueur humain
- L'IA joue automatiquement après votre coup

### Pause

- Une seule partie en pause autorisée par joueur
- Tenter de sauvegarder une nouvelle partie affiche un avertissement
- Les parties en pause sont automatiquement proposées au login

## 🔧 Configuration

### Paramètres personnalisables (option 6 du menu)

- Taille de la grille : Lignes et colonnes (min 6×7)
- Temps par coup : Durée en secondes (mode PvP uniquement)
- Style des pions : Choix parmi 3 styles visuels

### Fichiers de configuration

- files/<pseudo>.config.txt : Historique complet des parties
- files/<pseudo>.pause.txt : État de la partie en pause

```
Partie jouee contre <adversaire> le DD/MM/YYYY à HH:MM:SS
<joueur> <colonne>
<joueur> <colonne>
...
2 <durée_totale>
<message_de_victoire>
```

## Licence

Projet tutoré par Mme Nisrine BAJJA (Ingénieur en IA) - Utilisation libre dans un cadre académique.

---

Bon jeu ! Que le meilleur gagne ! 🏆
