#1:type(A = AX12 / S = servo), 2:IDservo / numero, 3:angle, 4:force, 5:time(ms)

#descente du nema
N 0 0 50000000 3000

#fermeture des pinces
A 2 250 240 0
A 3 250 240 0
A 4 250 240 1000

#remontée du nema
N 0 30 50000000 3000

#rangement des pinces
A 1 225 100 1000

#descente du nema
N 0 0 50000000 2000

