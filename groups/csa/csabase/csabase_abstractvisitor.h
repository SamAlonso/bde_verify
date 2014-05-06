// csabase_abstractvisitor.h                                          -*-C++-*-
// -----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// -----------------------------------------------------------------------------

#if !defined(INCLUDED_CSABASE_ABSTRACTVISITOR)
#define INCLUDED_CSABASE_ABSTRACTVISITOR 1
#ident "$Id$"

#include <clang/AST/Stmt.h>
#include <clang/AST/StmtVisitor.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclVisitor.h>

// -----------------------------------------------------------------------------

namespace csabase {
    class AbstractVisitor;
} // close package namespace

// -----------------------------------------------------------------------------

class csabase::AbstractVisitor:
    public clang::DeclVisitor<csabase::AbstractVisitor>,
    public clang::StmtVisitor<csabase::AbstractVisitor>
{
public:
    virtual ~AbstractVisitor();

    void visit(clang::Decl const*);
    void visit(clang::Stmt const*);

#define DECL(CLASS, BASE)                                               \
    virtual void do_visit(clang::CLASS##Decl const*);                   \
    void process_decl(clang::CLASS##Decl*, bool nest = false);          \
    void Visit##CLASS##Decl (clang::CLASS##Decl*);
DECL(,void)
#include "clang/AST/DeclNodes.inc"

#define STMT(CLASS, BASE)                                      \
    virtual void do_visit(clang::CLASS const*);                \
    void process_stmt(clang::CLASS*, bool nest = false);       \
    void Visit##CLASS(clang::CLASS*);
STMT(Stmt,void)
#include "clang/AST/StmtNodes.inc"

    void visit_decl(clang::Decl const*);
    void visit_stmt(clang::Stmt const*);
    void visit_context(void const*);
    void visit_context(clang::DeclContext const*);
    void visit_children(clang::StmtRange const&);
    template <typename Children> void visit_children(Children const&);
};

// -----------------------------------------------------------------------------

#endif
