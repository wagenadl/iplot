function h = ifind(h0, tag)
% IFIND - Finds an IObject by tag
%   h = IFIND(tag) returns the handle of the (first) IObject with the
%   given TAG, or [] if none.
%   h = IFIND(h0, tag) only searches H0 and its descendants.
%   Note that at most one handle is presently returned. This is a likely
%   target for future improvement: It would be make sense to return all
%   matching handles.

if nargin<2
  tag = h0;
  h0 = 0;
end

id = ip_send('find', h0, tag);
[cmd, h] = ip_receive(id);
ip_error(cmd, h);

if h==0
  h=[];
end
