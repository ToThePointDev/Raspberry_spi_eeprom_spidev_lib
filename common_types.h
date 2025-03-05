#ifndef _COMMON_TYPES_H_
#define _COMMON_TYPES_H_

typedef enum
{
  e_RETURNVALUE_Success = 0,	  /*0*/
  e_RETURNVALUE_Failure,
  e_RETURNVALUE_ActionStarted,
  e_RETURNVALUE_Ongoing,
  e_RETURNVALUE_IllegalState,
  e_RETURNVALUE_IllegalArgument , /*5*/
  e_RETURNVALUE_Timeout,
  e_RETURNVALUE_SensorTransformationProblem,
  e_RETURNVALUE_BufferOverrun,
  e_RETURNVALUE_NoMem,
  e_RETURNVALUE_ProgrammingError,
  e_RETURNVALUE_NotFound,
  e_RETURNVALUE_NotUnique,
  e_RETURNVALUE_Overrun,
  e_RETURNVALUE_Unknown,
} e_ReturnValue;
  
#endif /*_COMMON_MACROS_H_*/

