// csabase_visitor.h                                                  -*-C++-*-
// -----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// -----------------------------------------------------------------------------

#if !defined(INCLUDED_CSABASE_VISITOR)
#define INCLUDED_CSABASE_VISITOR 1
#ident "$Id$"

#include <csabase_abstractvisitor.h>
#include <cool/event.hpp>

// -----------------------------------------------------------------------------

namespace cool
{
    namespace csabase
    {
        class Visitor;
    }
}

// -----------------------------------------------------------------------------

class cool::csabase::Visitor:
    public cool::csabase::AbstractVisitor
{
public:
#define DECL(CLASS, BASE)                                             \
    cool::event<void(clang::CLASS##Decl const*)> on##CLASS##Decl;     \
    void do_visit(clang::CLASS##Decl const*);
DECL(,)
#include "clang/AST/DeclNodes.inc"

#define STMT(CLASS, PARENT)                                           \
    cool::event<void(clang::CLASS const*)> on##CLASS;                 \
    void do_visit(clang::CLASS const*);
STMT(Stmt,)
#include "clang/AST/StmtNodes.inc"
};

// -----------------------------------------------------------------------------

#endif