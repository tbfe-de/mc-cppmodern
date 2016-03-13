// http://coliru.stacked-crooked.com/a/a22a7e3292779248

namespace my {
    class clazz {
    public:
        virtual void m1() const {}

     // virtual void m2() {}
        virtual void m2() final {}

     // void m3() override {}
        void m3() {}
    };

    class other : public clazz {
    public:
    //  virtual void m1() override {}
        virtual void m1() const override {}

    //  virtual void m2() {}

        virtual void m3() {}
    //  virtual void m3() override {}
   };
}

int main() {
    my::clazz c; c.m1(); c.m2(); c.m3();
    my::other o; o.m1(); o.m2(); o.m3();
}
