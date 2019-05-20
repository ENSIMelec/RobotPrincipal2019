#1:type(A = AX12 / S = servo), 2:IDservo/num�ro, 3:angle, 4:force, 5:time(ms)
#forward
A 1 550 1023 1000

#close
A 2 230 1023 0
A 3 230 1023 0
A 4 230 1023 2000

#backward
A 1 850 1023 2000
#A 1 400 1023 10000

#open
A 2 165 240 0
A 3 165 240 0
A 4 165 240 0
