function val = iget(h, key)
% IGET - Retrieve a property from an IObject
%    val = IGET(h, key) retrieves the property named by KEY from the IObject
%    identified by the handle H.
%    val = IGET(h) is the same as val = IGETALL(h).

if nargin==1
  val = igetall(h);
  return;
elseif nargin!=2
  error('ISET takes 2 arguments');
end
 
if length(key)>3 && strcmp(key(end-2:end), 'fcn')
  val = icallback(h, key);
else
  id = ip_send('get', h, key);
  [cmd, h, k, val] = ip_receive(id);
  ip_error(cmd, h);
end
