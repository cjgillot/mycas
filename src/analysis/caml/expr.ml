type expr

external of_int : int  -> expr = "__caml_expr_new_of_int"

external null_p: expr -> bool = "__caml_expr_null"
external unit_p: expr -> bool = "__caml_expr_unit"

external has_p: expr -> expr -> bool = "__caml_expr_has"

external diff_n: expr -> expr -> int -> expr = "__caml_expr_diff_n"
external diff  : expr -> expr        -> expr = "__caml_expr_diff"

external expand: expr -> expr = "__caml_expr_expand"

external neg: expr -> expr = "__caml_expr_neg"
external inv: expr -> expr = "__caml_expr_inv"

external add: expr -> expr -> expr = "__caml_expr_add"
external sub: expr -> expr -> expr = "__caml_expr_sub"
external mul: expr -> expr -> expr = "__caml_expr_mul"
external div: expr -> expr -> expr = "__caml_expr_div"

external pow: expr -> expr -> expr = "__caml_expr_pow"

external compare: expr -> expr -> int = "__caml_expr_compare"
external hash: expr -> expr -> int = "__caml_expr_hash"
