Application de gestion d’élection pour les étudiants (E-Election) :

1. Le programme commence par demander le nombre d’administrateurs et d’électeurs (étudiants).

2. Ensuite, il crée une structure de file pour stocker les informations des administrateurs et des étudiants, et une structure de pile pour stocker les informations des candidats, tout en gardant une copie de ces informations dans deux fichiers au fur et à mesure du programme, un pour tout le monde et un pour les candidats.

3. Le programme entre ensuite dans une boucle où il demande à chaque administrateur et électeur d’entrer leurs informations. Si une personne choisit de se présenter comme candidate, elle est ajoutée à la pile des candidats et enregistrée dans le fichier correspondant.

4. Après avoir recueilli toutes les informations, le programme entre dans le processus de vote. Chaque électeur est invité à entrer son numéro de carte d’identité et à choisir un candidat pour lequel voter.

5. Si un administrateur entre son numéro de carte d’identité pendant le processus de vote, il a la possibilité d’arrêter les élections en entrant 0 (par exemple, dans le cas où la durée de l’élection est passée, dans ce cas, l’administrateur doit arrêter les élections même si pas tous les étudiants ont voté).

6. Enfin, le programme compte les votes et détermine le candidat gagnant, puis enregistre ses informations dans un fichier.
