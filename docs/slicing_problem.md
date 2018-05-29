Slicing" is where you assign an object of a derived class to an instance of a base class, thereby losing part of the information - some of it is "sliced" away.

For example,

```cpp
class A {
   int foo;
};

class B : public A {
   int bar;
};
```

So an object of type `B` has two data members, `foo` and `bar`.

Then if you were to write this:

```cpp
B b;

A a = b;
```

Then the information in `b` about member bar is lost in `a`.

