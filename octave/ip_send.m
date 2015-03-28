function id = ip_send(cmd, h, arg1, arg2)
% IP_SEND - Send a single IPlot command and return its sequence ID

if nargin<2
  h = 0;
end

if nargin>=4
  id = ip_internal(cmd, h, arg1, arg2);
elseif nargin>=3
  id = ip_internal(cmd, h, arg1);  
elseif nargin>=1
  id = ip_internal(cmd, h);  
else
  error('ip_send needs at least a command');
end

