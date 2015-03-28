function h = ifigure(h, x)
% IFIGURE - Create new IFigure or switch to existing
%    h = IFIGURE creates a new IFigure and returns its handle.
%    IFIGURE(h) switches to an existing IFigure.
%    h = IFIGURE(width, height) creates a new IFigure with given size
%    in pixels.

xy = [];
if nargin==0
  h = 0;
elseif nargin==2
  xy = [h x];
  h = 0;
elseif nargin>2
  error('IFIGURE takes 0 to 2 arguments');
end

if isempty(xy)
  id = ip_send('figure', h);
else
  id = ip_send('figure', h, xy);
end

[cmd, h] = ip_receive(id);
ip_error(cmd, h);

if nargout<1
  clear h
end
