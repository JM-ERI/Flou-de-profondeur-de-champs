# Flou-de-profondeur-de-champs
Ajout d'un flou de profondeur de champs sur une image RGB-D en C++

# Manuel Utilisateur
## Le Programme focus
Ce programme permet d’ajouter du flou de profondeur de champs sur une image qui n'en a pas.<br>
Nous travaillerons uniquement sur des images RGB-D.

<b>Usage : ./bin/focus <my_image></b>
- <b><i>my_image</i></b> : nom de l'image sans l'extension ( dans le dossier input)
- un fichier <b><i>my_image_depth</i></b> représentant l’image avec la profondeur de chaque pixel devra être
également trouvé dans le dossier input et sera utilisé automatiquement par le programme
  
## Commandes utilisateur
un certains nombre de commandes d’interactions sont disponibles pour un utilisateur:<br>
<b><i>q</i></b> : quitte le programme<br>
<b><i>s</i></b> : exporte l'image courante dans un fichier nommé "export_" + position du clic_x + position du clic_y.jpg<br>
<b><i>o</i></b> : affiche une fenêtre montrant l'objet correspondant au clic fait par l'utilisateur<br>
<b><i>+ ou p</i></b> : augmente la différence de profondeur de pixel pour qu'un pixel soit reconnu comme faisant
parti ou non de l'objet sur lequel on a cliqué<br>
<b><i>- ou m</i></b> : diminue la différence de profondeur de pixel pour qu'un pixel soit reconnu comme faisant
parti ou non de l'objet sur lequel on a cliqué<p>
La différence de profondeur de pixel est de 10 par défaut

# Exemples
![Book](https://user-images.githubusercontent.com/60750434/89559203-a1483c00-d815-11ea-8810-e0fcf7b1d25a.png)   ![Book_flou](https://user-images.githubusercontent.com/60750434/89559291-c0df6480-d815-11ea-8b43-a4dd6416c461.png)
![Cone](https://user-images.githubusercontent.com/60750434/89559334-d18fda80-d815-11ea-9159-38009633ce17.png)   ![Cone_flou](https://user-images.githubusercontent.com/60750434/89559338-d2c10780-d815-11ea-9acb-1e217889da30.png)

# Auteurs
Jean-Manuel ERIALC  --  Hadjer DJERROUMI
