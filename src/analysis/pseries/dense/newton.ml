open Base

(*
  Newton iteration :

  Given F a function over power series,
  compute the series defined by
    S_{n+1} = F( S_n )

  Parameters :
    F : 'a llist -> 'a llist
    S_0 : 'a llist
*)

module Flatten = struct
  (*
    Here is the code of the lazy flattening.
    The function segc drops [ndrop] elements from a list [u] and concatenates the following [ntake] items with [rest].

    flatn ((s0 : _) : v) = s0 : aux 1 v where
      aux nd (u0 : u) = segc u nd nd (aux m u)
        where m = 2 * nd
      segc u@(u0 : u') ndrop ntake rst
      | ndrop > 0 = segc u' (ndrop-1) ntake rst
      | ntake > 0 = u0 : segc u' ndrop (ntake-1) rst
      | otherwise = rst
  *)

  (* flatn: 'a llist llist -> 'a llist *)
  (* segc: 'a llist -> int -> int -> 'a llist lazy_t -> 'a llist *)
  (* aux: int -> 'a llist -> 'a llist *)

  (*
    Extract segment [ndrop, ndrop + ntake) within [u], and append [rest]
  *)
  (* C = ndrop + ntake *)
  let rec segc u ndrop ntake rest =
    if ndrop == 0 then concat u ntake rest else
    match u with
    | E -> E
    | N( _, t ) ->
        segc !!t (ndrop - 1) ntake rest

  and concat u ntake rest =
    match u with
    | E -> E
    | N( h, t ) ->
        if ntake == 1
        then
          (* is this _really_ needed ? *)
          let () = force_truncate u in
          N( h, rest )
        else
          N( h, lazy( concat !!t (ntake - 1) rest ) )

  (* output a list of length n *)
  (* C = n *)
  let _flatten = function
  | E -> E

  | N(lazy E,_) -> E

  | N(lazy( N(l00,_) ), v) ->
      let rec aux nd = function
      | E -> E
      | N(lazy u0, u') ->
          segc u0 nd nd (lazy(aux (2 * nd) !!u'))
      in
      N(l00, lazy(aux 1 !!v) )

  (* returns a llist of l llists of length n *)
  (* C = l F(n) *)
  let rec iterate f start =
    N( start, lazy( iterate f (f start) ) )

  (* C = N(F) = F(n) if F is regular enough *)
  let _newton fnc start ns =
    _flatten (iterate fnc start)
  let newton fnc start ns =
    lazy( _newton fnc start ns )
end

module SelfRec = struct
  let rec ndrop n u =
    if n == 0 then !!u else
    match !!u with
    | E -> E
    | N( h, t ) ->
        ndrop (n-1) t

  let rec concat n u rest =
    match u with
    | E -> E
    | N( h, t ) ->
        if n == 0
        then !!rest
        else N( h, lazy( concat (n-1) !!t rest ) )

  (* C = N(F) = F(n) if F is regular enough *)
  let _newton fnc start ns = match !!start with
  | E -> E
  | N(h, t) ->
      let rec ret = N(h, lazy(
        let lret = lazv ret in
        let tail = lazy( ndrop ns (fnc lret) ) in
        concat (ns-1) !!t tail
      )) in
      ret
  let newton fnc start ns =
    lazy( _newton fnc start ns )
end

(* TODO : needs testing *)

include SelfRec
