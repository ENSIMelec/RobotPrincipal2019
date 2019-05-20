import socket

PORT = 3333

HOST = '172.30.1.1'



milhabot = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
milhabot.connect((HOST, PORT))
print 'Connexion vers ' + HOST + ':' + str(PORT) + ' reussie.'

#Quand Milhabot recevra A, il ajoutera le nombre de points estimés (30) avec peut-être une probabilité
milhabot.send("A")

#Fermeture 
print 'Deconnexion'
milhabot.close()
