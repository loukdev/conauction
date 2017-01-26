# [EN]

## conauction
This is a Licence Informatics 3rd year project developped in C language, using POSIX sockets.
The program simulates auctions, devided into two parts :
- The auctionneer : there is only one. He commands the conauction server.
- An outbidder : there are several and one of them win the auction. Each one is on a conauction client.

Because it uses network sockets, the auctionner and the outbidders can be - and should be - on different computers, whenever they are into the world. In this obvious condition : being on the same network (same local network or connected on internet).

For more details, look at http://lab-sticc.univ-brest.fr/~leparc/Etud/Licence/Reseau/Serie2UDP/TP_UDP.pdf (in french).

## What is that name ?
This program simulates auction in console, this is not a GUI, and uses network with POSIX sockets.
conauction is a contraction of two words : console and auction. By the way, it does a pun with "connection".

# [FR]

## conauction
Ceci est un projet de 3e année de Licence Informatique développé en C, utilisant les sockets POSIX (chaussettes, en français ?).
Le programme simule une vente aux enchères, il est divisé en deux parties :
- Le commissaire-priseur : il y en a un seul. Il commande la version serveur de conauction.
- Un surenchérisseur : il y en a plusieurs, l'un d'eux finit par acquérir l'objet de la vente. Chacun est sur une version client de conauction.

Du fait que le programme utilise les sockets réseaux, le commissaire-priseur et les surenchérisseurs peuvent être - et devraient être - sur des machines différentes, où qu'ils soient dans sur la planète. A la condition évidente qu'ils soient tous sur le même réseau (même réseau local ou connectés à internet).

Pour plus de détails, allez jeter un oeil ici : http://lab-sticc.univ-brest.fr/~leparc/Etud/Licence/Reseau/Serie2UDP/TP_UDP.pdf (en français).

## Quel est ce nom ?
Ce programme simule une vente aux enchères (en anglais : auction) sur console (en anglais : ...console), il n'y pas d'interface graphique fenêtrée.
Le nom est une contraction des deux mots "auction" et "console". Ce qui fait un jeu de mot avec "connection" (avec une petite pirouette de prononciation) - connexion en français.
