/*	==================
 *	FSTree_RsrcFile.cc
 *	==================
 */

#include "Genie/FS/FSTree_RsrcFile.hh"

// POSIX
#include <fcntl.h>
#include <sys/stat.h>

// Genie
#include "Genie/FS/FSSpec.hh"
#include "Genie/FS/FSSpecForkUser.hh"
#include "Genie/FS/FSTree.hh"
#include "Genie/FS/StatFile.hh"
#include "Genie/IO/MacFile.hh"
#include "Genie/Utilities/AsyncIO.hh"


namespace Genie
{
	
	static mode_t file_mode( const FSSpec& file, bool async )
	{
		CInfoPBRec cInfo;
		
		FSpGetCatInfo< FNF_Throws >( cInfo, async, file );
		
		if ( bool locked = cInfo.hFileInfo.ioFlAttrib & kioFlAttribLockedMask )
		{
			return S_IFREG | 0400;
		}
		
		return S_IFREG | 0600;
	}
	
	class FSTree_RsrcFile : public FSTree
	{
		private:
			FSSpec  itsFileSpec;
			bool    itIsOnServer;
		
		public:
			FSTree_RsrcFile( const FSSpec& file, bool onServer )
			:
				FSTree( FSTreeFromFSSpec( file, onServer ),
				        "rsrc",
				        file_mode( file, onServer ) ),
				itsFileSpec( file ),
				itIsOnServer( onServer )
			{
			}
			
			void Stat( struct ::stat& sb ) const;
			
			IOPtr Open( OpenFlags flags ) const
			{
				flags |= itIsOnServer ? O_MAC_ASYNC : 0;
				
				return OpenMacFileHandle( itsFileSpec,
				                          flags,
				                          &Genie::FSpOpenRF,
				                          &New_RsrcForkHandle );
			}
	};
	
	
	FSTreePtr GetRsrcForkFSTree( const FSSpec& file, bool onServer )
	{
		return new FSTree_RsrcFile( file, onServer );
	}
	
	
	void FSTree_RsrcFile::Stat( struct ::stat& sb ) const
	{
		CInfoPBRec cInfo = { 0 };
		
		FSpGetCatInfo< FNF_Throws >( cInfo,
		                             itIsOnServer,
		                             itsFileSpec );
		
		Stat_HFS( itIsOnServer, &sb, cInfo, itsFileSpec.name, true );
	}
	
}

