open Expr

type func = {
  name: Rtti.corefuncs;
  args_b: expr Iterator.iterator;
  args_e: expr Iterator.iterator
}

external get_function: expr -> func = "__caml_expr_get_function"

type vectorseq = {
  coef: expr;
  seq_b: expr Iterator.iterator;
  seq_e: expr Iterator.iterator
}

external get_vseq: expr -> vectorseq = "__caml_expr_get_vseq"
