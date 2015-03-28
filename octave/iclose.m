function iclose(h)
% ICLOSE - Close an IFigure
%    ICLOSE(h) closes a figure identified by its handle
%    ICLOSE closes the current figure
%    ICLOSE ALL closes all figures

if nargin==0
  h = igcf();
end
if nargin>1
  error('ICLOSE takes 1 argument');
end

if isscalar(h) && h>0
  id = ip_send('close', h)
elseif ischar(h) && strcmp(h, 'all')
  id = ip_send('close', 0);
elseif nargin==0
  return; % Nothing to do
else
  error('ICLOSE must be given a handle or "all"');
end
[cmd, h] = ip_receive(id);
ip_error(cmd, h);
