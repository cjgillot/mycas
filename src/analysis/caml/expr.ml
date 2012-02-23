type basic
type expr = private { __custom_private: unit; rtti : Rtti.rtti }

external of_int : int  -> expr = "__caml_expr_new_of_int"

external null_p: expr -> bool = "__caml_expr_null" "noalloc"
external unit_p: expr -> bool = "__caml_expr_unit" "noalloc"

external has_p: expr -> expr -> bool = "__caml_expr_has" "noalloc"

external diff  : expr -> expr        -> expr = "__caml_expr_diff"
external diff_n: expr -> expr -> int -> expr = "__caml_expr_diff_n"
external series: expr -> expr        -> expr = "__caml_expr_series"

external expand: expr -> expr = "__caml_expr_expand"
external subs: expr -> ( expr * expr ) array -> expr = "__caml_expr_subs"

external neg: expr -> expr = "__caml_expr_neg"
external inv: expr -> expr = "__caml_expr_inv"

external add: expr -> expr -> expr = "__caml_expr_add"
external sub: expr -> expr -> expr = "__caml_expr_sub"
external mul: expr -> expr -> expr = "__caml_expr_mul"
external div: expr -> expr -> expr = "__caml_expr_div"

external pow: expr -> expr -> expr = "__caml_expr_pow"

external compare: expr -> expr -> int = "__caml_expr_compare" "noalloc"
external hash: expr -> expr -> int = "__caml_expr_hash" "noalloc"
