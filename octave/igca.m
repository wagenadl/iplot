function h = igca
% IGCA - Return handle of current IAxes
%   h = IGCA returns the handle of the current IAxes, and, like the
%   corresponding native function GCA creates IAxes if none existed before.
	 
id = ip_send('gca', 0);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

