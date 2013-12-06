/* SCOOP Object module
 *
 * Copyright (c) 2010, 2011, 2013 Joel K. Pettersson
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

   There is no need to "register" a class before allocating an instance. The
   class description will become fully initialized the first time an instance
   is allocated.

   A note on the SCOOP API naming convention:
   - Declarations and definitions meant to mimic new keywords are named
     with an "SCO"-prefix (no underscore follows), the rest of the name
     then in lowercase. E.g., SCOclass.
   - Functions and macros that behave like functions are named with an
     "sco_"-prefix and the rest of the name in lowercase, unless methods
     of a class.
   - Macros that don't behave like functions are generally named with an
     "SCO_"-prefix and the rest of the name in uppercase, unless methods
     of a class.
   - For methods of a class, the name of the class, followed by an
     underscore, prefixes the name. If the method is a macro which doesn't
     behave like a function, then the rest of the name is in all-uppercase
     letters; otherwise, the rest of the name is in all-lowercase letters.
 */

/*
 * SCOOP OOC inheritance-enabled structs.
 */

/**
 * Declare a SCOOP type without a class description. (And so without
 * support for sco_delete()/sco_finalize(), SCOOP RTTI and virtual
 * functions.)
 *
 * \p Struct is the name of the type to declare. The existence of a
 * macro, having the same name except with an appended underscore, is
 * expected. That macro should list all the members of the type, the
 * way they are listed within a struct declaration; it will be
 * referenced as a means of declaring the type.
 *
 * A member list macro can reference one other SCOstruct() type's
 * member list macro at the beginning, giving rise to single
 * inheritance and allowing type-cast compatibility.
 */
