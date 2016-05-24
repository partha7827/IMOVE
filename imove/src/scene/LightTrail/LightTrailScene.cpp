//
// Created by Wouter Posdijk on 11/05/16.
//

#include "LightTrailScene.h"
#include "Actions/UpdateLightTrailsAction.h"
#include "../Util/HueConverter.h"
#include "Actions/UpdateLightSourcesAction.h"
#include "Actions/AlternatingGravityPointAction.h"
#include "Conditions/PersonChangedTypeCondition.h"
#include "Actions/DeleteAllAction.h"

void LightTrailScene::draw(sf::RenderTarget &target) {

    //Slightly fade the current texture
    sf::RectangleShape rect(sf::Vector2f(800, 600));
    rect.setFillColor(sf::Color(0, 0, 0, 10));
    texture.draw(rect);

    //Draw all light trails on the texture
    for(auto &trail : *lightTrails){
        sf::RectangleShape circle(sf::Vector2f(3, 3) );
        circle.setPosition(trail->getLocation().x,trail->getLocation().y);
        circle.setFillColor(HueConverter::ToColor(trail->hue));
        texture.draw(circle);
    }

    //Draw the texture onto the target
    texture.display();
    target.draw(sf::Sprite(texture.getTexture()));

    //Draw all people on the target (for debugging purposes)
    lightPeople->for_each([&](std::shared_ptr<LightPerson> person){
        sf::CircleShape circle(5);
        circle.setFillColor(sf::Color::Cyan);
        circle.setPosition(sf::Vector2f(person->getLocation().x,person->getLocation().y));
        target.draw(circle);
    });

    //Draw all gravitypoints on the target (for debugging purposes)
    for(auto &point : *gravityPoints){
        sf::CircleShape gCircle(4);
        gCircle.setFillColor(sf::Color::Red);
        gCircle.setPosition(point->location.x,point->location.y);
        target.draw(gCircle);
    }

}

LightTrailScene::LightTrailScene() : Scene(),
                                     lightSources(LightSourceVectorRepository::getInstance()),
                                     lightTrails(LightTrailVectorRepository::getInstance()),
                                     gravityPoints(GravityPointVectorRepository::getInstance()),
                                     colorHoles(ColorHoleVectorRepository::getInstance()),
                                     lightPeople(LightPersonMapRepository::getInstance())
{
    //Initialize the light trail texture
    texture.create(800,600);

    //Add Light sources on every corner
    lightSources->add(std::shared_ptr<LightSource>(new LightSource(Vector2(0, 0),util::Range(0, 90,true),util::Range(0, 90,true),util::Range(0,100))));
    lightSources->add(std::shared_ptr<LightSource>(new LightSource(Vector2(800,0),util::Range(90,180,true),util::Range(90, 180,true),util::Range(0,100))));
    lightSources->add(std::shared_ptr<LightSource>(new LightSource(Vector2(0, 600),util::Range(180, 270,true),util::Range(270, 0,true),util::Range(0,100))));
    lightSources->add(std::shared_ptr<LightSource>(new LightSource(Vector2(800, 600),util::Range(270, 360,true),util::Range(180, 270,true),util::Range(0,100))));


    //Add all the basic actions
    actions.push_back(new DeleteAllAction());
    actions.push_back(new UpdateLightTrailsAction());
    actions.push_back(new UpdateLightSourcesAction());
    actions.push_back(new AlternatingGravityPointAction());
    actions.push_back(new AlternatingGravityPointAction());

    //Add all conditions
    conditions.push_back(new PersonChangedTypeCondition());
}

void LightTrailScene::processPeople() {
    if(!peopleQueue.empty()) { //If people have been updated

        //Get the first update and pop it.
        vector<Person> newPeople = peopleQueue.front();
        peopleQueue.pop();

        //Set up tracking of people that are gone
        map<unsigned int,bool> existingPeople;

        //Set up range for generating new hues
        util::Range hueDraw(0, 360, true);

        for (int i = 0; i < newPeople.size(); ++i) {

            Person person = newPeople[i];
            unsigned int id = person.getId();
            existingPeople[id] = true;

            if (lightPeople->has(id)) { //If the person currently exists

                //Update the person
                std::shared_ptr<LightPerson> lPerson = lightPeople->get(id);
                lPerson->setLocation(person.getLocation());
                lPerson->type = person.type;

            } else {

                //Create a new person with randomly generated hue
                float startHue = hueDraw.drawRandom();
                float endHue = startHue + 90;
                lightPeople->add(
                        std::shared_ptr<LightPerson>(new LightPerson(person.getLocation(), id, person.type, util::Range(startHue, endHue, true))));

            }
        }
        lightPeople->for_each([&](std::shared_ptr<LightPerson> person){
            if(existingPeople.count(person->getId()) == 0){ //If this person does not exist anymore
                person->type = None;

                //Remove it from the list
                lightPeople->scheduleForRemoval(person);
            }
        });
    }

    //TODO remove people when they're gone
}





