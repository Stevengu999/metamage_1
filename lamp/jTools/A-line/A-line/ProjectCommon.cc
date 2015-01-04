/*	================
 *	ProjectCommon.cc
 *	================
 */

#include "A-line/ProjectCommon.hh"

// C++
#include <map>

// A-line
#include "A-line/Exceptions.hh"
#include "A-line/Includes.hh"
#include "A-line/Project.hh"
#include "A-line/ProjectCatalog.hh"


namespace tool
{
	
	// A map from project name to project data
	typedef std::map< plus::string, Project* > ProjectMap;
	
	// A map from platform to project map
	typedef std::map< Platform, ProjectMap > ProjectPlatformMap;
	
	
	static ProjectPlatformMap gProjectPlatformMap;
	
	
	Project& GetProject( const plus::string& project_name, Platform platform )
	{
		// Cache the platform lookup
		ProjectMap& map = gProjectPlatformMap[ platform ];
		
		// Check for the project
		ProjectMap::iterator it = map.find( project_name );
		
		const bool exists = it != map.end();
		
		if ( !exists )
		{
			// First encounter with this project; load it
			
			const ProjectConfig& config = GetProjectConfig( project_name, platform );
			
			// Take a reference (auto-vivifying)
			Project*& project_ptr = map[ project_name ];
			
			// This gets leaked, but we would have stored it until exit anyway
			project_ptr = new Project( project_name,
			                           platform,
			                           config.get_project_dir(),
			                           config.get_config_data() );
			
			return *project_ptr;
		}
		
		if ( it->second == NULL )
		{
			// Project entry exists but is NULL -- probably trying to load itself
			
			throw circular_dependency( project_name );
		}
		
		return *it->second;
	}
	
}
