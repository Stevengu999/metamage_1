/*
	functions.cc
	------------
*/

#include "vlib/functions.hh"

// plus
#include "plus/binary.hh"
#include "plus/decode_binoid_int.hh"
#include "plus/hexadecimal.hh"
#include "plus/integer_hex.hh"

// vlib
#include "vlib/error.hh"
#include "vlib/string-utils.hh"
#include "vlib/symbol_table.hh"


namespace vlib
{
	
	void define( const char* name, function_type f )
	{
		const symbol_id sym = create_symbol( name, Symbol_const );
		
		assign_symbol( sym, Value( f, name ) );
	}
	
	static
	plus::string hex( const plus::string& s )
	{
		return plus::hex_lower( s.data(), s.size() );
	}
	
	static
	Value v_str( const Value& value )
	{
		return make_string( value, Stringified_to_print );
	}
	
	static
	Value v_bool( const Value& arg )
	{
		switch ( arg.type )
		{
			default:
				INTERNAL_ERROR( "invalid type in v_bool()" );
			
			case Value_empty_list:
				return false;
			
			case Value_boolean:
				return arg;
			
			case Value_number:
				return ! arg.number.is_zero();
			
			case Value_string:
				return ! arg.string.empty();
			
			case Value_function:
			case Value_pair:
				return true;
		}
	}
	
	static
	Value v_hex( const Value& arg )
	{
		switch ( arg.type )
		{
			default:  TYPE_ERROR( "invalid argument to hex()" );
			
			case Value_number:  return hex( arg.number );
			case Value_string:  return hex( arg.string );
		}
	}
	
	static
	Value v_abs( const Value& arg )
	{
		if ( arg.type != Value_number )
		{
			TYPE_ERROR( "invalid argument to abs()" );
		}
		
		return abs( arg.number );
	}
	
	static
	Value v_half( const Value& arg )
	{
		if ( arg.type != Value_number )
		{
			TYPE_ERROR( "invalid argument to half()" );
		}
		
		return half( arg.number );
	}
	
	bool install_basic_functions()
	{
		define( "abs",  &v_abs  );
		define( "bool", &v_bool );
		define( "half", &v_half );
		define( "hex",  &v_hex  );
		define( "str",  &v_str  );
		
		return true;
	}
	
	Value v_area( const Value& v )
	{
		return area( v );
	}
	
	Value v_rep( const Value& v )
	{
		return make_string( v, Stringified_to_reproduce );
	}
	
	static
	bool is_0x_numeral( const plus::string& s, char x )
	{
		return s.size() > 2  &&  s[ 0 ] == '0'  &&  s[ 1 ] == x;
	}
	
	Value v_unbin( const Value& v )
	{
		if ( v.type != Value_string )
		{
			TYPE_ERROR( "unbin() argument must be a string" );
		}
		
		if ( is_0x_numeral( v.string, 'b' ) )
		{
			return unbin_int( v.string.substr( 2 ) );
		}
		
		return unbin( v.string );
	}
	
	Value v_unhex( const Value& v )
	{
		if ( v.type != Value_string )
		{
			TYPE_ERROR( "unhex() argument must be a string" );
		}
		
		if ( is_0x_numeral( v.string, 'x' ) )
		{
			return unhex_int( v.string.substr( 2 ) );
		}
		
		return unhex( v.string );
	}
	
}