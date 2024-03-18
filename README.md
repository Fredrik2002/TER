### Spoiler : Ceci n'est pas un README

Rappel : 
- Glouton 1 part du sommet 0 et va toujours au plus proche voisin par encore exploré. 
    Complexité asymptotique : O(n²)
- Glouton 2 appelle Glouton 1 sur tous les sommets de départ possible et garde la meilleure solution :

    Complexité asymptotique : O(n<sup>3</sup>)


### Est-ce qu'à partir des solutions approchées, on peut deviner laquelle des méthodes exactes sera la plus efficace ? 
<br><br>
### Est-ce qu'on peut construction une instance qui prouverait que les solutions gloutonnes ne sont pas APX ?
#### (Autrement dit, il n'existe pas de p tel que OPT*p<glouton)
<br><br>

__Analyse de Données__ <br><br>
Main 1: N = 14
On cherche surtout de la régularité pour toutes les instances sur un N fixé :
- Branch & Bound : Corrélation entre le nombre de noeuds explorés et le temps de résolution
- Branch & Bound vs Backtracking : Corrélation entre les 2 temps de résolutions
- Est-ce qu'une méthode est plus régulière que les autres (typiquement le bracktracking)

<br><br>

Main 2 : N € [5;20]
Expérience :
Pour chaque N, on génère 100 instances, et on calcule la moyenne sur ces 100 instances. 

- Observer la complexité qui devrait croitre de façon exponentielle
- Solutions approchées vs Solution exacte : Écart entre les solutions approchées et la solution exactes
