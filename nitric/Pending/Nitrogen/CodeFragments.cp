// CodeFragments.cp

#ifndef NITROGEN_CODEFRAGMENTS_H
#include "Nitrogen/CodeFragments.h"
#endif

#ifndef NITROGEN_MACERRORS_H
#include "Nitrogen/MacErrors.h"
#endif
#ifndef NITROGEN_OSSTATUS_H
#include "Nitrogen/OSStatus.h"
#endif

namespace Nitrogen
{
	
	template < class ErrorCode >
	void ThrowOSStatusErrName( ConstStr255Param errMessage )
	{
		throw OSStatusErrMessage< ErrorCode >( errMessage );
	}
	
	static void ThrowOSStatusErrName( OSStatus status, ConstStr255Param errMessage )
	{
		switch ( status )
		{
			case noErr:
				return;
				break;
			
			case paramErr:
				throw ParamErr();
				break;
			
			case cfragNoLibraryErr:     ThrowOSStatusErrName< CFragNoLibraryErr    >( errMessage );
			case cfragUnresolvedErr:    ThrowOSStatusErrName< CFragUnresolvedErr   >( errMessage );
			case cfragNoPrivateMemErr:  ThrowOSStatusErrName< CFragNoPrivateMemErr >( errMessage );
			case cfragNoClientMemErr:   ThrowOSStatusErrName< CFragNoClientMemErr  >( errMessage );
			case cfragInitOrderErr:     ThrowOSStatusErrName< CFragInitOrderErr    >( errMessage );
			case cfragImportTooOldErr:  ThrowOSStatusErrName< CFragImportTooOldErr >( errMessage );
			case cfragImportTooNewErr:  ThrowOSStatusErrName< CFragImportTooNewErr >( errMessage );
			case cfragInitLoopErr:      ThrowOSStatusErrName< CFragInitLoopErr     >( errMessage );
			case cfragLibConnErr:       ThrowOSStatusErrName< CFragLibConnErr      >( errMessage );
			case cfragInitFunctionErr:  ThrowOSStatusErrName< CFragInitFunctionErr >( errMessage );
			
			default:
				ThrowOSStatus( status );
		}
	}
	
	void GetSharedLibrary( ConstStr63Param     libName,
	                       CFragArchitecture   archType,
	                       CFragLoadOptions    options,
	                       CFragConnectionID*  connID,
	                       SymbolAddressPtr*   mainAddr )
	{
		OnlyOnce< RegisterCodeFragmentManagerErrors >();
		
		::Ptr tempMainAddr;
		Str255 errMessage;
		
		ThrowOSStatusErrName( ::GetSharedLibrary( libName,
		                                          archType,
		                                          options,
		                                          connID,
		                                          &tempMainAddr,
		                                          errMessage ),
		                       errMessage );
		
		if ( mainAddr != NULL )
		{
			*mainAddr = reinterpret_cast< SymbolAddressPtr >( tempMainAddr );
		}
	}
	
	void GetDiskFragment( const FSSpec&       file,
	                      std::size_t         offset,
	                      std::size_t         length,
	                      ConstStr63Param     fragName,
	                      CFragLoadOptions    options,
	                      CFragConnectionID*  connID,
	                      SymbolAddressPtr*   mainAddr )
	{
		OnlyOnce< RegisterCodeFragmentManagerErrors >();
		
		::Ptr tempMainAddr;
		Str255 errMessage;
		
		// This works for all result codes that GetDiskFragment() might throw.
		// Ideally, it should work for any registered OSStatus, but that would take some work.
		// It may require substantial modification and extension of the ErrorCode mechanism.
		
		ThrowOSStatusErrName( ::GetDiskFragment( &file,
		                                         offset,
		                                         length,
		                                         fragName,
		                                         options,
		                                         connID,
		                                         &tempMainAddr,
		                                         errMessage ),
		                       errMessage );
		
		if ( mainAddr != NULL )
		{
			*mainAddr = reinterpret_cast< SymbolAddressPtr >( tempMainAddr );
		}
	}
	
