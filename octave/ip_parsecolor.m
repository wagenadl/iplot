function cc = ip_parsecolor(c)
% IP_PARSECOLOR - Parses a single character color definition
%    cc = IP_PARSECOLOR(c) where C is one of rgbcmykw returns the 
%    corresponding RGB triplet.

if nargin<1 || isempty(c)
  error('Illegal color');
end

idx = find('krgybmcw' == c);
if isempty(idx)
  error('Illegal color');
else
  idx = idx - 1;
  cc = [bitand(idx,1), bitand(idx,2)/2, bitand(idx,4)/4];
end


