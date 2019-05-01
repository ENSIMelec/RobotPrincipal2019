#1:type(A = AX12 / S = servo), 2:IDservo/numero, 3:angle, 4:force, 5:time(ms)

#Baguettes
S 2 145 0 0
S 3 10 0 1000

#Fermer Pinces
A 2 230 1023 0
A 3 230 1023 0
A 4 230 1023 0

#Rentrer Pinces
A 1 225 100 1000

#init du nema
N 0 20 50000000 1000