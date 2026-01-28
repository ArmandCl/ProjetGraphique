# Projet Informatique Graphique - Scène 3D

Notre projet représente une scène 3D développée en C++ avec OpenGL.

## Comment lancer le projet

### Installation classique
Pour compiler le projet manuellement, suivez ces étapes dans un terminal :

1.  Cloner le dépôt.
2.  Aller à la racine du projet.
3.  Exécuter les commandes suivantes :
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build . --config Debug
    ```

### Utilisation du script de lancement
J'ai mis à disposition un script pour automatiser la vérification du build, la compilation et le lancement avec les bons chemins de textures :

```powershell
# On entre dans le dossier build
if (-Not (Test-Path "build")) { mkdir build }
cd build

# On compile le projet
Write-Host "--- Compilation en cours... ---" -ForegroundColor Cyan
cmake --build . --config Debug

# On vérifie si la compilation a réussi avant de lancer
if ($LASTEXITCODE -eq 0) {
    Write-Host "--- Lancement du programme ---" -ForegroundColor Green
    cd ..
    # On lance l'exécutable depuis la racine pour que les chemins de textures soient bons
    .\build\Debug\opengl_program.exe
} else {
    Write-Host "--- Erreur de compilation ! ---" -ForegroundColor Red
    cd ..
}
```

### Contenu de la scène
La pièce est composée des objets suivants, modélisés ou programmés individuellement :

- La structure de la pièce : Murs, sol et plafond avec textures.
- Le mobilier : Un bureau, une table de chevet et son lit.
- L'équipement informatique : Écran, clavier, souris et unité centrale.
- Décoration : Un poster sur le mur.
- Animations : Des ventilateurs fonctionnels à l'intérieur de l'ordinateur.
- Éclairage : Une lampe au plafond avec gestion des ombres.

### Méthodologie de travail
Nous avons majoritairement travaillé directement sur OpenGL, ce qui a représenté la partie la plus complexe du projet (définition des buffers, coordonnées de textures, normales, etc.).

Lorsque certaines formes étaient trop complexes à coder manuellement, nous avons modélisé les objets sur Blender pour ensuite les importer dans notre moteur OpenGL.

### Commandes
```C``` : Alterner entre la caméra orbitale (automatique) et la caméra libre.
```Z/Q/S/D``` : Se déplacer en mode caméra libre.
```Souris``` : Orienter le regard.
```Molette``` : Zoomer la caméra
```Echap``` : Quitter le programme.
