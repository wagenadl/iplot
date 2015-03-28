function idelete(hh)
% IDELETE - Delete IObjects
%    DELETE(hh) deletes all the IObjects identitified by the handles HH.
%    Those can be figures, axes, plots, anything. Descendents of deleted
%    objects are deleted as well.
	 
if nargin~=1
  error('IDELETE takes 1 argument');
end

for h=hh(:)
  ip_send('delete', h);
  [cmd, h] = ip_receive(id);
  ip_error(cmd, h);

  % We ought to drop callbacks, but it is not critical
end
