#1:type(A = AX12 / S = servo / N = Nema17), 2:ID, 3:angle(servo)/hauteur(nema), 4:couple/vitesse(nema)(mm/s), 5:time(ms)

#Remontée des baguettes
S 14 0 0 0
S 15 150 0 0

#Fermer les pinces
A 2 230 1023 0
A 3 230 1023 0
A 4 230 1023 0

#Repliement des pinces
A 1 225 100 0

#Descente du nema
N 0 0 45 0