// csatr_globalfunctiononlyinsource.v.cpp                             -*-C++-*-
// ----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// ----------------------------------------------------------------------------

#include "csatr_globalfunctiononlyinsource.v.hpp"
#ident "$Id$"

namespace cool
{
    namespace csatr
    {
        void globalfunctiononlyinsource_declared_in_namespace() {}
        void in_namespace();
        namespace
        {
            void in_unnamed_namespace()
            {
            }
        }

        namespace
        {
            struct bar
            {
                void member();
            };
        }

// ----------------------------------------------------------------------------

        namespace
        {
            template <typename> struct local;
            template <>
            struct local<cool::csatr::bar>
            {
                static void member();
            };
            void local<cool::csatr::bar>::member()
            {
            }
        }
    }
}