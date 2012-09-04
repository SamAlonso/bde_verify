// csatr_globaltypeonlyinsource.cpp                                   -*-C++-*-
// ----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// ----------------------------------------------------------------------------

#include <csabase_analyser.h>
#include <csabase_location.h>
#include <csabase_registercheck.h>
#ident "$Id$"

// ----------------------------------------------------------------------------

static std::string const check_name("global-type-only-in-source");

// ----------------------------------------------------------------------------

namespace
{
    struct decl_not_in_toplevel
    {
        decl_not_in_toplevel(cool::csabase::Analyser* analyser)
            : analyser_(analyser)
        {
        }
        bool operator()(clang::Decl const& decl) const
        {
            return (*this)(&decl);
        }
        bool operator()(clang::Decl const* decl) const
        {
            return this->analyser_->get_location(decl).file()
                != this->analyser_->toplevel();
        }
        cool::csabase::Analyser* analyser_;
    };
}

// ----------------------------------------------------------------------------

static void
global_type_only_in_source(cool::csabase::Analyser&  analyser,
                           clang::TypeDecl const    *decl)
{
    if (decl->getDeclContext()->isFileContext()
        && analyser.get_location(decl).file() == analyser.toplevel()
        && !decl->isInAnonymousNamespace()
        && !analyser.is_test_driver()
        && std::find_if(decl->redecls_begin(), decl->redecls_end(),
                        ::decl_not_in_toplevel(&analyser))
            == decl->redecls_end()
        && (decl->getLinkage() == clang::UniqueExternalLinkage
            || decl->getLinkage() == clang::ExternalLinkage)
        && decl->getDeclName().isIdentifier()
        && !decl->getDeclName().getAsString().empty()
        )
    {
        analyser.report(decl, check_name,
                       "TR10: globally visible type '%0' "
                        "is not declared in header.")
            << decl->getQualifiedNameAsString();
    }
}

// ----------------------------------------------------------------------------

static cool::csabase::RegisterCheck check(check_name,
                                          &::global_type_only_in_source);