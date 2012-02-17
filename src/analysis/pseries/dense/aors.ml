open Base

(****** add ******)

(* C = n *)
let rec add a b =
  if !?a && !?b
  then lazv( _add ( !!a, !!b ) )
  else lazy( _add ( !!a, !!b ) )

and _add = function
| a,E -> a
| E,b -> b
| N( ha,ta ),N( hb, tb ) ->
    N( ha +/ hb, add ta tb )

let ( +: ) a b = add a b


(****** neg & sub ******)

(* C = n *)
let  neg a =  map ( ~/ ) a
let _neg a = _map ( ~/ ) a

let ( ~: ) a = neg a

(* C = n *)
let rec sub a b =
  if !?a && !?b
  then lazv( _sub ( !!a, !!b ) )
  else lazy( _sub ( !!a, !!b ) )

and _sub = function
| a,E -> a
| E,b -> _neg b
| N( ha,ta ),N( hb, tb ) ->
    N( ha -/ hb, sub ta tb )

let ( -: ) = sub

