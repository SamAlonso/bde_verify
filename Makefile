# Makefile                                                       -*-makefile-*-

PREFIX     ?= /opt/bb
SHELL      ?= bash
LLVMDIR    ?= $(PREFIX)

PATH       := $(DISTRIBUTION_REFROOT)$(PREFIX)/lib/gcc-4.8/bin:$(DISTRIBUTION_REFROOT)$(PREFIX)/bin:/bbshr/bde/bde-tools/bin:/usr/bin:/bin:/usr/ccs/bin:/opt/swt/bin:$(PREFIX)/bin
export PATH

SYSTEM      = $(shell uname -s)
DESTDIR    ?= $(SYSTEM)

GCCDIR     ?= $(abspath $(dir \
              $(abspath $(dir \
              $(abspath $(shell env PATH=$(PATH) which g++))))))

TARGET      = bde_verify_bin
CSABASE     = csabase
LIBCSABASE  = libcsabase.a
CSABASEDIR  = groups/csa/csabase

CXXFLAGS   += -std=c++11
CXXFLAGS   += -Wall -Wno-mismatched-tags -Wno-unused-local-typedefs

CXXFLAGS   += -DSPELL_CHECK=1
INCFLAGS   += -I$(PREFIX)/include -I/opt/swt/include

# Set up locations and flags for the compiler that will build bde_verify.
ifeq ($(SYSTEM),Linux)
    COMPILER ?= gcc
    ifeq ($(COMPILER),gcc)
        CXX         = g++
    endif
    ifeq ($(COMPILER),clang)
        CXX         = clang++
        CXXFLAGS   += --gcc-toolchain=$(GCCDIR)
        LDFLAGS    += --gcc-toolchain=$(GCCDIR)
    endif

    AR          = /usr/bin/ar
    LINK        = $(CXX)
    LDFLAGS    += -Wl,-L,$(GCCDIR)/lib64 -Wl,-rpath,$(GCCDIR)/lib64
    LDFLAGS    += -Wl,-L,$(PREFIX)/lib64 -Wl,-rpath,$(PREFIX)/lib64
    LDFLAGS    += -Wl,-L,/opt/swt/lib64  -Wl,-rpath,/opt/swt/lib64
endif

ifeq ($(SYSTEM),SunOS)
    COMPILER ?= gcc
    ifeq ($(COMPILER),gcc)
        CXX         = g++
    endif
    ifeq ($(COMPILER),clang)
        CXX         = clang++
        CXXFLAGS   += --gcc-toolchain=$(GCCDIR)
        LDFLAGS    += --gcc-toolchain=$(GCCDIR)
    endif

    AR          = /usr/ccs/bin/ar
    LINK        = $(CXX)
    CXXFLAGS   += -DBYTE_ORDER=BIG_ENDIAN
    LDFLAGS    += -Wl,-L,$(GCCDIR)/sparcv9 -Wl,-R,$(GCCDIR)/sparcv9
    LDFLAGS    += -Wl,-L,$(PREFIX)/lib64   -Wl,-R,$(PREFIX)/lib64
    LDFLAGS    += -Wl,-L,/opt/swt/lib64    -Wl,-R,/opt/swt/lib64
    EXTRALIBS  += -lrt -lmalloc
endif

OBJ         = $(SYSTEM)-$(COMPILER)

# Set up location of clang headers and libraries needed by bde_verify.
INCFLAGS   += -I$(LLVMDIR)/include
LDFLAGS    += -L$(LLVMDIR)/lib -L$(CSABASEDIR)/$(OBJ)

export VERBOSE ?= @

#  ----------------------------------------------------------------------------

