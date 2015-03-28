function h = isubplot(nrows, ncols, irow, icol)
% ISUBPLOT - Create a new subplot or switch to an existing one
%    ISUBPLOT(rows, cols, n) defines the number of rows and columns and 
%    switches to subplot number N (counted from 1).
%    ISUBPLOT(n) just switches to subplot number N.
%    ISUBPLOT(rows, cols, r, c) specifies a row and column rather than a 
%    raw subplot number. R and C count from 1.
%    ISUBPLOT(r, c) does not change the layout.
%    h = ISUBPLOT(...) returns the subplot handle

switch nargin
  case 1
    id = ip_send('subplot', [nrows]);
  case 2
    id = ip_send('subplot', [nrows ncols]);
  case 3
    id = ip_send('subplot', [nrows ncols irow]);
  case 4
    id = ip_send('subplot', [nrows ncols irow icol]);
  default
    error('ISUBPLOT takes 1 to 4 arguments');
end

[cmd, h] = ip_receive;
ip_error(cmd, h);

if nargout<1
  clear h
end
