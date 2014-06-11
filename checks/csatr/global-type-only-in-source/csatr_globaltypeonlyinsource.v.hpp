// csatr_globaltypeonlyinsource.v.hpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// ----------------------------------------------------------------------------

#ifndef INCLUDED_CSATR_GLOBALTYPEONLYINSOURCE
#define INCLUDED_CSATR_GLOBALTYPEONLYINSOURCE 1
#if !defined(INCLUDED_BDES_IDENT)
#  include <bdes_ident.h>
#endif
#ifndef INCLUDED_CSASCM_VERSION
#  include <csascm_version.h>
#endif

// ----------------------------------------------------------------------------

namespace bde_verify
{
    namespace csatr
    {
        struct globaltypeonlyinsource_s_ok { int member; };
        class  globaltypeonlyinsource_c_ok { int member; };
    }
}

// ----------------------------------------------------------------------------

#endif