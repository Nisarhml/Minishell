sur le prompt :

- quand on tape Entree sans rien -> Segfault  [OKOK]
- quand on tape un token (|, <>) sans rien -> Segfault  [OKOK]
- quand je met un caractere special (. , / ....) -> double free detected  -->  et apres un debuggage, je vois qu'il y a 2 tours de boucle qui se fait d'ou le double free


petit OBJECTIF :

cette ligne doit fonctionner correctement : pwd >> a | wc -c << here >b|echo kk >c

un des problemes que j'ai ici est que les '>' du heredoc rentrent dans les fichier a et c, et rien n'est affiche dans b, et un ou plusieurs process ne se terminent pas