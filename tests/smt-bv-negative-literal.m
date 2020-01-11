-- rumur_flags: SMT_BV_ARGS
-- skip_reason: 'no SMT solver available' if len(SMT_BV_ARGS) == 0 else None

-- similar to smt-negative-literal, but for --smt-bitvectors on

const
  N: -10;

var
  x: boolean;
  y: boolean;

startstate begin
  y := true;
end;

rule begin
  if x | !x then
    y := !y;
  end;
end;
