function h = ibutton(typ, txt, foo)
% IBUTTON - Add a button to an IFigure
%   IBUTTON(text) adds a simple label
%   IBUTTON(text, foo) adds a push button with a callback
%   IBUTTON(type, text, foo) adds a button of specified type: 
%      push, radio, or label.
%   h = IBUTTON(...) returns the button's handle.

if nargin==1
  txt = typ;
  typ = 'label';
  foo = [];
elseif nargin==2
  if ischar(txt)
    foo = [];
  else
    foo = txt;
    txt = typ;
    typ = 'push';
  end
elseif nargin~=3
  error('IBUTTON takes 1, 2 or 3 arguments');
end

id = ip_send('button', 0, typ, txt);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

if ~isempty(foo)
  icallback(h, 'buttondownfcn', foo);
end

if nargout<1
  clear h
end
