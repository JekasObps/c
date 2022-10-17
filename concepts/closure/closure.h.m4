dnl Trying out m4 preprocessor, in order to produce expandable closure.h header
dnl conclusion: it feels like it has much more powers over cpp, but syntax if so off
dnl             really it's match faster to write this thing in python instead (proved)
dnl             but finding out why these recursive macros expand not as was intended...
dnl
dnl Leaving it as is in a partialy complited state as part of a history.
dnl
define(`args', `_$1 ifelse(eval($1 < COUNT), 1, `, args(incr($1))')')dnl
define(`_args',`args(1)')dnl

`#define' _UNWRAP(M, _args, ARGN) M`##'ARGN

define(`_tr', `translit($1,_0123456789,` 'xabcdefghi)')dnl
define(`_alpha', `_tr($1) ifelse($2,,,`, _alpha(shift($@))')')dnl

define(`_shift', `ifelse(eval($1 > 0), 1, `_shift(decr($1), shift(shift($@)))', `shift($@)')')dnl

define(`_line', `$3 ifelse(eval($1 < $2), 1, `, _line(incr($1), $2, _shift(3, $@))')')dnl
define(`_lines', `_line(1, $2, _shift(3, $@)) ifelse(eval($2 < $3), 1, 
_lines($1, ircr($2), $3, _shift(3, $@)))')dnl

define(`_members', `_alpha($1) $1 ifelse($2,,,`; _members(shift($@))')')dnl 

define(`_expandARG', ``#define' _ARG$#(_alpha($@)) _members($@)')dnl


dnl _line(1, 3, _1, _2, _3)
dnl _expandARG(_1, _2, _3)
dnl argn(2, a, b, c, d, e, f) -> b
dnl _expandARG(_line(1, 3, _1, _2, _3))

