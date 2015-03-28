function h = iaxes(h)
% IAXES - Switch to an IAxes object
%   IAXES(h) switches to the given IAxes object so that subsequent IPLOT 
%   calls (etc) are directed there.

if nargin~=1
  error('IAXES takes 1 argument');
end

id = ip_send('axes', h);
[cmd, h] = ip_receive;
ip_error(cmd, h);

if nargout<1
  clear h
end