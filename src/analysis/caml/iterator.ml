open Expr

type 'a iterator = private unit

external copy   : 'a iterator -> 'a iterator = "__caml_iterator_copy"
external advance: 'a iterator -> int -> unit = "__caml_iterator_advance" "noalloc"
external deref  : 'a iterator -> 'a          = "__caml_iterator_deref"

let incr it = advance it   1
let decr it = advance it (-1)

let iter b e f =
  let it = copy b in
  while it <> e do
    f (deref it);
    incr it
  done
