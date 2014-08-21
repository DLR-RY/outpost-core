int globalVar = 0;

int globalFunction() {
    class Test  // local to fun
      {
        /* members of Test class */
      };
    int x = 1+1;
    return x;
}


class MyClass {
    float myFloat;
    static int myInt;
    public:
    int doSomething(char c) {
        int a = 5;
        int b = 10;
        while (b != 15)
        {
            b++;
        }
        return b;
    }
};

int MyClass::myInt = 10;