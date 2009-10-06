// at_flags_t.hh
// -------------

// Written 2009 by Joshua Juran.
//
// This code was written entirely by the above contributor, who places it
// in the public domain.


#ifndef POSEVEN_TYPES_AT_FLAGS_T_HH
#define POSEVEN_TYPES_AT_FLAGS_T_HH

// POSIX
#include <sys/stat.h>

// Nucleus
#include "Nucleus/Flag.h"


namespace poseven
{
	
	enum at_flags_t
	{
		at_symlink_nofollow = AT_SYMLINK_NOFOLLOW,
		
		at_flags_t_max = Nucleus::Enumeration_Traits< int >::max
	};
	
	NUCLEUS_DEFINE_FLAG_OPS( at_flags_t )
	
}

#endif

