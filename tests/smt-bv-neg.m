-- rumur_flags: SMT_BV_ARGS
-- skip_reason: 'no SMT solver available' if len(SMT_BV_ARGS) == 0 else None

-- test that the SMT bridge can cope with negation when using a bitvector logic

var
  x: 8 .. 9;
  y: boolean;

startstate begin
  y := true;
end;

rule begin
  -- the following condition should be simplified to `true` avoiding a read of
  -- `x` when it is undefined
  if -x = -8 | -x = -9 then
    y := !y;
  end;
end;