#define SCOstruct(Struct) \
typedef struct Struct { Struct##_ } Struct

/*
 * SCOOP OOC full-fledged classes.
 */

/**
 * Declare a SCOOP type having a class description. (And so having
 * support for sco_delete()/sco_finalize(), SCOOP RTTI and the vtable
 * system.) These are referred to as classes. A \a classdesc pointer to the
 * class description is accordingly included at the beginning of the data
 * structure.
 *
 * \p Class is the name of the type to declare. The existence of a
 * macro, having the same name except with an appended underscore, is
 * expected. That macro should list all the members of the type, the
 * way they are listed within a struct declaration; it will be
 * referenced as a means of declaring the type.
 *
 * A member list macro can reference one other
 * SCOclass()/SCOclasstype() type's member list macro at the
 * beginning, giving rise to single inheritance and allowing type-cast
 * compatibility.
 *
 * \ref SCOclass() combines this and \ref SCOclassdesc() into a single
 * "keyword".
 */
#define SCOclasstype(Class) \
  typedef struct Class { const struct Class##_CD *classdesc; Class##_ } Class

/**
 * Class destructor function pointer type.
 */
typedef void (*scoDtor)(void *o);

/**
 * Declare a class description for a type declared with SCOclass();
 * the name of this type seldom needs to be explicitly referenced,
 * but is the same as that of the class with _CD appended.
 *
 * This version \a does \a not forward-declare the corresponding global
 * instance made by SCO_CLASSDEF() for symbol export.
 *
 * The declaration uses the corresponding macro listing virtual
 * methods - named the same as the class except for having \a two
 * appended underscores - which should contain a sequence of
 * function pointer declarations.
 *
 * That macro can reference one other such macro at the beginning of
 * its contents for (single) inheritance - and this must be done when
 * inheriting from another class. The resulting list of function
 * pointers form the contents of the vtable data structure for the
 * class, named the same as the class except with _VT appended.
 *
 * _SCOclass() combines this and SCOclasstype() into a single
 * "keyword".
 */
#define _SCOclassdesc(Class) \
typedef struct Class##_VT { Class##__ } Class##_VT; \
typedef struct Class##_CD { \
	const struct scoObject_CD *super; \
	size_t size; \
	unsigned short vnum; \
	unsigned char done; \
	const char *name; \
	scoDtor vtinit; /* vtab initialization function; passed classdesc */ \
	scoDtor dtor; /* not counted in vnum (is not part of vtab) */ \
	Class##_VT vtab; \
} Class##_CD

/** Declare a class description for a type declared with SCOclass().
  * the name of this type seldom needs to be explicitly referenced,
  * but is the same as that of the class with _CD appended.
  *
  * This version \a will \a also forward-declare the corresponding global
  * instance made by SCO_CLASSDEF() for symbol export, as is done for
  * public APIs.
  *
  * The declaration uses the corresponding macro listing virtual
  * methods - named the same as the class except for having \a two
  * appended underscores - which should contain a sequence of
  * function pointer declarations.
  *
  * That macro can reference one other such macro at the beginning of
  * its contents for (single) inheritance - and this must be done when
  * inheriting from another class. The resulting list of function
  * pointers form the contents of the vtable data structure for the
  * class, named the same as the class except with _VT appended.
  *
  * SCOclass() combines this and SCOclasstype() into a single "keyword".
  */
#define SCOclassdesc(Class) \
_SCOclassdesc(Class); \
SCO_USERAPI extern Class##_CD _##Class##_cd

/** Get the global class description of the \p Class named.
  *
  * This requires it to have been either forward-declared with
  * SCOclassdesc() (as is done for public APIs) when the class was
  * defined - or to have been otherwise defined earlier in the same
  * module if part of a non-public API.
  *
  * Supplying the keyword \a scoNull as the class will produce a NULL
  * pointer.
  */
#define sco_classdescof(Class) (&(_##Class##_cd))

/** This combines SCOclasstype() and _SCOclassdesc() to declare a class
  * and its class description at once.
  * \see SCOclasstype()
  * \see _SCOclassdesc()
  */
#define _SCOclass(Class) \
SCOclasstype(Class); \
_SCOclassdesc(Class)

/** This combines SCOclasstype() and SCOclassdesc() to declare a class
  * and its class description at once - and forward-declare the symbol of
  * the SCO_CLASSDEF() definition of the global class description for
  * export, as is done for public APIs.
  * \see SCOclasstype()
  * \see SCOclassdesc()
  */
#define SCOclass(Class) \
SCOclasstype(Class); \
SCOclassdesc(Class)

/** Use to declare a pair of allocation and constructor functions for a
  * class if they do not take variable arguments. They will be named
  * FunctionName_new() and FunctionName_ctor(), and both functions take
  * the exact same arguments, the parameters given by \p Parlist.
  * Any number of these function pairs may be declared and defined.
  *
  * \ref SCO_CLASSCTOR() is used to define an allocation/construction
  * function pair declared with SCOclassctor().
  *
  * \see SCO_CLASSCTOR() for further details.
  */
#define SCOclassctor(Class, FunctionName, Parlist) \
SCO_USERAPI Class* FunctionName##_new Parlist; \
SCO_USERAPI unsigned char FunctionName##_ctor Parlist

/** Use to define a pair of allocation and constructor functions for a
  * class if they do not take variable arguments. Both functions take
  * the exact same arguments, given by \p Arglist. This is mirrored in
  * \p Parlist, which should be \p Arglist without types, used for
  * calling the FunctionName_ctor() function.
  *
  * The FunctionName_new() function will first allocate zero'd
  * memory if its memory pointer argument is zero, otherwise zero and
  * (re)use memory. If allocation is successful, it will thereafter set
  * the class description, and then call the corresponding
  * FunctionName_ctor() function. If everything succeeds, the address of
  * the instance is returned, otherwise NULL is returned.
  *
  * The FunctionName_ctor() function is a constructor which takes a
  * valid memory block - zero'd and with the correct class description
  * set. Its body is to be defined immediately after the macro
  * invocation, beginning with an opening curly brace; it should return
  * non-zero if construction successful, zero if construction failed.
  *
  * Note that a constructor for a derived class must explicitly call a
  * constructor for the superclass at the beginning of the function.
  *
  * A caveat: Since subclass constructors call superclass constructors,
  * and the class description remains that of the instantiated class,
  * this means a constructor might be called with the class description
  * of a derived class. This affects the definition of virtual
  * functions - so a constructor should not use the virtual function
  * table when it depends on a specific version of the function it calls.
  * Instead, it should then make a direct call to the function as
  * defined for that class.
  *
  * Any number of these function pairs may be declared and defined.
  *
  * \see SCOclassctor() for declaring them.
  */
#define SCO_CLASSCTOR(Class, FunctionName, Parlist, Arglist, o) \
unsigned char FunctionName##_ctor Parlist; \
Class* FunctionName##_new Parlist \
{ \
	void *SCO_CLASSCTOR__mem = (o); \
	(o) = _sco_new(SCO_CLASSCTOR__mem, sco_classdescof(Class)); \
	if ((o) && !FunctionName##_ctor Arglist) { \
		sco_set_classdescof((o), scoNull); \
		if (!SCO_CLASSCTOR__mem) free(o); \
		return 0; \
	} \
	return (o); \
} \
unsigned char FunctionName##_ctor Parlist

/** Define the global instance of the class description for the class.
  * (You can use the keyword \a static before invoking this macro when not
  * defining a class part of a public API.)
  *
  * \p Superclass should be \a scoNull for base classes, otherwise the name
  * of the superclass.
  *
  * \p dtor should be the destructor function for the class if it defines
  * one, otherwise NULL. Upon object destruction, destructors will be
  * called up the superclass chain from the object's class to the base
  * class.
  *
  * \p vtinit should be a function setting the pointers in the vtab
  * structure for any virtual functions (re)defined by the class. If no
  * virtual functions are (re)defined by the class, it can be NULL. If
  * provided, it will be called upon creation of the first instance of the
  * class, and given the class description as the argument. It needn't (and
  * shouldn't) change any other pointers: definitions inherited from the
  * superclass are automatically copied, and "pure virtual" (i.e. as-yet
  * undefined) functions are automatically defined to prompt a fatal error
  * (using \ref sco_fatal()) if called.
  */
#define SCO_CLASSDEF(Class, Superclass, dtor, vtinit) \
struct Class##_CD _##Class##_cd = { \
	(scoObject_CD*)sco_classdescof(Superclass), \
	sizeof(Class), \
	(sizeof(Class##_VT) / sizeof(void (*)())), \
	0, \
	#Class, \
	(scoDtor)vtinit, \
	(scoDtor)dtor, \
	{} \
}

/** The member content list for the dummy type scoObject - it is empty,
  * and does not need to be referenced anywhere.
  */
#define scoObject_

/** The virtual method list for the dummy type scoObject - it is empty,
  * and does not need to be referenced anywhere.
  */
#define scoObject__

/** Dummy class containing only the class description pointer; a
  * scoObject pointer and/or cast may be used to access the basic
  * (common) type information of any object of a class declared with
  * SCOclass().
  *
  * scoObject is just a dummy definition - not a valid class name!
  */
SCOclasstype(scoObject);

/** Dummy class description type containing only the common type
  * information; a scoObject_CD pointer and/or cast may be used to
  * access the information common to any type, ie. all except any
  * virtual methods present.
  *
  * scoObject is just a dummy definition - not a valid class name!
  */
_SCOclassdesc(scoObject);

#ifndef SCO_DOXYGEN
/* This is a dummy class description allowing the keyword \a scoNull
 * to be specified as the supertype for base classes in SCO_CLASSDEF().
 */
# define _scoNull_cd (*(scoObject_CD*)(0))
#endif

/** Assuming \p mem points to a valid object, retrieves the class
  * description through typecasting, allowing access to the
  * information common to all classes.
  */
#define sco_classdesc(mem) \
	((scoObject_CD*)((scoObject*)mem)->classdesc)

/** Assuming \p mem points to a valid object or to an object under
  * construction, changes the class description to \p _classdesc.
  */
#define sco_set_classdesc(mem, _classdesc) \
	((void)(((scoObject*)(mem))->classdesc = (_classdesc)))

/** Assuming \p mem points to a valid object or to an object under
  * construction, changes the class description to that of the
  * \p Class named.
  *
  * Supplying the keyword \a scoNull as the class will set it to a NULL
  * pointer.
  */
#define sco_set_classdescof(mem, Class) \
	((void)(((scoObject*)mem)->classdesc = (scoObject_CD*)sco_classdescof(Class)))

/** Get the vtable (class-specific) of an object.
  */
#define sco_vtab(o) \
	(&(o)->classdesc->vtab)

/** Allocation method used in instance construction functions,
  * typically in the wrapper around the initialization
  * function generated by SCO_CLASSCTOR(). (a *_new() function for a
  * type with an empty constructor can be defined as a wrapper
  * macro around this)
  *
  * If \a mem is zero, returns a new, zero'd allocation of
  * \a classdesc->size; if non-zero, zeroes \p mem and returns
  * it.
  *
  * If not done, the final run-time initialization of the type
  * description will be performed.
  *
  * The \a classdesc pointer of the new object is set to \p
  * classdesc.
  */
SCO_API void* _sco_new(void *mem, void *classdesc);

/** Destroys object and frees memory, first calling every destructor in
  * the class hierarchy from present type to base type.
  */
SCO_API void sco_delete(void *o);

/** Destroys object without freeing memory, calling every destructor in
  * the class hierarchy from present type to base type, and then zeroes
  * the type pointer so that the object is left explicitly invalid.
  *
  * The allocation can then be reused - and/or, if dynamic, later freed
  * with free().
  */
SCO_API void sco_finalize(void *o);

/** An underlying function used by the more convenient class type-checking
  * macros:
  * - sco_subclass()
  * - sco_superclass()
  * - sco_of_class()
  * - sco_of_subclass()
  *
  * It checks if \p subclassdesc is a subclass of \p classdesc.
  * Returns 1 if subclass, 0 if same class, -1 if neither.
  */
SCO_API int _sco_rtticheck(const void *subclassdesc, const void *classdesc);

/** Checks if the named \p Subclass is a subclass of the named \p Class.
  * Returns 1 if subclass, 0 if same class, -1 if neither.
  */
#define sco_subclass(Subclass, Class) \
	_sco_rtticheck(sco_classdescof(Subclass), sco_classdescof(Class))

/** Checks if the named \p Superclass is a superclass of the named \p Class.
  * Returns 1 if superclass, 0 if same class, -1 if neither.
  */
#define sco_superclass(Superclass, Class) \
	_sco_rtticheck(sco_classdescof(Class), sco_classdescof(Superclass))

/** Checks if \p o is an instance of \p Class or of a class derived
    from it. */
#define sco_of_class(o, Class) \
	(_sco_rtticheck((o)->classdesc, sco_classdescof(Class)) >= 0)

/** Checks if \p o is of a type derived from \p Class. */
#define sco_of_subclass(o, Class) \
	(_sco_rtticheck((o)->classdesc, sco_classdescof(Class)) > 0)

#ifdef __cplusplus
}
#endif
#endif

/* EOF */