CXXFILES =                                                                    \
        groups/csa/csaaq/csaaq_cppinexternc.cpp                               \
        groups/csa/csaaq/csaaq_freefunctionsdepend.cpp                        \
        groups/csa/csaaq/csaaq_friendsinheaders.cpp                           \
        groups/csa/csaaq/csaaq_includeinexternc.cpp                           \
        groups/csa/csaaq/csaaq_inentns.cpp                                    \
        groups/csa/csaaq/csaaq_runtimeinit.cpp                                \
        groups/csa/csaaq/csaaq_transitiveincludes.cpp                         \
        groups/csa/csabde/csabde_tool.cpp                                     \
        groups/csa/csabbg/csabbg_allocatorforward.cpp                         \
        groups/csa/csabbg/csabbg_allocatornewwithpointer.cpp                  \
        groups/csa/csabbg/csabbg_bslovrdstl.cpp                               \
        groups/csa/csabbg/csabbg_enumvalue.cpp                                \
        groups/csa/csabbg/csabbg_functioncontract.cpp                         \
        groups/csa/csabbg/csabbg_midreturn.cpp                                \
        groups/csa/csabbg/csabbg_testdriver.cpp                               \
        groups/csa/csafmt/csafmt_banner.cpp                                   \
        groups/csa/csafmt/csafmt_comments.cpp                                 \
        groups/csa/csafmt/csafmt_headline.cpp                                 \
        groups/csa/csafmt/csafmt_indent.cpp                                   \
        groups/csa/csafmt/csafmt_longlines.cpp                                \
        groups/csa/csafmt/csafmt_nonascii.cpp                                 \
        groups/csa/csafmt/csafmt_whitespace.cpp                               \
        groups/csa/csamisc/csamisc_anonymousnamespaceinheader.cpp             \
        groups/csa/csamisc/csamisc_arrayargument.cpp                          \
        groups/csa/csamisc/csamisc_arrayinitialization.cpp                    \
        groups/csa/csamisc/csamisc_boolcomparison.cpp                         \
        groups/csa/csamisc/csamisc_charvsstring.cpp                           \
        groups/csa/csamisc/csamisc_constantreturn.cpp                         \
        groups/csa/csamisc/csamisc_contiguousswitch.cpp                       \
        groups/csa/csamisc/csamisc_cstylecastused.cpp                         \
        groups/csa/csamisc/csamisc_funcalpha.cpp                              \
        groups/csa/csamisc/csamisc_hashptr.cpp                                \
        groups/csa/csamisc/csamisc_longinline.cpp                             \
        groups/csa/csamisc/csamisc_memberdefinitioninclassdefinition.cpp      \
        groups/csa/csamisc/csamisc_namespacetags.cpp                          \
        groups/csa/csamisc/csamisc_opvoidstar.cpp                             \
        groups/csa/csamisc/csamisc_spellcheck.cpp                             \
        groups/csa/csamisc/csamisc_strictaliasing.cpp                         \
        groups/csa/csamisc/csamisc_stringadd.cpp                              \
        groups/csa/csamisc/csamisc_swapab.cpp                                 \
        groups/csa/csamisc/csamisc_thatwhich.cpp                              \
        groups/csa/csamisc/csamisc_thrownonstdexception.cpp                   \
        groups/csa/csamisc/csamisc_unnamed_temporary.cpp                      \
        groups/csa/csamisc/csamisc_verifysameargumentnames.cpp                \
        groups/csa/csastil/csastil_externalguards.cpp                         \
        groups/csa/csastil/csastil_implicitctor.cpp                           \
        groups/csa/csastil/csastil_includeorder.cpp                           \
        groups/csa/csastil/csastil_leakingmacro.cpp                           \
        groups/csa/csastil/csastil_templatetypename.cpp                       \
        groups/csa/csastil/csastil_uppernames.cpp                             \
        groups/csa/csatr/csatr_componentheaderinclude.cpp                     \
        groups/csa/csatr/csatr_componentprefix.cpp                            \
        groups/csa/csatr/csatr_entityrestrictions.cpp                         \
        groups/csa/csatr/csatr_files.cpp                                      \
        groups/csa/csatr/csatr_friendship.cpp                                 \
        groups/csa/csatr/csatr_globalfunctiononlyinsource.cpp                 \
        groups/csa/csatr/csatr_globaltypeonlyinsource.cpp                     \
        groups/csa/csatr/csatr_groupname.cpp                                  \
        groups/csa/csatr/csatr_includeguard.cpp                               \
        groups/csa/csatr/csatr_nesteddeclarations.cpp                         \
        groups/csa/csatr/csatr_packagename.cpp                                \
        groups/csa/csatr/csatr_usingdeclarationinheader.cpp                   \
        groups/csa/csatr/csatr_usingdirectiveinheader.cpp                     \

