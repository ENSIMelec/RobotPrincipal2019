#1:type(A = AX12 / S = servo / N = Nema17), 2:ID, 3:angle(servo)/hauteur(nema), 4:couple/vitesse(nema)(mm/s), 5:time(ms)

#Deploiement des pinces qui sert a la tempo
A 2 250 1023 0
A 4 250 1023 1500

#Descente du nema
N 0 75 45 2000

#Fermeture de la pince centrale
A 3 245 1023 15000

#Remontee du nema
N 0 120 45 1000

#Ajout des points
P 20 66 0 0