/*	===========
 *	TextEdit.hh
 *	===========
 */

// Pedestal
#include "Pedestal/TextEdit.hh"


namespace Genie
{
	
	class FSTree;
	
	
	struct TextEditParameters
	{
		Point                    itsTextDimensions;
		std::string              itsText;
		Pedestal::TextSelection  itsSelection;
		std::size_t              itsValidLength;
		bool                     itHasChangedAttributes;
		bool                     itIsInterlocked;
		bool                     itIsWrapped;
		
		TextEditParameters();
		
		static TextEditParameters* Find( const FSTree* key );
		static TextEditParameters& Get ( const FSTree* key );
		
		static void Erase( const FSTree* key );
	};
	
	
	class TextEdit : public Pedestal::TextEdit
	{
		private:
			typedef const FSTree* Key;
			
			Key itsKey;
			
			Nucleus::Owned< Nitrogen::TEHandle >  itsTE;
			
			Pedestal::TextSelection  itsSelectionPriorToSearch;
			
			void On_UserSelect();
			void On_UserEdit();
			
			void On_EnterKey();
			
			void UpdateText();
			
			void UpdateClientHeight();
			void UpdateScrollOffsets();
			
			void ClickInLoop()  { UpdateScrollOffsets(); }
		
		public:
			TextEdit( Key key ) : itsKey( key )
			{
				itsSelectionPriorToSearch.start = -1;
			}
			
			void Install();
			void Uninstall();
			
			const FSTree* GetKey() const  { return itsKey; }
			
			TEHandle Get() const  { return itsTE; }
			
			void BeginQuasimode();
			void EndQuasimode();
			
			Pedestal::TextSelection GetPriorSelection() const;
			
			void SetPriorSelection( const Pedestal::TextSelection& selection );
			
			bool Wrapped() const;
	};
	
}

