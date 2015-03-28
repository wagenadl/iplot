function kk = iproperties(h)
% IPROPERTIES - Return a list of all properties for an IObject
%   kk = IPROPERTIES(h) returns a list of all the properties that are
%   defined or may be defined on the IObject H, including user-defined
%   properties.

id = ip_send('properties', h);
[cmd, h, k, val] = ip_receive(id);
ip_error(cmd, h);

kk = strsplit(val);
