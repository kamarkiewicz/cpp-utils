# SFINAE
## "Substitution Failure Is Not An Error"

This rule applies during overload resolution of function templates: When substituting the deduced type for the template parameter fails, the specialization is discarded from the overload set instead of causing a compile error.

This feature is used in template metaprogramming.

More on: http://en.cppreference.com/w/cpp/language/sfinae

