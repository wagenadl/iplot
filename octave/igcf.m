function h = igcf
% IGCA - Return handle of current IFigure
%   h = IGCF returns the handle of the current IFigure, and, like the
%   corresponding native function GCF creates IFigure if none existed before.
	 
id = ip_send('gcf', 0);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

