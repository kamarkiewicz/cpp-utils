# ADL
## Argument-dependent lookup

Argument-dependent lookup, also known as ADL, or Koenig lookup, is the set of rules for looking up the unqualified function names in function-call expressions, including implicit function calls to overloaded operators. These function names are looked up in the namespaces of their arguments in addition to the scopes and namespaces considered by the usual unqualified name lookup.

Argument-dependent lookup makes it possible to use operators defined in a different namespace.

More on: http://en.cppreference.com/w/cpp/language/adl

