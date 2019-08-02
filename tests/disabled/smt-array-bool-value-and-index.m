-- rumur_flags: ['--smt-simplification', 'on'] + smt_args()
-- skip_reason: 'no SMT solver available' if len(smt_args()) == 0 else None

/* test whether the SMT bridge can simplify expressions involving arrays with
 * bool values and indicies
 */

type
  t: array[boolean] of boolean;

var
  x: t;
  y: boolean;

startstate begin
  y := true;
end;

rule begin
  /* if the SMT bridge is working correctly, it will simplify the condition in
   * the following expression to `true` avoiding the read of an undefined
   * variable
   */
  if x[true] = x[true] then
    y := !y;
  end;
end;