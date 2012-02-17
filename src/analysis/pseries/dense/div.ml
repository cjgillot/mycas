open Base

open Aors
open Mul

module LongDiv = struct

	(* C = M(n) *)
	(*
	F = f+xF' = QG = (q+xQ')(g+xG')
	=> f = qg and F' = (q+xQ')G' + Q'g = QG' + Q'g
	=> Q = q+xQ' = f /g + x(1/g)(F' − QG')

	 (f:ft) / (g:gt) = qs where qs = f/g : 1/g*(ft-qs*gt)
	*)
	let rec div fs gs =
	  let fv = !?fs and gv = !?gs in
	  if fv && !!fs == E then lazv E else
	  if gv && !!gs == E then raise Division_by_zero else
	  if fv && gv
	  then lazv( _div ( !!fs, !!gs ) )
	  else lazy( _div ( !!fs, !!gs ) )

	and _div = function
	| E, _ -> E
	| _, E -> raise Division_by_zero

	| N(f,ft), N(g,gt) when Expr.null_p g ->
      if Expr.null_p f
      then
        (* XXX may break stream requirements !!! *)
        _div (!!ft, !!gt)
      else
        (* laurent series *)
        raise Division_by_zero

  | N(f,ft),N(g,gt) when Expr.unit_p g ->
      let rec qs = N(f, lazy(
        _sub ( !!ft, _mul ( qs, !!gt ) )
      ))
      in qs

  | N(f,ft),N(g,gt) ->
      let rec qs = N(f // g, lazy(
        let rest = _sub( !!ft, _mul ( qs, !!gt) ) in
        _map (fun x -> x // g) rest
      ))
      in qs

	let ( /: ) = div

	(* C = M(n) *)
	let _inv = function
	| E ->
      raise Division_by_zero

	| N(g,gt) when Expr.null_p g ->
	    raise Division_by_zero

  | N(g,gt) when Expr.unit_p g ->
      let rec qs = N( g, lazy(
        _neg (_mul (qs, !!gt))
      ))
      in qs

  | N(g,gt) ->
      let rec qs = N( Expr.inv g, lazy(
        let rest = _mul (qs, !!gt) in
        _map (fun x -> ~/ x // g) rest
      ))
      in qs

	let inv fs =
	  if !?fs
	  then lazv( _inv !!fs )
	  else lazy( _inv !!fs )

end

module IterNewton = struct

  open Newton

  let _s2 = of_int 2
  let fnc f s = s *: ( _s2 -: ( f *: s) )

  (* F(S) = S * ( 2 - f * S ) *)
  let _inv f =
    match f with
    | E -> raise Division_by_zero
    | N(f0,ft) ->
        let s0 = alone (Expr.inv f0) in
        _newton (fnc (lazv f)) s0

  let inv f =
    if !? f
    then lazv( _inv !!f )
    else lazy( _inv !!f )

  let _div (a,b) = _mul (a, _inv b)
  let div fs gs =
	  let fv = !?fs and gv = !?gs in
	  if fv && !!fs == E then lazv E else
	  if gv && !!gs == E then raise Division_by_zero else
	  if fv && gv
	  then lazv( _div ( !!fs, !!gs ) )
	  else lazy( _div ( !!fs, !!gs ) )

end

include LongDiv

let ( /: ) a b = div a b
