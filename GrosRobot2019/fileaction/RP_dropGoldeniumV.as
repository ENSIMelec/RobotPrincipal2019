#1:type(A = AX12 / S = servo / N = Nema17), 2:ID, 3:angle(servo)/hauteur(nema), 4:couple/vitesse(nema)(mm/s), 5:time(ms)

#remontée du nema pour tempo
N 0 140 45 2100

#ouverture de la pince gauche et fermeture de la pince droite
A 2 180 240 0
A 4 240 240 200

#déploiement des pinces
A 1 450 100 200

#ouverture de la pince centrale
A 3 160 240 1000

#Ajout des points
P 24 0 0 0