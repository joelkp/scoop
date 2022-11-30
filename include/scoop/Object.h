/* SCOOP Object module
 *
 * Copyright (c) 2010, 2011, 2013, 2022 Joel K. Pettersson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef scoop_Object_h
#define scoop_Object_h
#ifndef SCO_API
# include "API.h"
#endif
struct scoMempool;
#include <stdlib.h>
#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif

/** \file
   The API for the SCOOP object model and system. It provides support for the
   following:
   - Class declaration and definition
   - Single inheritance (by direct inclusion of the members of the supertype
     into the subtype)
   - Virtual functions
   - Explicit RTTI checks

   In addition to such "full-fledged" types, support is also provided for
   declaring simple structs with single inheritance (by direct inclusion of
   members). Whether this or a "real" class makes sense to use depends on
   the specific requirements of each case.

   Classes can be instantiated using either static or dynamic memory
   allocations. sco_finalize() is used to destroy the instance without
   deallocating its memory, while sco_delete() also deallocates its memory.

   Instance creation functions for a given class are declared and defined
   using a convenience macro for brevity, and use a utility function to deal
   with memory allocation and initialization of the instance.

   There is no need to "register" a class before allocating an instance;
   the meta type will become fully initialized the first time an instance
   is allocated.

   A note on the API naming convention:
   - "sco"-prefix (no underscore), camel case after.
     Typedefs and plain keyword-like macros.
   - "sco_"-prefix, lowercase after.
     Functions, and macros that behave like normal functions.
   - "SCO"-prefix (no underscore), lowercase after.
     Key feature macros like SCOclassdef() with special behavior.
   - "SCO_"-prefix, uppercase after.
     Enums, constants, miscellaneous macros.
 */

/*
 * OOC inheritance-enabled structs.
 */

/** Declare a type *without* a meta type, i.e. a plain struct.
  * (There is no support for passing such data to sco_delete(),
  * sco_finalize(), or trying to use RTTI or virtual functions.)
  *
  * \p Name is the name of the type to declare. The existence of
  * a macro, having the same name except with "_S_" appended, is
  * expected. That macro should list all the members of the type,
  * the way they are listed within a struct declaration; it
  * will be referenced as a means of declaring the type.
  *
  * A member list macro can reference one other SCOstructdef()
  * type's member list macro at the beginning, giving rise to single
  * inheritance and allowing type-cast pseudo-compatibility. (Padding
  * at the end of a base type struct may overlap with members added
  * in a derived type struct. To avoid this, inherited members can be
  * wrapped in an anonymous struct. This works for non-class types.)
  */
