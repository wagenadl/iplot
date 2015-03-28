function h = igcbf
% IGCBF - IFigure handle of callback object
%    h = IGCBF returns the handle of the IFigure from which the current
%    callback originated. Currently, this is always the same as IGCF.
%    In fact, I am not sure if it can differ from that, even in the 
%    native equivalents.
	 
id = ip_send('gcbf', 0);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

