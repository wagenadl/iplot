function moveme(h, a)

a = igca();
xy = iget(a, 'currentpoint');
iset(h, 'ydata', xy(2));
