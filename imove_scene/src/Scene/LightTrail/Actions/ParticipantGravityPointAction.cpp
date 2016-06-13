#include <iostream>
#include "ParticipantGravityPointAction.h"
#include "../../../../../scene_interface/src/Person.h"

ParticipantGravityPointAction::ParticipantGravityPointAction(std::shared_ptr<LightPerson> person,
                                                             GravityPointRepository* gravityPoints,
                                                             const LightTrailConfiguration& config)
	: gravityPoints(gravityPoints), person(person) {
		std::cerr << "PGPA: " << person->getLocation().x << "," << person->getLocation().y << " lb: " << person->hue.lowerBound << " ub: " << person->hue.upperBound << std::endl; 
    gravityPoint = std::shared_ptr<GravityPoint>(new GravityPoint(
            Vector2(0,0),person->hue,config.participantGravity(),config.participantGravityRange()));

    util::Range antiHue = person->hue;
    antiHue += 180;
    antigravityPoint = std::shared_ptr<GravityPoint>(
            new GravityPoint(Vector2(0,0),antiHue,-config.participantAntigravity(),config.participantGravityRange())
    );
    // Register the gravity point
    gravityPoints->add(gravityPoint);
    gravityPoints->add(antigravityPoint);

    setLocation();
}

void ParticipantGravityPointAction::setLocation() {
    gravityPoint->location.x = person->getLocation().x;
    gravityPoint->location.y = person->getLocation().y;
    antigravityPoint->location.x = person->getLocation().x;
    antigravityPoint->location.y = person->getLocation().y;
}


bool ParticipantGravityPointAction::isDone(std::vector<Action*> &followUp) {
    // This action is done when the person it tracks is no longer a participant
    if(person->person_type != scene_interface::Person::PersonType::Participant && person->movement_type != scene_interface::Person::MovementType::StandingStill){
        gravityPoints->scheduleForRemoval(gravityPoint);
        gravityPoints->scheduleForRemoval(antigravityPoint);
        return true;
    }
    return false;
}

void ParticipantGravityPointAction::execute(float dt) {
    setLocation();
    gravityPoint->hue = person->hue;
    antigravityPoint->hue = person->hue;
    antigravityPoint->hue += 180;
}
