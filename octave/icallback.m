function foo = icallback(h, key, foo)
% ICALLBACK - Install or inspect a callback on an IObject
%    ICALLBACK(h, key, foo) installs FOO as a callback on IObject H for
%    event KEY.
%    foo = ICALLBACK(h, key) returns the currently installed callback.
%    ICALLBACK(h, key, []) removes a callback.
%    The function FOO is called with the object's handle as its first
%    argument and a KEY dependent second argument.
%    Note: You may find it more convenient to use ISET and IGET to set
%    and query callbacks.

if nargin<2 || nargin>3
  error('ICALLBACK takes 2 or 3 arguments');
end

global ip_callbacks;
if isempty(ip_callbacks)
  ip_callbacks.hh = [];
  ip_callbacks.tbl = {};
end

idx = find(ip_callbacks.hh==h);
if ~isempty(idx)
  kidx = find(strcmp(ip_callbacks.tbl{idx}.keys, key));
else
  kidx = [];
end

if nargin==3
  % Set or remove a callback
  if isempty(foo)
    % Remove a callback
    iset(h, key, 0);
    if ~isempty(idx) && ~isempty(kidx)
      N = length(ip_callbacks.tbl{idx}.keys);
      ip_callbacks.tbl{idx}.keys ...
          = ip_callbacks.tbl{idx}.keys([1:kidx-1 kidx+1:N]);
      ip_callbacks.tbl{idx}.foos ...
          = ip_callbacks.tbl{idx}.foos([1:kidx-1 kidx+1:N]);
    end
  else
    % Add a callback
    if isempty(idx)
      K = length(ip_callbacks.hh);
      ip_callbacks.hh(K+1) = h;
      ip_callbacks.tbl{K+1}.keys = { key };
      ip_callbacks.tbl{K+1}.foos = { foo };
    elseif isempty(kidx)
      ip_callbacks.tbl{idx}.keys(end+1) = key;
      ip_callbacks.tbl{idx}.foos(end+1) = foo;
    else
      ip_callbacks.tbl{idx}.foos(kidx) = foo;
    end
    iset(h, key, 1);
  end  
  if nargout<1
    clear foo
  end
elseif ischar(key)
  % Report callback
  if isempty(idx) || isempty(kidx)
    foo = [];
  else
    foo = ip_callbacks.tbl{idx}.foos{kidx};
  end
else
   % Likely called as icallback(h, @foo), which is illegal
   error('ICALLBACK needs a handle, a key, and a function');
end
