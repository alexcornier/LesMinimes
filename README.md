# Projet "Les MinimesEnUnClic"

Projet de Master 1 en Biologie Computationnelle à L'université de Bordeaux, 2019-2020

Dans le cadre de notre projet en programmation orientée objet, nous devons réaliser un programme permettant la gestion du port de plaisance des Minimes à La Rochelle et permettre ainsi une comptabilité exemplaire. Pour cela le port dispose d’une liste d’abonnés, d’une liste de places pour les bateaux et une liste de tous les différents services qui sont proposés aux clients.

Les clients du ports se distinguent en deux catégories :

* **Les abonnés** : possédant une place permanente pour leur bateau et donc qui règlent à la fin de chaque mois sur la base d’une cotisation annuelle
* **Les clients** : qui sont de passage seulement pour quelques jours et qui règlent une taxe par jour en fonction de la durée de leur séjour

Les tarifs que le port propose dépendent également de la taille du bateau de chaque usager. En effet on distingue 3 catégories de bateaux :

* **Les voiliers non habitables** : ce sont des bateaux dont la taille est inférieure à 10 mètres de long ne possédant pas de cabine. Ces bateaux n’utilisent pas les services du port tel que l’électricité ou l’eau
* **Les voiliers de type 1** : ce sont des bateaux dont la taille est comprise entre 10m et 25m de long avec une ou plusieurs cabines, ils peuvent utiliser les services de branchement à l’électricité et à l’eau
* **Les voiliers de type 2** : ce sont des bateaux identiques aux voiliers de type 1 mais avec une taille supérieure à 25m. Ce type de bateau nécessite une place spécifique, de grande taille, et celles-ci sont en nombre limité

Le but du programme LesMinimesEnUnClic est de pouvoir proposer des places disponibles et d’automatiser la gestion du paiement de chaque client. Pour cela on doit être capable  d’enregistrer un nouvel usager en renseignant toutes les informations que nous avons vues précédemment. Il doit permettre également l’enregistrement d’un nouveau paiement et enfin d’éditer une facture, celle-ci, bien sûr devant être conservée dans un fichier historique.

## Pour Commencer

Cette application a été développée en c++11.
La lecture du document **[doc/CORNIER_Alexandre.pdf](doc/CORNIER_Alexandre.pdf)** fourni tous les détails relatifs à cette application.

### Pré-requis

Les prérequis correspondant aux connaissances nécessaires et à l'environnement de développement sont les suivants :

* Compilateur C++
* un EDI, par exemple VSCode avec le module C++

### Installation

Pour installer ce projet il faut :

* cloner le repository
* Compiler l'application
  * les fichiers **[build_mac](build_mac)** et **[build_win](build_win)** fournissent la commande utilisée pour compiler sur Mac OS X et sur Windows
* Executer le programme compilé et naviguer dans les menus console

## Fabriqué avec

Les programmes/logiciels/ressources utilisés pour développer cette Application :

* [C++](http://mingw-w64.org/doku.php) - C++, par exemple Mingw-w64 sous Windows 10
* [Visual Studio Code](https://code.visualstudio.com/docs/languages/markdown) - VS Code avec le module C++ et les extensions markdown pour la création de ce README.md

## Versions

Versions disponible :

* **Dernière version :** 1.0

## Auteur

Application développée par :

* **Alexandre Cornier** _alias_ [@alexcornier](https://github.com/alexcornier/)

## License

Ce projet est sous licence ``MIT License`` - voir le fichier [LICENSE.md](LICENSE/LICENSE.md) pour plus d'informations.
