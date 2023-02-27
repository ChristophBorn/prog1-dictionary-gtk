// Author: Christoph Born (53034, 22/041/62)
// common header file for unit tests:
// provides macros for debug output of expression & resulting value

#define EVAL_INT_EXPR(expr) printf("(%s) = %d\n", #expr, expr)
#define EVAL_STRING_EXPR(expr) printf("(%s) = %s\n", #expr, expr)
#define EVAL_POINTER_EXPR(expr) printf("(%s) = %s\n", #expr, (expr) ? "(valid pointer)" : "(null)")