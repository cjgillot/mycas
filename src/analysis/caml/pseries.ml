type expr = Expr.expr

open Lazy

let ( !! ) = force
let ( !? ) = lazy_is_val

let lazv = lazy_from_val

(* series type *)
type 'a series = 'a node lazy_t
and 'a node =
| E | N of 'a * 'a series

(* operators aliases *)
let ( ~/ ) = Expr.neg
let ( +/ ) = Expr.add
let ( -/ ) = Expr.sub
let ( */ ) = Expr.mul
let ( // ) = Expr.div

let _ex0 = Expr.of_int 0

(****** utils ******)
let rec map f s =
  if !? s
  then lazv( _map f !!s )
  else lazy( _map f !!s )
and _map f = function
| E -> E
| N( h, t ) ->
    N( f h, map f t )

(****** fast construction ******)
let rec of_list = function
| [] -> lazv E
| a::r -> lazv( N( a, of_list r ) )

let alone a = N(a, lazv E)

let of_int n = alone( Expr.of_int n )

let zero = E
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

(****** display ******)
(*let print p =
  let rec loop = function
    E -> ()
  | N(m, r) -> M.print m; print_string " ; "; loop (force r)
  in
  print_string "[ "; loop p; print_string "]"
;;

let printn nb p =
  let rec loop n = function
    E -> ()
  | N(m, r) -> if n > 0 then ( M.print m; print_string " ; "; loop (pred n) (force r) )
  in
  print_string "[ "; loop nb p; print_string "]"
;;*)

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

let ( +: ) = add


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


(****** mul ******)

(* C = n *)
let rec mmul_l m l = map (fun x -> m */ x) l
let rec mmul_r l m = map (fun x -> x */ m) l

(* C = M(n) = n^2 *)
module Mul = struct

  module DL = Dllist

  type mul_rep = {
    (* points to first *)
    mutable f: expr DL.t;
    mutable fs: expr series option;

    (* points to last *)
    mutable g: expr DL.t;
    mutable gs: expr series option

    (* invariant : |f| = |g| *)
  }

  let empty r =
    r.fs == None && r.gs == None

  let make_rep fs gs =
    let N( fh, ft ) = fs
    and N( gh, gt ) = gs in
    let ret = {
      f=DL.create fh;
      fs=Some ft;

      g=DL.create gh;
      gs=Some gt
    }
    in ret

  let incr_rep r =
    let () = match r.fs with
    | None ->
        (* add at end of list *)
        ignore( DL.prepend r.f _ex0 )

    | Some( lazy E ) ->
        ignore( DL.prepend r.f _ex0 );
        r.fs <- None

    | Some( lazy ( N( h,t ) ) ) ->
        ignore( DL.prepend r.f h );
        r.fs <- Some t

    and () = match r.gs with
    | None ->
        (* add at end of list *)
        r.g <- DL.append r.g _ex0;
        ()

    | Some( lazy E ) ->
        r.g <- DL.append r.g _ex0;
        r.gs <- None

    | Some( lazy ( N( h,t ) ) ) ->
        r.g <- DL.append r.g h;
        r.gs <- Some t
    in
    ()

  let get_ex r =
    let ret = ref( DL.get r.f */ DL.get r.g ) in
    let rec loop f g =
      if f == r.f then () else
      let () =
        ret := !ret +/ ( DL.get f */ DL.get g )
      in
      loop (DL.next f) (DL.prev g)
    in
    loop (DL.next r.f) (DL.prev r.g);
    !ret

  (* use simple convolution rule *)
  let rec mul_with_rep r =
    if empty r then E else
    let ex = get_ex r in
    N( ex, lazy(
      incr_rep r;
      mul_with_rep r
    ))

  let _mul ( fs, gs ) =
    let r = make_rep fs gs in
    mul_with_rep r

  let mul fs gs =
    if ( !? fs && !!fs == E ) || ( !? gs && !!gs == E )
    then lazv E
    else lazy( _mul ( !!fs, !!gs ) )

  (* C = n^2 = M(n) *)
  let square a = mul a a
  let _square a = _mul (a, a)
end

let mul a b = Mul.mul a b
let _mul ( a,b ) = Mul._mul ( a,b )
let ( *: ) a b = Mul.mul a b

let square a = Mul.square a
let _square a = Mul._square a

(****** div & inv ******)

(* C = M(n) *)
(*
F = f+xF' = QG = (q+xQ')(g+xG')
=> f = qg and F' = (q+xQ')G' + Q'g = QG' + Q'g
=> Q = q+xQ' = f /g + x(1/g)(F' − QG')

 (f:ft) / (g:gt) = qs where qs = f/g : 1/g*(ft-qs*gt)
*)
let rec div fs gs =
  let fv = !?fs and gv = !?gs in
  if fv && !!fs == E then lazv E else
  if gv && !!gs == E then raise Division_by_zero else
  if fv && gv
  then lazv( _div ( !!fs, !!gs ) )
  else lazy( _div ( !!fs, !!gs ) )

