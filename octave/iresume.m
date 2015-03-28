function iresume
% IRESUME - Wake up a pending IWAIT
%   IRESUME makes any currently executing IWAIT return immediately.
%   At present, IWAKE is not selective for a particular figure.

global iplot_wake
iplot_wake = 1;
