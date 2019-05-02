#1:type(A = AX12 / S = servo), 2:IDservo / numero, 3:angle, 4:force, 5:time(ms)

#déploiement des pinces
A 1 380 100 0

#baisser nema
N 0 80 22000000 2000

#open
A 2 160 240 0
A 3 160 240 0
A 4 160 240 1000