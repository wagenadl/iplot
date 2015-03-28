function iwait(h0, timeout_s)
% IWAIT - Wait until an IFigure is closed or IWAKE is called
%    IWAIT(h) waits until the IFigure with handle H is closed, or IWAKE
%    is called.
%    IWAIT(h, timeout_s) aborts the wait after the given number of seconds.
%    IWAIT without arguments is the same as IWAIT(igcf).

if nargin<1
  h0 = igcf;
end

if ifigure(h0)==0
  return;
end

if nargin<2
  timeout_s = 0;
end

global iplot_wake
iplot_wake = 0;

while ~iplot_wake
  [id, cmd, h, arg1, arg2] = ip_recv(1e3*timeout_s);
  if id==0
    error('IPlot: No response. Server died?');
  elseif strcmp(cmd, 'callback')
    if strcmp(arg1, 'deletefcn') && h==h0
      iplot_wake = 1;
    else
      ip_callback(h, arg1, arg2);
    end
  else
    fprintf(2, '%s (%i, %i)\n', cmd, id, h);
    error('IPlot: Unexpected response while waiting');
  end
end
