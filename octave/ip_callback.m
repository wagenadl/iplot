function ip_callback(h, key, arg)
% IP_CALLBACK - Internal function that implements callbacks
%    IP_CALLBACK(h, key, arg) calls any callback function installed on 
%    object H for function KEY, with second argument ARG.

foo = icallback(h, key);
if ~isempty(foo)
  foo(h, arg);
end
