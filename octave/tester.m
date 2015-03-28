f = ifigure;
h = iplot([0 1 2 3], [0 2 1 3]);
k = ipoints(2, 2);
icallback(k, 'buttonmotionfcn', @moveme);
icallback(h, 'buttondownfcn', @(h,x) ( iwake ));

itext(1, 1, 'Foo');
ibutton('Label');
ibutton('Push', @(h,x) ( printf('Pushed %i %i\n', h, x) ))
ibutton('check', 'Check', @(h,x) ( printf('Check %i %i\n', h, x) ))
iset(f, 'title', 'Pretty IPlot window');
iset(igca(), 'xlabel', 'x-axis');
iset(igca(), 'ylabel', 'y-axis');

%iwait(f);

