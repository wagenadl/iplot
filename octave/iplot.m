function h = iplot(varargin)
% IPLOT - Create new IPlot or switch to existing
%    IPLOT(xx, yy) creates a new line series in the current axes.
%    IPLOT(xx, yy, pen) specifies a linestyle (one of '-', '.', or '--').
%    IPLOT(xx, yy, key, value, ...) specifies additional properties.
%    IPLOT(yy, ...) assumes XX to be 1:LENGTH(YY).
%    h = IPLOT(...) returns the new line series' handle.

if nargin<1
  error('IPLOT takes at least 1 argument');
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

cc = '';
ll = '';

if nargin>=k0
  pp = varargin{k0}; % putative pen
  k0 = k0 + 1;
  for n=1:length(pp)
    if ~isempty(find('rgbcmykw'==pp(n)))
      cc = pp(n);
    elseif ~isempty(find('-.'==pp(n)))
      ll(end+1) = pp(n);
    else
      k0 = k0 - 1;
      cc = '';
      ll = '';
      break;
    end
  end
end

if mod(nargin+1-k0, 2)
  error('Keys and values must come in pairs');
end
if ~isempty(cc)
  cc = ip_parsecolor(cc);
end

id = ip_send('line', 0, xx, yy);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

if ~isempty(cc)
  iset(h, 'color', cc);
end
if ~isempty(ll)
  iset(h, 'linestyle', ll);
end
for q=k0:2:nargin
  iset(h, varargin{q}, varargin{q+1});
end

if nargout<1
  clear h
end
