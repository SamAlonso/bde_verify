// csatr_componentprefix.v.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// ----------------------------------------------------------------------------

#ifndef INCLUDED_CSATR_COMPONENTPREFIX
#define INCLUDED_CSATR_COMPONENTPREFIX 1
#ident "$Id$"

// ----------------------------------------------------------------------------

namespace cool
{
    namespace csatr
    {
        enum BadEnum
        {
            badBadEnumTag
        };

        typedef BadEnum BadTypedef;

        struct BadStruct
        {
            enum GoodEnum
            {
                goodGoodEnumTag
            };
            int member;
            int method();
        };

        typedef struct BadStruct1
        {
            int member;
            int method();
        } BadStruct2;

        typedef struct
        {
            int member;
            int method();
        } BadStruct3;

        class BadClass
        {
        public:
            int member;
            int method();
        };

        template <typename>
        class BadTemplate
        {
        public:
            int member;
            int method();
        };

        template <typename>
        int badTemplate()
        {
            int goodBadTemplateVariable;
            return goodBadTemplateVariable;
        }

        int badFunction()
        {
            int goodBadFunctionVariable = badTemplate<int>();
            return goodBadFunctionVariable;
        }

        int badVariable(0);

        enum ComponentPrefixGoodEnum
        {
            badGoodEnumTag,
            componentPrefixGoodEnumTag
        };

        typedef BadEnum ComponentPrefixGoodTypedef;

        struct ComponentPrefixGoodStruct
        {
            enum GoodEnum
            {
                goodGoodEnumTag
            };
            int member;
            int method();
        };

        class ComponentPrefixGoodClass
        {
        public:
            int member;
            int method();
        };

        void swap(ComponentPrefixGoodClass&, ComponentPrefixGoodClass&);

        template <typename>
        class ComponentPrefixGoodTemplate
        {
        public:
            int member;
            int method();
        };

        template <typename T>
        void swap(ComponentPrefixGoodTemplate<T>&,
                  ComponentPrefixGoodTemplate<T>&);

        template <typename>
        int componentPrefixGoodTemplate()
        {
            int goodGoodTemplateVariable;
            return goodGoodTemplateVariable;
        }

        int componentPrefixGoodFunction()
        {
            int goodGoodFunctionVariable = componentPrefixGoodTemplate<int>();
            return goodGoodFunctionVariable;
        }

        int componentPrefixGoodVariable(0);

        class GoodDeclaration;
    }
}

// ----------------------------------------------------------------------------

#endif