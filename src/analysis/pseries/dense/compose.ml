open Base

exception Bad_composition

open Mul
open Div

module Horner = struct

  let rec compose a b =
    lazy( _compose (!!a,b) )

  and _compose = function
  | E, _ -> E
  | N( f0, _ ), lazy E -> N( f0, zero )
  | _, lazy(N( g0, _ )) when not (Expr.null_p g0) ->
      raise Division_by_zero
  | N( f0, ft ), g ->
      N( f0, g *: (compose ft g) )

end

(*
module BrentKung = struct

  (* see : http://algo.inria.fr/bostan/publications/Bostan10.pdf *)

  let _slice m = function
	| E -> E,lazv E
	| N( h, t ) ->
	    if m = 1
	    then (alone h),t else
      let p1,p2 = _slice (m-1) !!t in
      N( h, lazv p1 ), p2

  (*
    F( G1 + X^m G2 ) = F(G1) + F'(G1) X^m G2 + F''(G1)/2 X^2m G2^2 + ...

    computed using F'(G)=F(G)'/G'
   *)
  let next_comp fg g =
    (* return F'(G)=F(G)'/G' *)
    (diff fg)/(diff g)

  let rec do_comp m k fg g =
    let next () = ( (diff fg) /: (diff g) ) *: g
    and ex_k = Expr.of_int k in

    fg +: translate m (lazy(
      let ret = mdiv_r (next ()) (k+1) in
      !!ret
    ))

  (* requires G(0) != 0 and G'(0) != 0 *)
  let compose_bk f g =
    (* TODO choose m, best is sqrt( n/lg(n) ) *)
    let m = 1 in
    let g1 = _slice m g in
    let fg1 = Horner._compose f g1 in
    do_comp m 1 fg1 g

end
*)

include Horner
