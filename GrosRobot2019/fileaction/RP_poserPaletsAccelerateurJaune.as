#1:type(A = AX12 / S = servo / N = Nema17), 2:ID, 3:angle(servo)/hauteur(nema), 4:couple/vitesse(nema)(mm/s), 5:time(ms)

#Deploiement des pinces
A 1 415 100 500

#Descente du nema
N 0 20 45 3000

#Ouverture des pinces
A 2 180 240 0
A 3 180 240 0
A 4 180 240 0

#Montee du Nema
N 0 150 45 2000

#Ajout des points
P 30 66 0 0

