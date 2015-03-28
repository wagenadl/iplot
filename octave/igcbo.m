function h = igcbo
% IGCBO - IObject handle of callback source
%   h = IGCBO returns the handle of the object where the current callback
%   originated.
	 
id = ip_send('gcbo', 0);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

