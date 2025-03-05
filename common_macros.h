/**
 * @file
 * @brief This file provides some useful macro helpers.
 */
#ifndef _COMMON_MACROS_H_
#define _COMMON_MACROS_H_



#define MAX_INT8		0x7F
#define MAX_UINT8		0xFF
#define MAX_INT16		0x7FFF
#define MAX_UINT16		0xFFFF
#define MAX_INT32		0x7FFFFFFF
#define MAX_UINT32		0xFFFFFFFF

#ifndef UNUSED
  /* For removing warnings about unused function arguments*/
  //#define UNUSED(x)				do { if( x ) ; } while(0) => does not work when x is a direct struct
  #define UNUSED(x)				( ( x ) = ( x ) )
#endif // UNUSED

/*
 * For places where we do not expect we will pass
 */
#define TODO_UNEXPECTEDEVENT 	do {} while(1)
/*
 * For code which still needs some critical fixing which is part
 * of the regular flow
 */
#define TODO_REGULARFLOW     	do {} while(1)
/*
 * For code which still needs some fixing which is part
 * of the regular flow, but we can already run without it for now
 */
#define SOFT_TODO_REGULARFLOW  	do {} while(0)

#if !(defined(ARRAY_SIZE))
 #define ARRAY_SIZE(arr)   (sizeof(arr) / sizeof((arr)[0]))
#endif /* !defined(ARRAY_SIZE) */


/**
  * Wrapper for macro arguments that are arrays.
  * Normally if you have a single argument which contains ',' it would mean that the single argument is split on ',' to become multiple individual arguments.
  * This behaviour is overuled though if it is present inside a ( ) block.
  */
#define ARRAYWRAPPER(...)	   { __VA_ARGS__ }


#define GET_ABS(x) ( (x)>= 0 ? (x) : -(x))

/*
 *	Return the ceil of a division of positive numbers
 */
#define CEIL_DIV_POS(divident, divisor)		( ( (divident) + ((divisor)-1) ) / (divisor) )
 
/*
 * Return the offset of a member in a struct
 */
#define MEMBER_OFFSET(type, member) ((uint32_t)(&((type *)0)->member))

/*
 * Return the size of a member of a struct
 */
#define MEMBER_SIZE(type, member) (sizeof(((type *)0)->member))

  
/*
 * Calulcate the maximum possible value the variable or type vt can take if it is unsigned
 * The value must fit in uint32_t
 */
#define MAX_POSSIBLE_VALUE_UINT32(vt)	( ( ((uint32_t)1) << ((8*sizeof(vt))-1)) + ( ( ((uint32_t)1) << ((8*sizeof(vt))-1)) - 1) )
/*
 * When using an UINT to presens a -100% to +100% range the 0 lies in the middle
 * This returns the value of that 0 point
 */
#define GET_ZERO_OFFSET( lm_t_or_v )		(( MAX_POSSIBLE_VALUE_UINT32( lm_t_or_v )>>1) + 1)
 
#ifndef STRINGIFY
	#define STR_HELPER(x) 	#x
	#define STRINGIFY(x)	STR_HELPER(x)
#endif

//#define MIN(a,b) ((a)<(b))?(a):(b))
#define MINIMUM3(a,b,c) MIN(MIN((a),(b)),(c))

#define CHAR2INT(x) ( (47 < x) && (x < 58)  ? (x-48) : 0 )
#define ISCHARNB(x) ( (47 < x) && (x < 58)  ? 1 : 0 )


#endif /*_COMMON_MACROS_H_*/

