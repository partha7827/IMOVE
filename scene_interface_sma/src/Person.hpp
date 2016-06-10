#ifndef SCENEINTERFACESMA_PERSON_H
#define SCENEINTERFACESMA_PERSON_H

#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include "Vector2.hpp"

namespace scene_interface_sma {
	//Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
	//This allocator will allow placing containers in the segment
	typedef boost::interprocess::allocator<boost::interprocess::offset_ptr<Vector2>, boost::interprocess::managed_shared_memory::segment_manager> Vector2SMA;
	
	//Alias a vector that uses the previous STL-like allocator so that allocates
	//its values from the segment
	typedef boost::interprocess::vector<boost::interprocess::offset_ptr<Vector2>, Vector2SMA> Vector2Vector;
	
	class Person {
	 public:
			enum PersonType{
			    Bystander,
			    Passerthrough,
			    Participant,
			    None
			};

			enum MovementType{
				StandingStill,
				Moving
			};
	
	    Person(boost::interprocess::offset_ptr<Vector2Vector> locations, PersonType person_type, MovementType movement_type, unsigned int id);
	
	    //Getters
	    const boost::interprocess::offset_ptr<Vector2Vector> getLocations() const;
	    const unsigned int getId() const;
	    const PersonType getPersonType() const;
	    const MovementType getMovementType() const;
	 
	 private:
	    // List of previous locations
	    const boost::interprocess::offset_ptr<Vector2Vector> locations;
	
			// Person Type
			const PersonType person_type;
			
			// Movement Type
			const MovementType movement_type;
	
	    // Id
	    const unsigned int id;
	};
}

#endif //SCENEINTERFACESMA_PERSON_H
