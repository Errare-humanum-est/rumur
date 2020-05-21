-- rumur_flags: self.config['SMT_BV_ARGS']
-- skip_reason: 'no SMT solver available' if self.config['SMT_BV_ARGS'] is None else None

-- equivalent of smt-exists4.m but using --smt-bitvectors on

var
  x: boolean;
  y: boolean;

startstate begin
  x := true;
end;

rule begin
  -- if the SMT bridge is working correctly, it should simplify the condition as
  -- a tautology into true, avoiding the read of an undefined variable
  if y | !exists z := 2 to 4 by 2 do z = 3 end then
    x := !x;
  end;
end;
