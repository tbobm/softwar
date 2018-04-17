Évenement |Description |Coût action |Coût énergie |Commande |Paramètres |Data de la réponse |Destinataire
------|------|------|------| ------|------|------|------
Connexion d’un client |Un client se connecte et s’identifie |NA |NA |identify |client\_id (ex #0x01) |null |Serveur
Avancer |Le processus avance d’une case |0.5 |0 |forward |null |null |Serveur
Reculer |Le processus recule d’une case |1 |0 |backward |null |null |Serveur
Aller à gauche |Le processus pivote vers la gauche et avance d’une case |1 |0 |leftfwd |null |null |Serveur
Aller à droite |Le processus pivote vers la droite et avance d’une case |1 |0 |rightfwd |null |null |Serveur
Pivoter vers la droite |Le processus pivote vers la droite |0.5 |0 |right |null |null |Serveur
Pivoter vers la gauche |Le processus pivote vers la gauche |0.5 |0 |left |null |null |Serveur
Orientation |La direction dans laquelle le processus regarde |0 |0 |looking |null |1(voir enum direction) |Serveur
Récolter de l’énergie |Le processus extrait de l’énergie d’une case en surcharge |1 |0 |gather |null |null |Serveur
Regarder |Le processus regarde les cases devant lui |0 |0 |watch |null |voir “vision serveur” |Serveur
Envoyer une vague d’énergie |Le processus envoie une vague d’énergie devant lui |0.5 |1 |attack |null |null |Serveur
Récupérer son identifiant |Le processus va récupérer son propre identifiant |0 |0 |selfid |null |“#0x01” (exemple) |Serveur
Voir son energie |Le processus va récupérer inspecte son énergie |0 |0 |selfstats |null |42 (exemple) |Serveur
Analyser un processus |Examiner un processus à partir de sa signature numérique |0.5 |0 |inspect |#0x01 (exemple) |55 (exemple) |Serveur
Passer un tour |Le processus demande la fin de son tour |0 |0 |next |null |null |Serveur
Bondir vers l’avant |Le processus fait un bond de deux cases vers l’avant |0 |2 |jump |null |null |Serveur
