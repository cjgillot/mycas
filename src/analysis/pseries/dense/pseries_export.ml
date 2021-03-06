open Base

external call_map_closure  : 'a -> expr -> expr = "caml_call_map_closure"

let map f a = Base.map (call_map_closure f) a

let force s = Lazy.force s
let is_forced s = Lazy.lazy_is_val s
