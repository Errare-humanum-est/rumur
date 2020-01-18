-- rumur_flags: SMT_BV_ARGS
-- skip_reason: 'no SMT solver available' if len(SMT_BV_ARGS) == 0 else None

-- equivalent of smt-forall2.m but using --smt-bitvectors on

type
  t: 1 .. 2;

var
  x: boolean;
  y: boolean;

startstate begin
  x := true;
end;

rule begin
  -- if the SMT bridge is working correctly, it should simplify the condition as
  -- a tautology into true, avoiding the read of an undefined variable
  if forall z: t do z = 1 | z = 2 end | y then
    x := !x;
  end;
end;
