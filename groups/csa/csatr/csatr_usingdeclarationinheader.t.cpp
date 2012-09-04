// csatr_usingdeclarationinheader.t.cpp                               -*-C++-*-
// ----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// ----------------------------------------------------------------------------

#include "csatr_usingdeclarationinheader.t.hpp"
#ident "$Id$"

using cool::csamisc::foo;
namespace cool
{
    namespace csatr
    {
        using cool::csamisc::foo;
        static void f()
        {
            using cool::csamisc::foo;
        }
    }
}