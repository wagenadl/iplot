function [id, cmd, h, arg1, arg2] = ip_recv(timeout_ms)
% IP_RECV - Receive a single IPlot response and return its contents

if nargin<1 || timeout_ms<=0
  [id, cmd, h, arg1, arg2] = ip_internal;
else
  [id, cmd, h, arg1, arg2] = ip_internal(timeout_ms);
end
