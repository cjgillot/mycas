open Pseries_base

module DL = Dllist

module Naive = struct

  type mul_rep = {
    (* points to first *)
    mutable f: expr DL.t;
    mutable fs: series option;

    (* points to last *)
    mutable g: expr DL.t;
    mutable gs: series option

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

	(* C = n^2 = M(n) *)
	let _mul ( fs, gs ) =
	  let r = make_rep fs gs in
	  mul_with_rep r

	let mul fs gs =
	  if ( !? fs && !!fs == E ) || ( !? gs && !!gs == E )
	  then lazv E
	  else lazy( _mul ( !!fs, !!gs ) )

	(* C = n^2 = M(n) *)
	let _square a = _mul (a, a)
	let square a = mul a a
end

include Naive

let ( *: ) a b = mul a b
