-- rumur_flags: config['SMT_ARGS']
-- skip_reason: 'no SMT solver available' if config['SMT_ARGS'] is None else None

-- test that the SMT bridge can deal with integer range types

var
  x: 2 .. 10;
  y: boolean;

startstate begin
  y := true;
end;

rule begin
  -- if the SMT bridge can deal with integer ranges, it should turn the
  -- following condition into `true`, avoiding a read of x which is undefined
  if x > 1 then
    y := !y;
  end;
end;
