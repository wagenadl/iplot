function h = itext(varargin)
% ITEXT - Add text to an IAxes
%    ITEXT(x, y, txt) places text at the given location
%    ITEXT(txt) places text at the origin
%    ITEXT(..., key, value, ...) specifies additional properties
%    h = ITEXT(...) returns a handle

if nargin==0 || nargin==2
  error('ITEXT takes 1, 3, or more arguments');
elseif nargin==1 || ischar(vargargin{2})
  x = 0;
  y = 0;
  txt = varargin{1};
  k0 = 2;
else
  x = varargin{1};
  y = varargin{2};
  txt = varargin{3};
  k0 = 4;
end

if mod(nargin+1-k0, 2)
  error('Keys and values must come in pairs');
end

id = ip_send('text', 0, [x y], txt);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

for k=k0:2:nargin
  iset(h, varargin{k}, varargin{k+1});
end

if nargout<1
  clear h
end
