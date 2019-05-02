#1:type(A = AX12 / S = servo), 2:IDservo / numero, 3:angle, 4:force, 5:time(ms)

#déploiement des pinces qui sert a la tempo
A 2 250 1023 0
A 4 250 1023 2000

#descente du nema
N 0 55 22000000 3000

#fermeture de la pince
A 3 280 1023 1000

#remontée du nema
N 0 120 22000000 1000

#points
P 20 0 0 0