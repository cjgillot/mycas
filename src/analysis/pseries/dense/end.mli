open Expr
open Base

val _intpow : expr node -> int -> expr node
val intpow : series -> int -> series

val _diff : expr node -> expr node
val diff : series -> series

val integrate0 :
  Expr.expr node Lazy.t Lazy.t ->
  Expr.expr node Lazy.t
val _integrate0 :
  Expr.expr node Lazy.t -> Expr.expr node

val _integrate :
  Expr.expr ->
  Expr.expr node Lazy.t Lazy.t -> Expr.expr node
val integrate :
  Expr.expr ->
  Expr.expr node Lazy.t Lazy.t ->
  Expr.expr node Lazy.t

val newton :
  ('a node lazy_t -> 'a node lazy_t) ->
  'a node lazy_t -> 'a node lazy_t
