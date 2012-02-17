open Base

let rec _taylor ex sym denom order =
  let dex = Expr.diff ex sym
  and ord' = order + 1 in
  let den' = denom */ (Expr.of_int ord') in

  if Expr.null_p dex
  then E
  else
    let dex0 = Expr.subs dex [| sym, _ex0 |] in
    N( dex0 // denom, lazy( _taylor dex sym den' ord' ))

let taylor ex sym =
  if Expr.null_p ex
  then lazv E
  else
    let ex' = Expr.subs ex [| sym, _ex0 |] in
    lazv( N( ex', lazy( _taylor ex sym (Expr.of_int 1) 1 ) ) )
