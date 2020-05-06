-- rumur_flags: self.config['SMT_BV_ARGS']
-- skip_reason: 'no SMT solver available' if self.config['SMT_BV_ARGS'] is None else None

-- test that the SMT bridge can cope with addition when using a bitvector logic

var
  x: 0 .. 1;
  y: boolean;

startstate begin
  y := true;
end;

rule begin
  -- the following condition should be simplified to `true` avoiding a read of
  -- `x` when it is undefined
  if x + 1 = 1 | x + 1 = 2 then
    y := !y;
  end;
end;
