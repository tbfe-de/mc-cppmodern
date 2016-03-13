// http://coliru.stacked-crooked.com/a/57b243b26d57175c

namespace my {
    struct other {};
    struct clazz {
                 clazz() {} // default c'tor
                 clazz(char) {}                     // from char
        explicit clazz(int)  {}                     // from int
                 clazz(const other&) {}             // from other
        explicit operator bool() const {return {};} // to bool
//               operator char() const {return {};} // to char
        explicit operator long() const {return {};} // to long
                  long to_long() const {return {};} // member function
                operator other() const {return {};} // to other
    };
}

void foo(my::clazz) {}
void bar(my::other) {}
void fbool(bool) {}
void fchar(char) {}
void flong(long) {}
    
int main()
{
    foo(my::clazz());
    foo(my::clazz{});
    foo({});
    
    foo('x');
    foo(my::clazz('x'));
    foo(my::clazz{'x'});
    foo((my::clazz)'x');
    foo(static_cast<my::clazz>('x'));
    
    foo(42);
    foo(my::clazz(42));
    foo(my::clazz{42});
    foo((my::clazz)42);
    foo(static_cast<my::clazz>(42));
    
    my::clazz z; foo(z);

//  bool b{z};
//  fbool(z);
    fbool(bool(z));
//  fbool(bool{z});
    fbool((bool)z);
    fbool(static_cast<bool>(z));
    if (z) {}
    if (!z) {}
//  if (z == true) {}

#if 0
    char c{z};
    fchar(z);
    fchar(char(c));
    fchar(char{z});
    fchar((char)c);
    fbool(static_cast<char>(c));
#endif
  
    long l{z};
//  flong(z);
    flong(long(z));
//  flong(long{z});
    flong((long)z);
    flong(static_cast<long>(z));
    flong(z.to_long());
    
    foo(my::other{});
    my::other obj;
    foo(obj);
    foo(my::clazz(obj));
    foo(my::clazz{obj});
    foo((my::clazz)obj);
    foo(static_cast<my::clazz>(obj));
    
    bar(z);
    bar(my::other(z));
//  bar(my::other{z});
    bar((my::other)z);
    bar(static_cast<my::other>(z));
//  bar(reinterpret_cast<my::other>(z));
    bar(reinterpret_cast<my::other&>(z));
}
