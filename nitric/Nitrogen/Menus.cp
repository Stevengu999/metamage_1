// Nitrogen/Menus.cp
// -----------------
//
// Maintained by Joshua Juran

// Part of the Nitrogen project.
//
// Written 2003-2006 by Lisa Lippincott and Joshua Juran.
//
// This code was written entirely by the above contributors, who place it
// in the public domain.


#ifndef NITROGEN_MENUS_H
#include "Nitrogen/Menus.h"
#endif

namespace Nitrogen
  {
	MenuManagerErrorsRegistrationDependency::MenuManagerErrorsRegistrationDependency()
	{
		// does nothing, but guarantees construction of theRegistration
	}
	
	
	static void RegisterMenuManagerErrors();
	
	
	class MenuManagerErrorsRegistration
	{
		public:
			MenuManagerErrorsRegistration()  { RegisterMenuManagerErrors(); }
	};
	
	static MenuManagerErrorsRegistration theRegistration;
	
	
   Nucleus::Owned<MenuRef> NewMenu( MenuID           menuID,
                           ConstStr255Param menuTitle )
     {
      MenuRef result = ::NewMenu( menuID, menuTitle );
      if ( result == 0 )
         throw NewMenu_Failed();
      return Nucleus::Owned<MenuRef>::Seize( result );
     }
   
   MenuFont GetMenuFont( MenuRef menu )
     {
      MenuFont result;
      SInt16 fontID;
      ThrowOSStatus( ::GetMenuFont( menu, &fontID, &result.fontSize ) );
      result.fontID = FontID( fontID );
      return result;
     }

   void SetMenuFont( MenuRef menu, FontID inFontID, UInt16 inFontSize )
     {
      ThrowOSStatus( ::SetMenuFont( menu, inFontID, inFontSize ) );
     }
   
   void SetMenuExcludesMarkColumn( MenuRef menu, bool excludesMark )
     {
      ThrowOSStatus( ::SetMenuExcludesMarkColumn( menu, excludesMark ) );
     }

   
   Nucleus::Owned<MenuRef> CreateNewMenu( MenuID inMenuID, MenuAttributes inMenuAttributes )
      {
        MenuRef result;
       ThrowOSStatus( ::CreateNewMenu( inMenuID, inMenuAttributes, &result ) );
       return Nucleus::Owned<MenuRef>::Seize ( result );
      }
   
   MenuItemIndex AppendMenuItemTextWithCFString( MenuRef            inMenu,
                                                 CFStringRef        inString,
                                                 MenuItemAttributes inAttributes,
                                                 MenuCommand        inCommandID )
     {
      MenuItemIndex result;
      ThrowOSStatus ( ::AppendMenuItemTextWithCFString( inMenu, inString, inAttributes, inCommandID, &result ) );
      return result;
     }

   void InsertMenuItemTextWithCFString( MenuRef            inMenu,
                                        CFStringRef        inString,
                                        MenuItemIndex      inAfterItem,
                                        MenuItemAttributes inAttributes,
                                        MenuCommand        inCommandID )
     {
      ThrowOSStatus ( ::InsertMenuItemTextWithCFString( inMenu, inString, inAfterItem, inAttributes, inCommandID ) );
     }

	Nucleus::Owned< MenuID > MacInsertMenu( MenuRef menu, MenuID beforeID )
	{
		::MacInsertMenu( menu, beforeID );
		return Nucleus::Owned< MenuID >::Seize( Nitrogen::GetMenuID( menu ) );
	}
	
	Str255 GetMenuItemText( MenuRef menu, SInt16 item )
	{
		Str255 itemText;
		::GetMenuItemText( menu, item, itemText );
		return itemText;
	}
	
   void ChangeMenuItemAttributes( MenuRef            menu,
                                  MenuItemIndex      item,
                                  MenuItemAttributes setTheseAttributes,
                                  MenuItemAttributes clearTheseAttributes )
     {
      ThrowOSStatus( ::ChangeMenuItemAttributes( menu, item, setTheseAttributes, clearTheseAttributes ) );
     }
   
   void RegisterMenuManagerErrors()
     {
      RegisterOSStatus< paramErr                >();
      RegisterOSStatus< memFullErr              >();
      RegisterOSStatus< resNotFound             >();
      RegisterOSStatus< hmHelpManagerNotInited  >();
      RegisterOSStatus< menuPropertyInvalidErr  >();
      RegisterOSStatus< menuPropertyNotFoundErr >();
      RegisterOSStatus< menuNotFoundErr         >();
      RegisterOSStatus< menuUsesSystemDefErr    >();
      RegisterOSStatus< menuItemNotFoundErr     >();
      RegisterOSStatus< menuInvalidErr          >();
     }
  }
