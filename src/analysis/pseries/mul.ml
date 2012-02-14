open Pseries_base

module Naive = struct
  open Aors

  let rec _mul (fs,gs) = match fs,gs with
  | E,_ | _,E -> E
  | N( f0,ft ),N( g0, gt ) ->
      let f00 = Expr.null_p f0
      and g00 = Expr.null_p g0 in

           if f00 && g00 then N( _ex0, shift( mul ft gt ) )
      else if f00        then N( _ex0, lazy( _mul (!!ft,gs) ) )
      else if g00        then N( _ex0, lazy( _mul (fs,!!gt) ) )
      else
      N( f0 */ g0,
          (mmul_l f0 gt)
      +:  (mmul_r ft g0)
      +:  (shift (mul ft gt))
      )

  and mul f g =
    lazy( _mul (!!f, !!g) )

  let rec _square = function
  | E -> E
  | N( f0, ft ) ->
      if Expr.null_p f0
      then N( _ex0, lazv(N( _ex0, square ft )) )
      else N( f0 */ f0,
        let sca = mmul_l f0 ft in
          sca +: sca +: (shift (square ft))
      )

  and square f = lazy( _square !!f )

end

include Naive

let ( *: ) a b = mul a b
