// nobrace.cpp                                                        -*-C++-*-

void f()
{
    volatile int a = 0;

    do
    a = 1;
    while (a != 1);

    for (a = 0; a == 1; ++a)
    a = 1;

    if (a == 0)
    a = 1;

    if (a == 0)
    a = 1;
    else
    a = 0;

    switch (a)
    a = 1;

    while (a == 0)
    a = 1;

    do
        a = 1;
    while (a != 1);

    for (a = 0; a == 1; ++a)
        a = 1;

    if (a == 0)
        a = 1;

    if (a == 0)
        a = 1;
    else
        a = 0;

    switch (a)
        a = 1;

    while (a == 0)
        a = 1;
}

// ----------------------------------------------------------------------------
// Copyright (C) 2017 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
