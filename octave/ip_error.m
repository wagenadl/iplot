function ip_error(cmd, h, key)
% IP_ERRROR - Internal function to report error implied in an IPlot response
%   IP_ERROR(cmd, h, key) checks whether CMD represents an error condition,
%   and if so, creates an octave-level ERROR.

if nargin<3
  key = '';
end
if nargin<2
  h = 0;
end

switch cmd
  case 'badcommand'
    error('IPlot: Unknown command');
  case 'badhandle'
    error('IPlot: Unknown handle');
  case 'badkey'
    error('IPlot: Unknown key');
  case 'badvalue'
    error('IPlot: Illegal value');
  case 'badsyntax'
    error('IPlot: Syntax error');
  case 'unknown'
    error('IPlot: Unknown response');
end
