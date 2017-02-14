/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

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
