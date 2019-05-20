#1:type(A = AX12 / S = servo / N = Nema17), 2:ID, 3:angle(servo)/hauteur(nema), 4:couple/vitesse(nema)(mm/s), 5:time(ms)

#Ouverture de la pince centrale et fermeture des pinces gauche et droite
A 2 250 240 0000
A 3 120 240 0000
A 4 250 240 1000

#Deploiement des pinces
A 1 450 100 1000

#Ajout des points
P 20 0 0 0