TODO =                                                                        \
        groups/csa/csadep/csadep_dependencies.cpp                             \
        groups/csa/csadep/csadep_types.cpp                                    \
        groups/csa/csamisc/csamisc_calls.cpp                                  \
        groups/csa/csamisc/csamisc_includeguard.cpp                           \
        groups/csa/csamisc/csamisc_selfinitialization.cpp                     \
        groups/csa/csamisc/csamisc_superfluoustemporary.cpp                   \

# -----------------------------------------------------------------------------

DEFFLAGS += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
INCFLAGS += -I. -I$(CSABASEDIR) -Igroups/csa/csadep
CXXFLAGS += -g -fno-common -fno-strict-aliasing -fno-exceptions -fno-rtti
LDFLAGS  += -g

OFILES = $(CXXFILES:%.cpp=$(OBJ)/%.o)

LIBS     =    -lcsabase                                                       \
              -lLLVMX86AsmParser                                              \
              -lLLVMSparcAsmParser                                            \
              -lclangFrontendTool                                             \
              -lclangCodeGen                                                  \
              -lLLVMIRReader                                                  \
              -lLLVMLinker                                                    \
              -lLLVMipo                                                       \
              -lLLVMX86CodeGen                                                \
              -lLLVMSparcCodeGen                                              \
              -lLLVMSelectionDAG                                              \
              -lLLVMAsmPrinter                                                \
              -lLLVMInterpreter                                               \
              -lLLVMCodeGen                                                   \
              -lLLVMScalarOpts                                                \
              -lLLVMInstrumentation                                           \
              -lLLVMInstCombine                                               \
              -lLLVMVectorize                                                 \
              -lclangRewriteFrontend                                          \
              -lclangRewrite                                                  \
              -lclangARCMigrate                                               \
              -lclangStaticAnalyzerFrontend                                   \
              -lclangIndex                                                    \
              -lclangFormat                                                   \
              -lclangTooling                                                  \
              -lclangToolingCore                                              \
              -lclangFrontend                                                 \
              -lclangDriver                                                   \
              -lLLVMObjCARCOpts                                               \
              -lLLVMTransformUtils                                            \
              -lLLVMipa                                                       \
              -lLLVMAnalysis                                                  \
              -lLLVMAsmParser                                                 \
              -lclangSerialization                                            \
              -lLLVMBitReader                                                 \
              -lLLVMBitWriter                                                 \
              -lLLVMProfileData                                               \
              -lLLVMTarget                                                    \
              -lLLVMExecutionEngine                                           \
              -lLLVMCore                                                      \
              -lLLVMDebugInfo                                                 \
              -lclangParse                                                    \
              -lLLVMMCParser                                                  \
              -lLLVMX86Desc                                                   \
              -lLLVMSparcDesc                                                 \
              -lLLVMX86Info                                                   \
              -lLLVMSparcInfo                                                 \
              -lLLVMX86AsmPrinter                                             \
              -lLLVMSparcAsmPrinter                                           \
              -lLLVMX86Utils                                                  \
              -lLLVMMCDisassembler                                            \
              -lclangSema                                                     \
              -lclangStaticAnalyzerCheckers                                   \
              -lclangStaticAnalyzerCore                                       \
              -lclangDynamicASTMatchers                                       \
              -lclangASTMatchers                                              \
              -lclangEdit                                                     \
              -lclangAnalysis                                                 \
              -lclangAST                                                      \
              -lclangLex                                                      \
              -lclangBasic                                                    \
              -lLLVMMC                                                        \
              -lLLVMObject                                                    \
              -lLLVMOption                                                    \
              -lLLVMTableGen                                                  \
              -lLLVMSupport                                                   \
              -lpthread                                                       \
              -lcurses                                                        \
              -ldl                                                            \
              -lz                                                             \
              -laspell                                                        \
              $(EXTRALIBS)

default: $(OBJ)/$(TARGET)

.PHONY: csabase

$(CSABASEDIR)/$(OBJ)/$(LIBCSABASE): csabase
	$(VERBOSE) $(MAKE) -s  -C $(CSABASEDIR)

