function kk = iset(h, varargin)
% ISET - Set properties on an IObject
%    ISET(h, key, value, ...) specifies property values
%    In addition to the predefined properties, ISET allows to set user-defined
%    properties. User-defined properties must have names starting with "*".
%    ISET(h) is the same as IPROPERTIES(h).

if nargin==1
  kk = iproperties(h);
  return;
elseif nargin==0 || mod(length(varargin), 2)>0
  error('ISET takes key, value pairs');
end

for k=1:2:length(varargin)
  key = varargin{k};
  val = varargin{k+1};
  if strcmp(key, 'color')
    if ischar(val)
      val = ip_parsecolor(val);
    end
  end
  if length(key)>3 && strcmp(key(end-2:end), 'fcn') && ~isnumeric(val)
    icallback(h, key, val);
  else
    id = ip_send('set', h, varargin{k}, varargin{k+1});
    cmd = ip_receive(id);
    ip_error(cmd);
  end
end