and _div = function
| E, _ -> E
| _, E -> raise Division_by_zero

| N(f,ft), N(g,gt) ->
    if Expr.null_p g then

      if Expr.null_p f
      then
        (* XXX may break stream requirements !!! *)
        _div (!!ft, !!gt)
      else
        (* laurent series *)
        raise Division_by_zero

    else if Expr.unit_p g then

      let rec qs = N(f, lazy(
        _sub ( !!ft, _mul ( qs, !!gt ) )
      ))
      in qs

    else

      let rec qs = N(f // g, lazy(
        let rest = _sub( !!ft, _mul ( qs, !!gt) ) in
        _map (fun x -> x // g) rest
      ))
      in qs

let ( /: ) = div

(* C = M(n) *)
let rec inv fs =
  if !?fs
  then
    if !!fs == E
    then raise Division_by_zero
    else lazv( _inv !!fs )
  else
    lazy( _inv !!fs )

and _inv = function
| E -> raise Division_by_zero

| N(g,gt) ->
    if Expr.null_p g

    then raise Division_by_zero

    else if Expr.unit_p g then

      let rec qs = N( g, lazy(
        _neg (_mul (qs, !!gt))
      ))
      in qs

    else

      let rec qs = N( Expr.inv g, lazy(
        let rest = _mul (qs, !!gt) in
        _map (fun x -> ~/ x // g) rest
      ))
      in qs

(****** intpow ******)

(* binary powering *)
let rec _intpow_h c p e =
  if e = 0 then c else
  if e = 1 then _mul (c, p) else
  if e mod 2 = 0
  then _intpow_h  c            (_square p) (e asr 1)
  else _intpow_h (_mul (c, p)) (_square p) (e asr 1)

(* C = M(n) lg e, e > 0 *)
let rec intpow p e =
  if !? p
  then lazv( _intpow !!p e )
  else lazy( _intpow !!p e )

and _intpow p e = match p with
| E -> E
| N( h,t ) ->
    if Expr.null_p h
    then N( _ex0, translate (e - 1) (intpow t e) )
    else _intpow_h one p e


(******************** COMPOSE ***********************)

(*
(* Horner evalutation adapted to composition *)

(* C = C(n) = n M(n) *)
(* (f:ft) @ gs=(0:gt) = f : gt*(ft@gs)
   (f:ft) @ gs=(g:gt) = [f] + gs*(ft@gs)
 *)
let rec ( @ ) fs gs = match fs, gs with
  E, _         -> E
| N(f,_), E -> alone f

| N(f, ft), N(g, gt) ->
    if M.compare g M.one < 0
    then
      N(f, lazy(
        !gt *: (!ft @ gs)
      ))
    else
      (alone f) +: (gs *: (!ft @ gs))
let compose = ( @ )
*)

(****** diff & integrate ******)

(* C = n *)
let rec diff_h n s =
  if !? s
  then lazv( _diff_h n !!s )
  else lazy( _diff_h n !!s )

and _diff_h n = function
| E -> E
| N( h, t ) ->
    let ex_n = Expr.of_int n in
    N( ex_n */ h, diff_h (n + 1) t )

let rec diff s =
  if !? s
  then match !!s with
  | E -> lazv E
  | N( h,t ) ->
    if !? t
    then lazv( _diff_h 1 !!t )
    else lazy( _diff_h 1 !!t )
  else
    lazy( _diff !!s )

and _diff = function
| E -> E
| N( h, t ) ->
    _diff_h 1 !!t

let rec integrate_h n s =
  if !?s
  then lazv( _integrate_h n !!s )
  else lazy( _integrate_h n !!s )

and _integrate_h n = function
| E -> E
| N( h, t ) ->
    let ex_n = Expr.of_int n in
    N( h // ex_n, integrate_h (n + 1) t )

let rec integrate0 s =
  if !? s
  then lazv( _integrate0 !!s )
  else lazy( _integrate0 !!s )

and _integrate0 s = _integrate_h 1 !!s

let _integrate const p = N( const, integrate0 p )
let integrate const p = lazv( _integrate const p )


(****** newton iteration ******)

module Newton = struct
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

  (* C = ndrop + ntake *)
  let rec segc u ndrop ntake rest =
    if ndrop == 0 then concat u ntake rest else
    match u with
    | E -> E
    | N( _, t ) ->
        segc !!t (ndrop - 1) ntake rest

  and concat u ntake rest =
    if ntake == 0 then !!rest else
    match u with
    | E -> E
    | N( h, t ) ->
        N( h, lazy( concat !!t (ntake - 1) rest ) )

  (* output a list of length n *)
  (* C = n *)
  let flatten = function
  | E -> E

  | N(E,_) -> E

  | N(N(l00,_), v) ->
      let rec aux nd = function
      | E -> E
      | N(u0, u') ->
          segc u0 nd nd (lazy(aux (2 * nd) !!u'))
      in
      N(l00, lazy(aux 1 !!v) )

  let truncate n u = concat u n (lazv E)

  (* returns a llist of l llists of length n *)
  (* C = l F(n) *)
  let rec iterate n f start =
    let n' = n lsl 1 in
    N( start, lazy(
      iterate n' f ( truncate n' (f start) )
    ))

  (* C = N(F) = F(n) if F is regular enough *)
  let newton fnc start =
    lazy( flatten (iterate 1 fnc start) )
end

let newton f s = Newton.newton f s

(*
(****** reciprocal ******)

(* C = N(C) = C(n) = n^2 M(n) *)
let recip = function
| E -> raise Division_by_zero

| N(h,_) as p when M.compare M.one h < 0 -> (* ie f(0) = 0 *)
    let p' = diff p in
    newton (fun x -> x -: (p @ x -: (alone M.var)) /: (p' @ x)) (alone M.zero)

| N(f, lazy(N(f', lazy E))) ->
    let g' = M.shift (M.inv f') in (* get rid of exponent *)
    N(~/ f */ g', lazy(alone (M.shift g')))

| _ -> raise Division_by_zero

(*
(* revert (0:ft) = rs where rs = 0 : 1/(ft#rs) *)
let rec recip = function
  E -> E

| N(h, p) when M.null h ->
    let rec rs = N(M.zero, lazy(inv (!p @ rs)) ) in
    rs

| N(f, lazy(N(f', lazy E))) ->
    let f'' = M.inv f' in
    N(~/ (f */ f''), lazy(alone f''))

| N(_, _) -> raise Division_by_zero
*)


(**************** STANDARD FUNCTIONS *********************)

(* utils *)

(* C = F(n) *)
let fixpoint const func =
  let rec ret = N(const, lazy(func ret)) in
  ret

(* C = n *)
let rec fixpoint_map const func =
  N(const, lazy(fixpoint_map (func const) func))

(* C = 1 *)
let comp n m = M.deg_pow m n
let alt m = if M.odd m then M.neg m else m

(*** composable versions ***)
module Std_ = struct

  (* 1/(1-x) *)
  let rec ooom_mk e = N(M.translate e M.one, lazy(ooom_mk (e+1)))
  let one_over_one_minus = ooom_mk 0

  (* 1/(1+x) *)
  let one_over_one_plus  = map alt one_over_one_minus

  (* (1+x)^a *)
  let rec pow a =
    let rec loop last i =
      (* last * (a-i+1)/i *)
      let coef = (a -/ i) +/ M.one in
      let next = last */ (coef // i) in
      N(last, lazy(loop (M.shift next) (i +/ M.one)))
    in loop M.one M.one

  (* sqrt(1+x) *)
  let sqrt = pow M.half

  let exp  = fixpoint_map M.one M.integrate
  let ln   = integrate0 one_over_one_plus

  let cos =
    (* cos = -\int\int cos *)
    let help m = ~/ (M.integrate (M.integrate m)) in
    fixpoint_map M.one help
  let sin =
    (* sin = \int cos *)
    integrate0 cos

  let tan =
    (* tan = x + \int tan^2 *)
    let help p = integrate0 (square p) in
    fixpoint M.var help

  let asin =
    (* f = (1+x) ^ (-1/2) *)
    (* asin = \int f(-x)(x^2) *)
    let help m = M.integrate (comp 2 (alt m)) in
    map help (pow (~/ M.half))

  let acos =
    N(M.pi */ M.half, lazy(neg asin))

  let atan =
    (* atan' = 1/(1+x^2) *)
    let help m = M.integrate (comp 2 m) in
    map help one_over_one_plus


  let cosh =
    let help m = M.integrate (M.integrate m) in
    fixpoint_map M.one help
  let sinh =
    integrate0 cosh

  let tanh =
    let h0 m = ~/ (M.integrate m) in
    let help p = map h0 (square p) in
    fixpoint M.var help


  let asinh =
    (* f = (1+x)^(-1/2) *)
    (* asinh = \int f(x^2) *)
    let help m = M.integrate (comp 2 m) in
    map help (pow (~/ M.half))

  let atanh =
    (* atanh' = 1/(1-x^2) *)
    let help m = M.integrate (comp 2 m) in
    map help one_over_one_minus

end

(*** composing versions ***)
module Std = struct

  (** exp/log functions **)

  (* ln(h) = \int h' / h *)
  let rec ln = function
  | E -> raise Division_by_zero

  | N(m, _) as p ->
      if M.compare m M.one <> 0 then raise Division_by_zero else
      let const =
        if M.equal M.one m
        then M.zero else M.ln m
      in
      N(const, lazy(integrate0 ((diff p) /: p)))

  (* Integral method : exp(h) = \int h' * exp(h) *)
  let rec exp = function
  | E -> one

  | N(m,_) as p ->
      let cmp = M.compare m M.one in
      if cmp < 0 then raise Division_by_zero else
      let const =
        if cmp = 0 then M.exp m else M.one
      in
      let rec ret = N(const, lazy(integrate0 ((diff p) *: ret))) in
      ret

  (* Newton's method *)
  let exp2 p = match p with
  | E -> one

  | N(m, _) ->
      let help x =
        let image = (ln x) -: p in (* f(x) *)
        let corr  = x *: image in  (* f(x)/f'(x) *)
        x -: corr
      in
      newton help (alone (M.exp m))

  (* exponentiation *)

  (* Newton square root *)
  let sqrt p = match p with
  | E -> E

  | N(m, _) ->
      let help x =
        (* returns :  1/2 ( x + p/x ) *)
        let quo = p /: x in
        mmul_l M.half (x +: quo)
      in
      newton help (alone (M.sqrt m))

  (* p ^ (-1/2) using Newton *)
  let isqrt p = match p with
  | E -> raise Division_by_zero

  | N(m, _) ->
      let help x =
        (* returns : x/2 ( 3 - p x^2 ) *)
        let fac = (of_int 3) -: p *: (square x) in
        (mmul_l M.half x) *: fac
      in
      newton help (alone (M.inv (M.sqrt m)))

  (* Integral method for exponentiation *)
  (* P = p0 + ... ; P' = diff P
    R = P^a = p0^a + \int [ a P^(a-1) P' ] = p0^a + \int [ a R (P'/P) ]
  *)
  let anypow p a = match p with
  | E -> E
  | N(p0, p') ->
      let rec ret = N(M.anypow p0 a, lazy(integrate0(
        let p' = diff p in
        (mmul_l a ret) *: (p' /: p)
      ))) in
      ret

  (* integral square root & inverse square root *)
  let sqrt p  = anypow p M.half
  let isqrt p = anypow p (~/M.half)

  (* generic power *)
  let pow base expo =
    exp ((ln base) *: expo)

  (* trigonometric functions *)
  let cossin = function
  | E -> one,E

  | N(p0,_) as p ->
      let help m = M.integrate (M.neg m) in
      let p' = lazy(diff p) in
      let const =
        if M.compare M.one p0 < 0 then M.zero else p0
      in
      let rec s = N(M.sin const, lazy(integrate0(!p' *: c)))
      and     c = N(M.cos const, lazy(map help  (!p' *: s))) in
      (c,s)

  let cos p = fst(cossin p)
  let sin p = snd(cossin p)

  let tan = function
  | E -> E

  | N(m,_) as p when M.compare M.one m < 0 ->
      Std_.tan @ p

  | N(m, p) ->
      let rest = N(M.zero, p) in
      let trest = Std_.tan @ rest in
      let tm = M.tan m in
      ((alone tm) +: trest ) /: (one -: (mmul_l tm trest))


  (* reciprocal trigonometric functions *)
  (* integral method *)
  let asin = function
  | E -> E

  | N(t,_) as p when M.compare M.one t < 0 ->
      Std_.asin @ p

  | N(t,_) as p ->
      let c = M.asin t in
      let pol = (diff p) *: (isqrt (one -: (square p))) in
      integratem c pol
  let acos p = (alone (M.pi */ M.half)) -: (asin p);;
  let atan = function
    E -> E

  | N(t,_) as p when M.null t ->
      Std_.atan @ p

  | N(t,_) as p ->
      let c = M.atan t in
      let pol = (diff p) /: (one +: (square p)) in
      integratem c pol

  (* hyperbolic trigonometric fuctions *)
  let cossinh = function
  | E -> one,E

  | N(p0,_) as p ->
      let p' = lazy(diff p) in
      let const =
        if M.compare M.one p0 < 0 then M.zero else p0
      in
      let rec s = N(M.sinh const, lazy(integrate0(!p' *: c)))
      and     c = N(M.cosh const, lazy(integrate0(!p' *: s))) in
      (c,s)

  let cosh p = fst(cossinh p)
  let sinh p = snd(cossinh p)


  let tanh = function
  | E -> E

  | N(m, p) ->
      let rest = N(M.zero, p) in
      let trest = Std_.tanh @ rest in
      let tm = M.tan m in
      ((alone tm) +: trest ) /: (one +: (mmul_l tm trest))

  let asinh = function
  | E -> E

  | N(m, _) as p when M.null m ->
      Std_.asinh @ p

  | N(t, _) as p ->
      let c = M.asinh t in
      let pol = (diff p) *: (isqrt (one +: (square p))) in
      integratem c pol
  let acosh = function
  | E -> raise Division_by_zero

  | N(t, _) as p ->
      let c = M.acosh t in
      let minus_one = alone (~/ M.one) in
      let pol = (diff p) *: (isqrt (minus_one +: (square p))) in
      integratem c pol
  let atanh = function
  | E -> E

  | N(t,_) as p when M.null t ->
      Std_.atanh @ p

  | N(t,_) as p ->
      let c = M.atanh t in
      let pol = (diff p) /: (one -: (square p)) in
      integratem c pol

  let pi = alone M.pi

end

include Std
*)
