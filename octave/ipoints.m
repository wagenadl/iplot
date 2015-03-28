function h = ipoints(varargin)
% IPOINTS - Create new IPoints series
%    IPOINTS(xx, yy) creates a new points series in the current axes.
%    IPOINTS(yy) assumes XX is 1:LENGTH(YY)
%    IPOINTS(..., key, value, ...) specifies additional properties

if nargin<1
  error('IPOINTS takes at least 1 argument');
end


if nargin==1 || ischar(varargin{2})
  yy = varargin{1};
  xx = [1:length(yy)];
  k0 = 2;
else
  xx = varargin{1};
  yy = varargin{2};
  k0 = 3;
end

if mod(nargin+1-k0, 2)
  error('Keys and values must come in pairs');
end

id = ip_send('points', 0, xx, yy);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

for q=k0:2:nargin
  iset(h, varargin{q}, varargin{q+1});
end

if nargout<1
  clear h
end