$(OBJ)/$(TARGET): $(CSABASEDIR)/$(OBJ)/$(LIBCSABASE) $(OFILES)
	@echo linking executable
	$(VERBOSE) $(LINK) $(LDFLAGS) -o $@.$$ $(OFILES) $(LIBS)
	mv $@.$$ $@

$(OBJ)/%.o: %.cpp
	@if [ ! -d $(@D) ]; then mkdir -p $(@D); fi
	@echo compiling $(@:$(OBJ)/%.o=%.cpp)
	$(VERBOSE) $(CXX) $(INCFLAGS) $(DEFFLAGS) $(CXXFLAGS) \
                          -o $@ -c $(@:$(OBJ)/%.o=%.cpp)

install:  $(OBJ)/$(TARGET) $(CSABASEDIR)/$(OBJ)/$(LIBCSABASE)
	mkdir -p $(DESTDIR)/bin
	cp $(OBJ)/$(TARGET) $(DESTDIR)/bin
	mkdir -p $(DESTDIR)/lib
	cp $(CSABASEDIR)/$(OBJ)/$(LIBCSABASE) $(DESTDIR)/lib
	cp bde_verify.cfg $(DESTDIR)/bin
	cp scripts/bde_verify $(DESTDIR)/bin
	mkdir -p $(DESTDIR)/include/bde_verify
	cp $(CSABASEDIR)/csabase_*.h $(DESTDIR)/include/bde_verify
	cp groups/csa/csadep/csadep_*.h $(DESTDIR)/include/bde_verify

.PHONY: clean

clean:
	@echo cleaning files
	$(VERBOSE) $(RM) $(OFILES)
	$(VERBOSE) $(RM) $(OBJ)/$(TARGET)
	$(VERBOSE) $(RM) $(OBJ)/make.depend
	$(VERBOSE) $(RM) -r $(OBJ)
	$(VERBOSE) $(RM) mkerr olderr *~
	$(VERBOSE) $(MAKE) -C $(CSABASEDIR) clean

# -----------------------------------------------------------------------------

export BDE_VERIFY_DIR := $(shell /bin/pwd)

# All the Makefiles below the checks directory.
define ALLM :=
    $(shell find checks -name Makefile | sort)
endef

# All the Makefiles below both the checks and CURRENT directory.
define CURM :=
    $(shell find checks -regex 'checks\(/.*\)?/$(CURRENT)\(/.*\)?/Makefile' | \
            sort)
endef

CNAMES   := $(foreach N,$(ALLM),$(patsubst %,%.check,$(N)))
CCURNAME := $(foreach N,$(CURM),$(patsubst %,%.check,$(N)))
RNAMES   := $(foreach N,$(ALLM),$(patsubst %,%.run,$(N)))
RCURNAME := $(foreach N,$(CURM),$(patsubst %,%.run,$(N)))

.PHONY: check-current check $(CNAMES) run-current run $(RNAMES)

check: $(OBJ)/$(TARGET) $(CNAMES)
check-current: $(OBJ)/$(TARGET) $(CCURNAME)

$(CNAMES):
	$(VERBOSE) $(MAKE) -s -C $(@D) check

run: $(OBJ)/$(TARGET) $(RNAMES)
run-current: $(OBJ)/$(TARGET) $(RCURNAME)

$(RNAMES):
	$(VERBOSE) $(MAKE) -s -C $(@D) run

# -----------------------------------------------------------------------------

.PHONY: depend

depend $(OBJ)/make.depend:
	@if [ ! -d $(OBJ) ]; then mkdir $(OBJ); fi
	@echo analysing dependencies
	$(VERBOSE) $(CXX) $(INCFLAGS) $(DEFFLAGS) -M $(CXXFILES)                  \
            $(filter-out -Wno-unused-local-typedefs, $(CXXFLAGS))             \
		| perl -pe 's[^(?! )][$(OBJ)/]' > $(OBJ)/make.depend

ifneq "$(MAKECMDGOALS)" "clean"
    include $(OBJ)/make.depend
endif

## ----------------------------------------------------------------------------
## Copyright (C) 2014 Bloomberg Finance L.P.
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to
## deal in the Software without restriction, including without limitation the
## rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
## sell copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
## IN THE SOFTWARE.
## ----------------------------- END-OF-FILE ----------------------------------
