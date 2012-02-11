open Pseries_base

external call_map_closure  : 'a -> expr -> expr = "caml_call_map_closure"

let make () = lazv E

let empty s = match !!s with
| E -> true | _ -> false

let null a  = null_p a
let unit a  = unit_p a

let map f a = map  (call_map_closure f) a

let add a b = Aors.add a b
let sub a b = Aors.sub a b
let neg a   = Aors.neg a

let mul a b = Mul.mul a b
let div a b = Div.div a b
let inv a   = Div.inv a

let diff s = Pseries_end.diff s

let compare a b = Pseries_base.compare a b

(* for iterator *)
let iterator_force  s = !!s
let iterator_forced s = !?s
