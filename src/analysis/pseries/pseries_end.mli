val _intpow_h :
  Pseries_base.expr Pseries_base.node ->
  Pseries_base.expr Pseries_base.node ->
  int -> Pseries_base.expr Pseries_base.node
val intpow :
  Pseries_base.expr Pseries_base.node Lazy.t ->
  int -> Pseries_base.expr Pseries_base.node Lazy.t
val _intpow :
  Pseries_base.expr Pseries_base.node ->
  int -> Pseries_base.expr Pseries_base.node
val diff_h :
  int ->
  Expr.expr Pseries_base.node Lazy.t -> Expr.expr Pseries_base.node Lazy.t
val _diff_h :
  int -> Expr.expr Pseries_base.node -> Expr.expr Pseries_base.node
val diff :
  Expr.expr Pseries_base.node Lazy.t -> Expr.expr Pseries_base.node Lazy.t
val _diff : Expr.expr Pseries_base.node -> Expr.expr Pseries_base.node
val integrate_h :
  int ->
  Expr.expr Pseries_base.node Lazy.t -> Expr.expr Pseries_base.node Lazy.t
val _integrate_h :
  int -> Expr.expr Pseries_base.node -> Expr.expr Pseries_base.node
val integrate0 :
  Expr.expr Pseries_base.node Lazy.t Lazy.t ->
  Expr.expr Pseries_base.node Lazy.t
val _integrate0 :
  Expr.expr Pseries_base.node Lazy.t -> Expr.expr Pseries_base.node
val _integrate :
  Expr.expr ->
  Expr.expr Pseries_base.node Lazy.t Lazy.t -> Expr.expr Pseries_base.node
val integrate :
  Expr.expr ->
  Expr.expr Pseries_base.node Lazy.t Lazy.t ->
  Expr.expr Pseries_base.node Lazy.t
val newton :
  ('a Pseries_base.node lazy_t -> 'a Pseries_base.node lazy_t) ->
  'a Pseries_base.node lazy_t -> 'a Pseries_base.node lazy_t