#define SCOstructdef(Name) \
typedef struct Name { Name##_S_ } Name;

/*
 * OOC full-fledged classes.
 */

/** Declare a type with an associated meta type, i.e. a class.
  * (Such data can be passed to sco_delete(), sco_finalize(), and the
  * RTTI and virtual table system used.) Each such struct begins
  * with a \a meta pointer to its meta type.
  *
  * \p Name is the name of the type to declare. The existence of
  * a macro, having the same name except with "_C_" appended, is
  * expected. That macro should list all the members of the type,
  * the way they are listed within a struct declaration; it
  * will be referenced as a means of declaring the type.
  *
  * A member list macro can reference one other SCOclassdef()/SCOclasstype()
  * type's member list macro at the beginning, giving rise to single
  * inheritance and allowing type-cast pseudo-compatibility.
  *
  * \ref SCOclassdef() combines this and \ref SCOmetatype() into a single
  * keyword-like macro.
  */
#define SCOclasstype(Name) \
typedef struct Name { const struct Name##_Meta *meta; Name##_C_ } Name;

#ifndef SCO__DTOR_F_DEFINED
# define SCO__DTOR_F_DEFINED
/** Class destructor function pointer type. */
typedef void (*scoDtor_f)(void *o);
#endif

/** Meta type vtable initializer function pointer type.
    The meta type instance is expected as the \p o argument. */
typedef void (*scoVtinit_f)(void *o);

/** Declare a meta type for a type declared with SCOclassdef().
  * the name of this type seldom needs to be explicitly referenced,
  * but is the same as that of the class with _Meta appended.
  *
  * This version \a will \a also forward-declare the corresponding global
  * instance made by SCOmetainst() for symbol export, as is done for
  * public APIs. To avoid that, use \ref SCOmetatype_() instead.
  *
  * The declaration uses the corresponding macro listing virtual
  * methods - named the same as the class except for having \a two
  * appended underscores - which should contain a sequence of
  * function pointer declarations.
  *
  * That macro can reference one other such macro at the beginning of
  * its contents for (single) inheritance - and this must be done when
  * inheriting from another class. The resulting list of function
  * pointers form the contents of the virtual table data structure for the
  * class, named the same as the class except with _Virt appended.
  *
  * SCOclassdef() combines this and SCOclasstype() into a single step.
  */
#define SCOmetatype(Class) \
SCOmetatype_(Class) \
SCO_USERAPI extern Class##_Meta Class##_meta;

/** Declare a meta type for a type declared with SCOclassdef();
  * the name of this type seldom needs to be explicitly referenced,
  * but is the same as that of the class with _Meta appended.
  *
  * This version \a does \a not forward-declare the corresponding global
  * instance made by \ref SCOmetainst() for symbol export.
  *
  * \see SCOmetatype()
  *
  * SCOclassdef_() combines this and SCOclasstype() into a single step.
  */
#define SCOmetatype_(Class) \
typedef struct Class##_Virt { scoDtor_f dtor; Class##_V_ } Class##_Virt; \
typedef struct Class##_Meta { \
	const struct scoObject_Meta *super; \
	size_t size; \
	unsigned short vnum; \
	scoFakeBool done; \
	const char *name; \
	scoVtinit_f vtinit; /* virtual table init function, passed meta */ \
	Class##_Virt virt; \
} Class##_Meta;

/** Get the global meta type instance of the \p Class named.
  *
  * This requires it to have been either forward-declared with
  * SCOmetatype() (as is done for public APIs) when the class was
  * defined - or to have been otherwise defined earlier in the same
  * module if part of a non-public API.
  *
  * Supplying the keyword \a scoNone as the class will produce a NULL
  * pointer.
  */
#define sco_metaof(Class) (&(Class##_meta))

/** This combines SCOclasstype() and SCOmetatype() to declare a class
  * and its meta type at once - and forward-declare the symbol of
  * the SCOmetainst() definition of the global meta type for
  * export, as is done for public APIs.
  * \see SCOclasstype()
  * \see SCOmetatype()
  */
#define SCOclassdef(Class) \
SCOclasstype(Class) \
SCOmetatype(Class)

/** This combines SCOclasstype() and SCOmetatype_() to declare a class
  * and its meta type at once.
  * \see SCOclasstype()
  * \see SCOmetatype_()
  */
#define SCOclassdef_(Class) \
SCOclasstype(Class) \
SCOmetatype_(Class)

/** Use to declare a set of allocation and constructor functions for a
  * class if they do not take variable arguments. They will include a
  * FunctionName_new() and a FunctionName_ctor() for \p FunctionName
  * (if \p NameSuffix is empty), and both functions take exactly the
  * the same arguments, the parameters given by \p Parlist.
  *
  * A FunctionName_mpnew() variation will also be included, which
  * replaces the first parameter with one for a mempool instance.
  *
  * If \p FunctionName is blank, \p Class will be used for it.
  * Optionally \p NameSuffix can be non-blank to add a suffix to the
  * names in the set of functions, useful for adding several sets.
  *
  * Any number of these function sets may be declared and defined.
  *
  * \ref SCOctordef() is used to define an allocation/construction
  * function set, whether or not it was forward-declared using this.
  *
  * See \ref SCOctordef() for further details.
  */
#define SCOctordec(Class, FunctionName, NameSuffix, Parlist) \
	SCO__ctordec(SCO_USERAPI, Class, \
			SCO_NONEMPTY_OR(FunctionName, Class), \
			NameSuffix, Parlist)

/** Use to declare a set of allocation and constructor functions for a
  * class if they do not take variable arguments. This version is used
  * to forward-declare a static set (not part of any visible API).
  *
  * \see SCOctordec()
  */
#define SCOctordec_(Class, FunctionName, NameSuffix, Parlist) \
	SCO__ctordec(static, Class, \
			SCO_NONEMPTY_OR(FunctionName, Class), \
			NameSuffix, Parlist)

/* Body of SCOctordec() and SCOctordec_(). */
#define SCO__ctordec(Attr, Class, FunctionName, NameSuffix, Parlist) \
Attr Class *SCO_CAT(FunctionName, _new##NameSuffix) Parlist; \
Attr Class *SCO_CAT(FunctionName, _mpnew##NameSuffix) \
SCO_SUBST_HEAD(struct scoMempool *mp, Parlist); \
Attr scoFakeBool SCO_CAT(FunctionName, _ctor##NameSuffix) Parlist;

/** Use to define a set of allocation and constructor functions for a
  * class if they do not take variable arguments. They will include a
  * FunctionName_new() and a FunctionName_ctor() for \p FunctionName
  * (if \p NameSuffix is empty), and both functions take exactly the
  * same arguments, the parameters given by \p Parlist. \p Arglist is
  * used to pass arguments from one of them to the other, and should
  * be \p Parlist without types. The object pointer, whatever it is
  * named, must come first in \p Arglist.
  *
  * The FunctionName_new() function will first allocate zero'd memory
  * if its first, memory pointer argument is zero, otherwise zero and
  * (re)use the memory. Unless an error occurs, the meta type is set,
  * the corresponding FunctionName_ctor() function is called, and the
  * instance is thereafter returned. On error, either sco_delete() or
  * sco_finalize() is used depending on if memory had been allocated.
  *
  * The FunctionName_ctor() function is a constructor which takes a
  * valid memory block - zero'd and with the correct meta type
  * set. Its body is to be defined immediately after the macro
  * invocation, beginning with an opening curly brace; it should return
  * non-zero if construction successful, zero if construction failed.
  *
  * A FunctionName_mpnew() variation will also be included, which
  * replaces the first parameter with one for a mempool instance.
  *
  * If \p FunctionName is blank, \p Class will be used for it.
  * Optionally \p NameSuffix can be non-blank to add a suffix to the
  * names in the set of functions, useful for adding several sets.
  *
  * Note that a constructor for a derived class must explicitly call a
  * constructor for the superclass at the beginning of the function.
  *
  * A caveat: Since subclass constructors call superclass constructors,
  * and the meta type remains that of the instantiated class,
  * this means a constructor might be called with the meta type
  * of a derived class. This affects the definition of virtual
  * functions - so a constructor should not use the virtual function
  * table when it depends on a specific version of the function it calls.
  * Instead, it should then make a direct call to the function as
  * defined for that class.
  *
  * Any number of these function sets may be declared and defined.
  *
  * \see SCOctordec() for simply declaring them as in a header.
  */
#define SCOctordef(Class, FunctionName, NameSuffix, Parlist, Arglist) \
	SCO__ctordef(, Class, \
			SCO_NONEMPTY_OR(FunctionName, Class), \
			NameSuffix, Parlist, Arglist)

/** Use to define a set of allocation and constructor functions for a
  * class if they do not take variable arguments. This version is used
  * to forward-declare a static set (not part of any visible API).
  *
  * \see SCOctordef()
  */
#define SCOctordef_(Class, FunctionName, NameSuffix, Parlist, Arglist) \
	SCO__ctordef(static scoMaybeUnused, Class, \
			SCO_NONEMPTY_OR(FunctionName, Class), \
			NameSuffix, Parlist, Arglist)

/* Body of SCOctordef() and SCOctordef_(). */
#define SCO__ctordef(Attr, Class, FunctionName, NameSuffix, Parlist, Arglist) \
Attr scoFakeBool SCO_CAT(FunctionName, _ctor##NameSuffix) Parlist; \
Attr Class *SCO_CAT(FunctionName, _new##NameSuffix) Parlist \
{ \
	void *SCOctordef__mem = (SCO_ARG1 Arglist); \
	if (((SCO_ARG1 Arglist) = \
	     sco_raw_new(SCOctordef__mem, sco_metaof(Class))) != NULL && \
	    !SCO_CAT(FunctionName, _ctor##NameSuffix) Arglist) { \
		(SCOctordef__mem ? sco_finalize : sco_delete)( \
				(SCO_ARG1 Arglist)); \
		return 0; \
	} \
	return (SCO_ARG1 Arglist); \
} \
Attr Class *SCO_CAT(FunctionName, _mpnew##NameSuffix) \
SCO_SUBST_HEAD(struct scoMempool *SCOctordef__mp, Parlist) \
{ \
	void *SCOctordef__mem; \
	if ((SCOctordef__mem = \
	     sco_raw_mpnew(SCOctordef__mp, sco_metaof(Class))) != NULL && \
	    !SCO_CAT(FunctionName, _ctor##NameSuffix) \
	    SCO_SUBST_HEAD(SCOctordef__mem, Arglist)) { \
		return 0; \
	} \
	return SCOctordef__mem; \
} \
Attr scoFakeBool SCO_CAT(FunctionName, _ctor##NameSuffix) Parlist

/** Define the global instance of the meta type for the class.
  * This version makes the symbol static (not part of a public API).
  *
  * \see SCOmetainst()
  */
#define SCOmetainst_ static SCOmetainst

/** Define the global instance of the meta type for the class.
  *
  * \p Superclass should be \a scoNone for base classes, otherwise the name
  * of the superclass.
  *
  * \p dtor should be the destructor function for the class if it
  * (re)defines one, otherwise NULL. A destructor should call the
  * destructor for the superclass, if any, at the end. If no
  * destructor has been set and none ends up inherited, unlike for
  * other virtual functions, a safe blank no-op function will be set
  * so that calling the superclass destructor using its dtor field is
  * always valid.
  *
  * \p vtinit should be a function setting any other pointers in the virt
  * structure for virtual functions (re)defined by the class. If no
  * virtual functions are (re)defined by the class, it can be NULL. If
  * provided, it will be called upon creation of the first instance of the
  * class, and given the meta type as the argument. It needn't (and
  * shouldn't) change any other pointers: definitions inherited from the
  * superclass are automatically copied, and "pure virtual" (i.e. as-yet
  * undefined) functions are automatically defined to prompt a fatal error
  * (using \ref sco_fatal()) if called.
  */
#define SCOmetainst(Class, Superclass, dtor, vtinit) \
struct Class##_Meta Class##_meta = { \
	(scoObject_Meta*)sco_metaof(Superclass), \
	sizeof(Class), \
	(sizeof(Class##_Virt) / sizeof(void (*)())), \
	0, \
	#Class, \
	(scoVtinit_f)vtinit, \
	{(scoDtor_f)dtor}, \
};

/** The class member list for the dummy type scoObject - it is empty,
  * and does not need to be referenced anywhere.
  */
#define scoObject_C_

/** The virtual method list for the dummy type scoObject - it is empty,
  * and does not need to be referenced anywhere.
  */
#define scoObject_V_

/** Dummy class containing only the meta type pointer; a
  * scoObject pointer and/or cast may be used to access the basic
  * (common) type information of any object of a class declared with
  * SCOclassdef().
  *
  * scoObject is just a dummy definition - not a valid class name!
  */
SCOclasstype(scoObject)

/** Dummy meta type type containing only the common type
  * information; a scoObject_Meta pointer and/or cast may be used to
  * access the information common to any type, ie. all except any
  * virtual methods present.
  *
  * scoObject is just a dummy definition - not a valid class name!
  */
SCOmetatype_(scoObject)

#ifndef SCO_DOXYGEN
/* This is a dummy meta type allowing the keyword \a scoNone
 * to be specified as the supertype for base classes in SCOmetainst().
 */
# define scoNone_meta (*(scoObject_Meta*)(0))
#endif

/** Assuming \p mem points to a valid object, retrieves the class
  * description through typecasting, allowing access to the
  * information common to all classes.
  */
#define sco_meta(mem) \
	((scoObject_Meta*)((scoObject*)mem)->meta)

/** Assuming \p mem points to a valid object or to an object under
  * construction, changes the meta type to \p _meta.
  */
#define sco_set_meta(mem, _meta) \
	((void)(((scoObject*)(mem))->meta = (_meta)))

/** Assuming \p mem points to a valid object or to an object under
  * construction, changes the meta type to that of the
  * \p Class named.
  *
  * Supplying the keyword \a scoNone as the class will set it to a NULL
  * pointer.
  */
#define sco_set_metaof(mem, Class) \
	((void)(((scoObject*)mem)->meta = (scoObject_Meta*)sco_metaof(Class)))

/** Call a virtual method named \p func belonging to the
  * class instance given by the second argument, passing
  * the instance, and any additional arguments after it.
  *
  * This convenience macro is meant to simplify calls to
  * dynamically selected versions of functions. When the
  * function doesn't take an object pointer as its first
  * argument, \ref sco_svirt() can instead be used.
  */
#define sco_virt(func, ...) \
	(SCO_ARG1(__VA_ARGS__))->meta->virt.func(__VA_ARGS__)

/** Call a static virtual method named \p func belonging to the
  * class instance given by the second argument. Only arguments
  * after the second argument, if any, are passed for the call.
  *
  * This convenience macro is for virtual functions which don't
  * take the object pointer as their first parameter. Otherwise
  * it is the same as \ref sco_virt().
  */
#define sco_svirt(func, ...) \
	(SCO_ARG1(__VA_ARGS__))->meta->virt.func(SCO_ARGS_TAIL(__VA_ARGS__))

/** Allocation method used in instance creation functions,
  * typically inside the "new"-wrapper around the constructor
  * function, as generated by SCOctordef(). (A *_new()
  * function for a type with an empty constructor can alternatively
  * be defined as a simple wrapper macro around this.)
  *
  * If \a mem is zero, returns a new, zero'd allocation of
  * \a meta->size; if non-zero, zeroes \p mem and returns
  * it.
  *
  * If not done, the final run-time initialization of the type
  * description will be performed.
  *
  * The \a meta pointer of the new object is set to \p
  * meta.
  */
SCO_API void* sco_raw_new(void *mem, void *meta);

/** Memory pool-using version of the allocation method
  * used in instance creation functions, typically
  * inside the "mpnew"-wrapper around the constructor
  * function, as generated by SCOctordef(). (A *_mpnew()
  * function for a type with an empty constructor can alternatively
  * be defined as a simple wrapper macro around this.)
  *
  * Uses \p mp to allocate an instance. If \p meta has a
  * destructor, it will be registered with the mempool.
  *
  * Otherwise behaves like \ref sco_raw_new().
  */
SCO_API void* sco_raw_mpnew(struct scoMempool *mp, void *meta);

/** Destroys object and frees memory, first calling the destructor for
  * the class if any.
  *
  * Should not be used for an object allocated using a mempool.
  */
SCO_API void sco_delete(void *o);

/** Destroys object without freeing memory, calling the destructor for
  * the class if any, and zeroes the type pointer so that the object
  * is left explicitly invalid. The allocation can be reused after this;
  * if it's dynamic, it may later need to be freed with free().
  *
  * Should not be used for an object allocated using a mempool.
  */
SCO_API void sco_finalize(void *o);

/** An underlying function used by the more convenient class type-checking
  * macros:
  * - sco_subclass()
  * - sco_superclass()
  * - sco_of_class()
  * - sco_of_subclass()
  *
  * It checks if \p submeta is a subclass of \p meta.
  * Returns 1 if subclass, 0 if same class, -1 if neither.
  */
SCO_API int sco_rtticheck(const void *submeta, const void *meta);

/** Checks if the named \p Subclass is a subclass of the named \p Class.
    Returns 1 if subclass, 0 if same class, -1 if neither. */
#define sco_subclass(Subclass, Class) \
	sco_rtticheck(sco_metaof(Subclass), sco_metaof(Class))

/** Checks if the named \p Superclass is a superclass of the named \p Class.
    Returns 1 if superclass, 0 if same class, -1 if neither. */
#define sco_superclass(Superclass, Class) \
	sco_rtticheck(sco_metaof(Class), sco_metaof(Superclass))

/** Checks if \p o is an instance of \p Class or of a class derived from it.
    Returns 1 if such an instance, 0 if not. */
#define sco_of_class(o, Class) \
	(sco_rtticheck(((scoObject*)(o))->meta, sco_metaof(Class)) >= 0)

/** Checks if \p o is of a type derived from \p Class.
    Returns 1 if such an instance, 0 if not. */
#define sco_of_subclass(o, Class) \
	(sco_rtticheck(((scoObject*)(o))->meta, sco_metaof(Class)) > 0)

#ifdef __cplusplus
}
#endif
#endif
