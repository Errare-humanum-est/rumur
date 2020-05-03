-- rumur_flags: config['SMT_ARGS']
-- skip_reason: 'no SMT solver available' if config['SMT_ARGS'] is None else None

-- test that the SMT bridge can cope with division

var
  x: 4 .. 6;
  y: boolean;

startstate begin
  y := true;
end;

rule begin
  -- the following condition should be simplified to `true` avoiding a read of
  -- `x` when it is undefined
  if x / 2 = 2 | x / 2 = 3 then
    y := !y;
  end;
end;
