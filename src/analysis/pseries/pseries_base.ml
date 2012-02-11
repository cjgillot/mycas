type expr = Expr.expr

open Lazy

let ( !! ) = force
let ( !? ) = lazy_is_val

let lazv = lazy_from_val

(* series type *)
type 'a llist = 'a node lazy_t
and 'a node =
| E | N of 'a * 'a llist

type _series = expr node
type  series = expr llist

(* operators aliases *)
let ( ~/ ) = Expr.neg
let ( +/ ) = Expr.add
let ( -/ ) = Expr.sub
let ( */ ) = Expr.mul
let ( // ) = Expr.div

let _ex0 = Expr.of_int 0

(* truncation hack *)
let lErep = Obj.repr (lazv E)
let rec force_truncate s = match s with
| E -> ()
| N( h, t ) ->
    if !? t then force_truncate !!t;
    let r = Obj.repr s in
    Obj.set_field r 1 lErep;
    ()

(****** fast construction ******)
let rec of_list = function
| [] -> lazv E
| a::r -> lazv( N( a, of_list r ) )

module A1 = Bigarray.Array1

let of_bigarray a =
  let n = A1.dim a in
  let ret = ref E in
  for i = n-1 downto 0 do
    ret := N( A1.unsafe_get a i, lazv !ret )
  done;
  ret

let alone a = lazv (N(a, lazv E))

let of_int n = alone( Expr.of_int n )

let zero = lazv E
let one = of_int 1

let rec null_p s =
  if not( !? s ) then false else
  match !!s with
  | E -> true | N( h, t ) ->
    Expr.null_p h && null_p t

let unit_p s =
  if not( !? s ) then false else
  match !!s with
  | E -> false | N( h, t ) ->
    Expr.unit_p h && null_p t

let equal _ _ = failwith "Not Implementable"
let compare _ _ = failwith "Not Implementable"

(*let of_int n = alone (M.of_int n)*)

let shift a = lazv( N( _ex0, a ) )

let rec translate n a =
  if n == 0
  then a
  else translate (n-1) (shift a)

(****** utils ******)
let rec map f s =
  if !? s
  then lazv( _map f !!s )
  else lazy( _map f !!s )
and _map f = function
| E -> E
| N( h, t ) ->
    N( f h, map f t )

let rec iter f s =
  if !? s then match !!s with
  | E -> ()
  | N( h, t ) ->
      f h; iter f t

let rec compare a b =
  if !? a && !? b
  then begin match !!a,!!b with
    | E,E -> 0
    | E,_ -> -1
    | _,E -> 1
    | N( h1, t1 ), N( h2, t2 ) ->
        let c = Expr.compare h1 h2 in
        if c != 0 then c else
        compare t1 t2
  end
  else (* signal unsuccessful comparison *)
    2

(* C = n *)
let rec mmul_l m l = map (fun x -> m */ x) l
let rec mmul_r l m = map (fun x -> x */ m) l
let rec mdiv_r l m = map (fun x -> x // m) l
