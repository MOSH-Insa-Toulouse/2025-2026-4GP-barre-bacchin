# Partie I : Contexte 

Le but de ce projet est de réaliser un capteur de flexion lowtech à base de crayon à papier et d'une feuille de papier.  
L'origine de ce projet vient d'un article publié en 2014 dans Nature par _Lin, CW, Zhao, Z., Kim, J. et al._ nommé "**Pencil Drawn Strain Gauges and Chemiresistors on Paper "**.  
Le capteur est dessiné par dépot de graphite sur une feuille à l'aide d'un crayon de papier. Ce dépot est un système granulaire dont la conductance dépend de la distance inter-grain. Plus la distance inter-grain diminue, plus la conductance augmente. C'est exactement ce qu'il se passe lorsque la feuille est placé en compression. De ce fait, on peut théoriquement déduire l'angle de flexion de la feuille en fonction de la résistance du capteur.  
  
Notre travail consiste a réaliser le capteur, le circuit électronique de lecture et de le tester afin de le comparer à des capteurs commerciaux. 

## Partie I.1 : Liste des Livrables

1. Un shield PCB adapté pour une Arduino UNO permettant de faire la mesure de résistance du capteur graphite, celle du capteur commercial et de connecter un module Bluetooth pour envoyer les résulats sur une application.
2. La dite application permettant de voir la valeur de la résistance mesuré et la variation relative de résistance.
3. Le code Arduino qui gère la mesure de flexion et le module bluetooth
4. Le protocole de mesure du banc de test
5. La datasheet du capteur graphite

## Partie I.2 : Liste du matériel utilisé


# Partie II : Réalisation théorique



## Partie II.1 : Conception du circuit amplificateur
La résistance du capteur est énorme, de l'ordre du GigaOhm, ainsi le courant obtenu en sortie est très faible. Il nous faut donc amplifier le signal afin de l'exploiter.  

Pour ce faire nous avons réaliser un circuit amplificateur transimpédance que nous avons simulé sur LTSpice.

## Partie II.2 : Design du PCB sous KiCad

Une fois que nous avons fixé ce la forme de notre circuit et tous les modules que nous voulons connecté à l'Arduino, nous avons pu concevoir la shield.  
Pour ce faire, on a utilisé le logiciel KiCad. 
Comme tous les composants n'étaient pas présent dans la bibliothèque de composant de KiCad, nous avons dû réalisé les schématiques et les empreintes du flex sensor, du capteur graphite, du module bluetooth et du potentiomètre digital.   
Voilà le schématique complet de notre shield :   
**INSERER IMAGE SCHEMATIQUE**

Concernant les empreintes, nous n'allons pas souder les composants onéreux directement. Ainsi pour le flex sensor et le module bluetooth nous avons prévu des pins dans lesquels nous brancherons les modules. Pour l'AOP et le potentiomètre nous allons souder des support DIP8.  
Ainsi notre empreinte ressemble à cela :  
**INSERER IMAGE SCHEMATIQUE**

Enfin la modélisation 3D donne :  
**INSERER IMAGE SCHEMATIQUE**

# Partie III : Fabrication et fonctionnement

# Partie IV : Banc de test et caractérisation

# Partie V : Conclusion