import socket

PORT = 3333

HOST = '172.30.1.1'



milhabot = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
milhabot.connect((HOST, PORT))
print 'Connexion vers ' + HOST + ':' + str(PORT) + ' reussie.'

#Quand Milhabot recevra O, il saura que Kiroulpa est connecté
milhabot.send("O")

#Fermeture 
print 'Deconnexion'
milhabot.close()
