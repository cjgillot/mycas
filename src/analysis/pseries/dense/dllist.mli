type 'a t

exception Empty
val create : 'a -> 'a t
val length : 'a t -> int
val add : 'a t -> 'a -> unit
val append : 'a t -> 'a -> 'a t
val prepend : 'a t -> 'a -> 'a t
val promote : 'a t -> unit
val demote : 'a t -> unit
val remove : 'a t -> unit
val drop : 'a t -> 'a t
val rev_drop : 'a t -> 'a t
val splice : 'a t -> 'a t -> unit
val set : 'a t -> 'a -> unit
val get : 'a t -> 'a
val next : 'a t -> 'a t
val prev : 'a t -> 'a t
val skip : 'a t -> int -> 'a t
val rev : 'a t -> unit
val iter : ('a -> unit) -> 'a t -> unit
val for_all : ('a -> bool) -> 'a t -> bool
val find : ('a -> bool) -> 'a t -> 'a t
val exists : ('a -> bool) -> 'a t -> bool
val fold_left : ('a -> 'b -> 'a) -> 'a -> 'b t -> 'a
val fold_right : ('a -> 'b -> 'b) -> 'a t -> 'b -> 'b
val map : ('a -> 'b) -> 'a t -> 'b t
val copy : 'a t -> 'a t
val to_list : 'a t -> 'a list
val of_list : 'a list -> 'a t