	void GetMemFragment( const void*         memAddr,
	                     std::size_t         length,
	                     ConstStr63Param     fragName,
	                     CFragLoadOptions    options,
	                     CFragConnectionID*  connID,
	                     SymbolAddressPtr*   mainAddr )
	{
		OnlyOnce< RegisterCodeFragmentManagerErrors >();
		
		::Ptr tempMainAddr;
		Str255 errMessage;
		
		ThrowOSStatusErrName( ::GetMemFragment( const_cast< void* >( memAddr ),
		                                        length,
		                                        fragName,
		                                        options,
		                                        connID,
		                                        &tempMainAddr,
		                                        errMessage ),
		                       errMessage );
		
		if ( mainAddr != NULL )
		{
			*mainAddr = reinterpret_cast< SymbolAddressPtr >( tempMainAddr );
		}
	}
	
	void CloseConnection( Owned< CFragConnectionID > connID )
	{
		OnlyOnce< RegisterCodeFragmentManagerErrors >();
		
		CFragConnectionID connIDcopy = connID.Release();
		
		ThrowOSStatus( ::CloseConnection( &connIDcopy ) );
	}
	
	void FindSymbol( CFragConnectionID  connID, 
	                 ConstStr255Param   symName, 
	                 SymbolAddressPtr*  symAddr, 
	                 CFragSymbolClass*  symClass )
	{
		OnlyOnce< RegisterCodeFragmentManagerErrors >();
		
		::Ptr tempSymAddr;
		::CFragSymbolClass tempSymClass;
		
		ThrowOSStatus( ::FindSymbol( connID, symName, &tempSymAddr, &tempSymClass ) );
		
		if ( symAddr != NULL )
		{
			*symAddr = reinterpret_cast< SymbolAddressPtr >( tempSymAddr );
		}
		
		if ( symClass != NULL )
		{
			*symClass = CFragSymbolClass( tempSymClass );
		}
	}
	
	std::size_t CountSymbols( CFragConnectionID connID )
	{
		OnlyOnce< RegisterCodeFragmentManagerErrors >();
		
		long result;
		
		ThrowOSStatus( ::CountSymbols( connID, &result ) );
		
		return result;
	}
	
	void GetIndSymbol( CFragConnectionID  connID,
	                   std::size_t        symIndex,
	                   Str255             symName,
	                   SymbolAddressPtr*  symAddr,
	                   CFragSymbolClass*  symClass )
	{
		OnlyOnce< RegisterCodeFragmentManagerErrors >();
		
		::Ptr tempSymAddr;
		::CFragSymbolClass tempSymClass;
		
		ThrowOSStatus( ::GetIndSymbol( connID,
		                               symIndex,
		                               symName,
		                               &tempSymAddr,
		                               &tempSymClass ) );
		
		if ( symAddr != NULL )
		{
			*symAddr = reinterpret_cast< SymbolAddressPtr >( tempSymAddr );
		}
		
		if ( symClass != NULL )
		{
			*symClass = CFragSymbolClass( tempSymClass );
		}
	}
	
	void RegisterCodeFragmentManagerErrors()
	{
		RegisterOSStatus< paramErr                >();
		RegisterOSStatus< cfragContextIDErr       >();
		RegisterOSStatus< cfragConnectionIDErr    >();
		RegisterOSStatus< cfragNoSymbolErr        >();
		RegisterOSStatus< cfragNoSectionErr       >();
		RegisterOSStatus< cfragNoLibraryErr       >();
		RegisterOSStatus< cfragDupRegistrationErr >();
		RegisterOSStatus< cfragFragmentFormatErr  >();
		RegisterOSStatus< cfragUnresolvedErr      >();
		RegisterOSStatus< cfragNoPrivateMemErr    >();
		RegisterOSStatus< cfragNoClientMemErr     >();
		RegisterOSStatus< cfragNoIDsErr           >();
		RegisterOSStatus< cfragInitOrderErr       >();
		RegisterOSStatus< cfragImportTooOldErr    >();
		RegisterOSStatus< cfragImportTooNewErr    >();
		RegisterOSStatus< cfragInitLoopErr        >();
		RegisterOSStatus< cfragInitAtBootErr      >();
		RegisterOSStatus< cfragLibConnErr         >();
		RegisterOSStatus< cfragCFMStartupErr      >();
		RegisterOSStatus< cfragCFMInternalErr     >();
		RegisterOSStatus< cfragFragmentCorruptErr >();
		RegisterOSStatus< cfragInitFunctionErr    >();
		RegisterOSStatus< cfragNoApplicationErr   >();
		RegisterOSStatus< cfragArchitectureErr    >();
		RegisterOSStatus< cfragFragmentUsageErr   >();
		RegisterOSStatus< cfragLastErrCode        >();
	}
	
}

