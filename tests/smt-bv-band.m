-- rumur_flags: self.config['SMT_BV_ARGS']
-- skip_reason: 'no SMT solver available' if self.config['SMT_BV_ARGS'] is None else None

-- test that the SMT bridge can simplify bitwise AND when using bitvector logic

var
  x: 1 .. 2
  y: boolean

startstate begin
  y := true;
end

rule begin
  -- if the SMT bridge can handle bitwise AND, it should simplify the following
  -- to `true`, avoiding an undefined read
  if (x & 1) = 1 | x = 2 then
    y := !y;
  end;
end
