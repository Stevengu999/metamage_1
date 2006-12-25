/*	===========
 *	Document.hh
 *	===========
 */

#ifndef USEEDIT_DOCUMENT_HH
#define USEEDIT_DOCUMENT_HH

// Standard C++
#include <memory>

// UseEdit
#include "UseEdit/Window.hh"


namespace UseEdit
{
	
	namespace N = Nitrogen;
	
	class Document
	{
		private:
			Ped::SingleWindowOwner< Window >  fWindow;
			FSSpec                            fFile;
			bool                              fHasFile;
			bool                              fDirtyFlag;
		
		public:
			Document( Ped::WindowClosure& closure );
			Document( Ped::WindowClosure& closure, const FSSpec& file );
			
			Window const& GetWindow() const  { return fWindow.Get(); }
			Window      & GetWindow()        { return fWindow.Get(); }
			
			N::WindowRef GetWindowRef() const  { return GetWindow().Get(); }
			
			std::string GetName() const  { return GetWindow().GetName(); }
	};
	
}

#endif

