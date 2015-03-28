function h = ipatch(xx, yy, varargin)
% IPATCH - Create new IPatch or switch to existing
%   IPATCH(xx, yy) creates a new patch series in the current axes.
%   IPATCH(xx, yy, key, value, ...) specifies additional properties.
%   h = IPATCH(...) returns its handle.

if nargin<2
  error('IPATCH takes 2 at least arguments');
end
if mod(nargin, 2)
  error('Keys and values must come in pairs');
end

id = ip_send('patch', 0, xx, yy);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

for q=1:2:length(varargin)
  iset(h, varargin{q}, varargin{q+1});
end


if nargout<1
  clear h
end
