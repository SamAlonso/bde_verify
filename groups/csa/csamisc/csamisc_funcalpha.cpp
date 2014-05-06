// csabbg_funcalpha.cpp                                               -*-C++-*-
// ----------------------------------------------------------------------------

#include <csabase_analyser.h>
#include <csabase_debug.h>
#include <csabase_location.h>
#include <csabase_ppobserver.h>
#include <csabase_registercheck.h>
#include <csabase_util.h>
#include <csabase_visitor.h>
#include <llvm/Support/Regex.h>
#include <clang/AST/TypeLoc.h>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <ctype.h>

#ident "$Id$"

// ----------------------------------------------------------------------------

static std::string const check_name("alphabetical-functions");

// ----------------------------------------------------------------------------

using clang::Decl;
using clang::DeclarationName;
using clang::FunctionDecl;
using clang::FunctionTemplateDecl;
using clang::FunctionTypeLoc;
using clang::SourceManager;
using clang::SourceRange;
using clang::TypeLoc;
using csabase::Analyser;
using csabase::Location;
using csabase::PPObserver;
using csabase::Visitor;

namespace
{

struct data
    // Data attached to analyzer for this check.
{
    typedef std::set<unsigned> Lines;
    typedef std::map<std::string, Lines> Comments;
    Comments d_comments;  // Comment lines per file.

    typedef std::vector<std::pair<const FunctionDecl*, const Decl*> >
                                                                     Functions;
    Functions d_functions;
};

struct comments
    // Callback object for inspecting comments.
{
    Analyser& d_analyser;         // Analyser object.
    SourceManager& d_manager;     // SourceManager within Analyser.
    data::Comments& d_comments;   // Analyser's comment data.

    comments(Analyser& analyser);
        // Create a 'comments' object, accessing the specified 'analyser'.

    static bool isReset(llvm::StringRef comment);
        // Return wehether the specified 'comment' acts as a "reset" marker for
        // alphabetical ordering.

    void operator()(SourceRange range);
        // The specified 'range', representing a comment, is added to the
        // comments list.
};

comments::comments(Analyser& analyser)
: d_analyser(analyser)
, d_manager(analyser.manager())
, d_comments(analyser.attachment<data>().d_comments)
{
}

bool comments::isReset(llvm::StringRef comment)
{
    // Look for "aspect" directives in comments.
    static llvm::Regex re("^(//|/[*])" "[[:space:]]*"
                          "("
                             "CLASS"     "|"
                             "FREE"      "|"
                             "NOT"       "|"
                             "PRIVATE"   "|"
                             "PROTECTED" "|"
                             "PUBLIC"    "|"
                             "."         "|"  // Try allowing everything here.
                             "[[:space:]]"
                          ")*"
                          "("
                             "ACCESSORS?"    "|"
                             "Aspects?"      "|"
                             "CREATORS?"     "|"
                             "DATA"          "|"
                             "FUNCTIONS?"    "|"
                             "IMPLEMENTED"   "|"
                             "MANIPULATORS?" "|"
                             "METHODS?"      "|"
                             "OPERATORS?"    "|"
                             "TRAITS?"       "|"
                             "[-=_]+" "(" "[[:space:]]*" "[-=_]+" ")*"
                          ")"
                          "[:;.[:space:]]*" "([*]/)?" "[[:space:]]*" "$",
                          llvm::Regex::IgnoreCase);
    std::string error;
    return re.match(comment);
}

void comments::operator()(SourceRange range)
{
    Location location(d_analyser.get_location(range.getBegin()));
    if (   d_analyser.is_component(location.file())
        && isReset(d_analyser.get_source(range))) {
        d_comments[location.file()].insert(location.line());
    }
}

struct report
    // Callback object invoked upon completion.
{
    Analyser& d_analyser;       // Analyser object.
    SourceManager& d_manager;   // SourceManager within Analyser.
    data& d;                    // Analyser's data for this module.

    report(Analyser& analyser);
        // Create a 'report' object, accessing the specified 'analyser'.

    void operator()();
        // Invoked at end;

    void operator()(const FunctionDecl *decl);
        // Invoked to process function declarations.

    void operator()(const FunctionTemplateDecl *decl);
        // Invoked to process function template declarations.

    void check_order(std::pair<const FunctionDecl *, const Decl *> p);
        // Check if function is in alphanumeric order.
};

report::report(Analyser& analyser)
: d_analyser(analyser)
, d_manager(analyser.manager())
, d(analyser.attachment<data>())
{
}

void report::operator()()
{
    data::Functions::iterator b = d.d_functions.begin();
    data::Functions::iterator e = d.d_functions.end();
    for (data::Functions::iterator i = b; i != e; ++i) {
        check_order(*i);
    }
}

void report::operator()(const FunctionTemplateDecl *decl)
{
    d.d_functions.push_back(std::make_pair(decl->getTemplatedDecl(), decl));
}

void report::operator()(const FunctionDecl *decl)
{
    d.d_functions.push_back(std::make_pair(decl, decl));
}

void report::check_order(std::pair<const FunctionDecl *, const Decl *> p)
{
    const FunctionDecl *decl = p.first;
    DeclarationName name = decl->getDeclName();
    const Decl *next = p.second->getNextDeclInContext();

    if (   d_analyser.is_component(decl)
        && !decl->isTemplateInstantiation()
        && next
        && name.isIdentifier()
        && !name.isEmpty()) {
        const FunctionDecl *nextf = llvm::dyn_cast<FunctionDecl>(next);
        const FunctionTemplateDecl* nextt =
            llvm::dyn_cast<FunctionTemplateDecl>(next);
        if (nextt) {
            nextf = nextt->getTemplatedDecl();
        }
        if (nextf && !nextf->isTemplateInstantiation()) {
            DeclarationName next_name = nextf->getDeclName();
            if (next_name.isIdentifier() &&
                !next_name.isEmpty() &&
                llvm::StringRef(next_name.getAsString())
                        .compare_numeric(name.getAsString()) < 0) {
                std::string q1 = decl->getQualifiedNameAsString();
                std::string q2 = nextf->getQualifiedNameAsString();
                q1 = q1.substr(0, q1.rfind(':'));
                q2 = q2.substr(0, q2.rfind(':'));
                bool reset = q1 != q2;
                if (!reset) {
                    Location l1 = d_analyser.get_location(decl);
                    Location l2 = d_analyser.get_location(nextf);
                    const data::Lines &lines = d.d_comments[l1.file()];
                    for (unsigned i = l1.line(); i <= l2.line(); ++i) {
                        if ((reset = lines.count(i)) == true) {
                            break;
                        }
                    }
                }
                if (!reset && q1 == q2) {
                    d_analyser.report(
                            decl->getLocation(),
                            check_name, "FABC01",
                            "Function '%0' not in alphanumeric order")
                        << name.getAsString()
                        << decl->getNameInfo().getSourceRange();
                    d_analyser.report(nextf->getLocation(),
                                      check_name, "FABC01",
                                      "Next function is '%0'",
                                      false, clang::DiagnosticsEngine::Note)
                        << next_name.getAsString()
                        << nextf->getNameInfo().getSourceRange();
                }
            }
        }
    }
}

void subscribe(Analyser& analyser, Visitor& visitor, PPObserver& observer)
    // Hook up the callback functions.
{
    observer.onComment             += comments(analyser);
    visitor.onFunctionDecl         += report(analyser);
    visitor.onFunctionTemplateDecl += report(analyser);
    analyser.onTranslationUnitDone += report(analyser);
}

}  // close anonymous namespace

// ----------------------------------------------------------------------------

static csabase::RegisterCheck c3(check_name, &subscribe);
