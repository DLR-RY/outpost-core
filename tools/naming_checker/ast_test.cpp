/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014, Rhea Rinaldo (DLR RY-AVS)
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
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
