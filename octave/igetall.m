function kv = igetall(h)
% IGETALL - Returns all properties of an IObject
%    kv = IGETALL(h) returns a structure in which the elements are filled
%    with all the properties of an IObject.
%    Because structure members with names starting with '*' are hard to
%    access in Octave, user properties result in structure member names
%    starting with '_' instead.

kk = iproperties(h);
kv = struct;
length(kk)
for k=1:length(kk)
  key = kk{k};
  if key(1)=='*'
    key(1) = '_';
  end
  k
  kv.(key) = iget(h, kk{k});
  k
end
