type expr = Expr.expr

val ( !! ) : 'a Lazy.t -> 'a
val ( !? ) : 'a Lazy.t -> bool

val lazv : 'a -> 'a Lazy.t

type 'a llist = 'a node lazy_t
and 'a node = E | N of 'a * 'a llist

type _series = expr node
type  series = expr llist

val ( ~/ ) : expr -> expr
val ( +/ ) : expr -> expr -> expr
val ( -/ ) : expr -> expr -> expr
val ( */ ) : expr -> expr -> expr
val ( // ) : expr -> expr -> expr

val _ex0 : expr

val force_truncate : 'a node -> unit

val of_list : 'a list -> 'a llist
val alone : 'a -> 'a llist

val of_int : int -> series

val zero : series
val one : series

val null_p : series -> bool
val unit_p : series -> bool

val shift : series -> series
val translate : int -> series -> series

val map  : ('a -> 'b) -> 'a llist -> 'b llist
val _map : ('a -> 'b) -> 'a node -> 'b node

val iter : ('a -> unit) -> 'a llist -> unit

val compare : series -> series -> int

val mmul_l : expr -> series -> series
val mmul_r : series -> expr -> series
val mdiv_r : series -> expr -> series
