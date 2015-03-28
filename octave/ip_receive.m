function [cmd, h, arg1, arg2] = ip_receive(id)
% IP_RECEIVE - Wait for IPlot responses until one is received with a given ID
%   [cmd, h, arg1, arg2] = IP_RECEIVE(id) waits for IPlot responses and handles
%   callbacks until a response is received with sequence id ID.

global iplot_callbackqueue
global iplot_nestingdepth

if isempty(iplot_nestingdepth)
  iplot_nestingdepth=0;
end

iplot_nestingdepth = iplot_nestingdepth + 1;

unwind_protect
  while 1
    [i1, cmd, h, arg1, arg2] = ip_recv;
    if i1==id
      return;
    end
    if i1==0
      error('IPlot: No response. Server died?');
    end
    if i1>id
      error('IPlot: Skipped my ID. Strange.');
    end
    if strcmp(cmd, 'callback')
      iplot_callbackqueue{end+1} = [h, arg1, arg2];
    end
    
    if iplot_nestingdepth==1
      while ~isempty(iplot_callbackqueue)
        doqueue = iplot_callbackqueue;
        iplot_callbackqueue = {};
        for n=1:length(doqueue)
  	ip_callback(doqueue{n}(1), doqueue{n}(2), doqueue{n}(3));
        end
      end      
    end
  end
unwind_protect_cleanup
  iplot_nestingdepth = iplot_nestingdepth - 1;
end

% The crazy iplot_nestingdepth trick is essential, because callbacks can
% themselves call ip_receive, and in so doing elicit further callbacks.
% We want to handle all callbacks in the order received, but we don't want
% to exhaust our stack. So we will not honor any callbacks when called from
% within a callback. Instead, we just add the callback to a list that gets
% emptied when we get back to top level.
% The tricky part is to make sure that iplot_nestingdepth gets restored
% upon exit, even in case of errors. Hence the catch/rethrow.