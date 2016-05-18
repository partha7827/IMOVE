//
// Created by Wouter Posdijk on 11/05/16.
//

#include "LightTrailScene.h"
#include "Actions/UpdateLightTrailsAction.h"
#include "../Util/HueConverter.h"
#include "Actions/UpdateLightSourcesAction.h"
#include "Actions/AlternatingGravityPointAction.h"

void LightTrailScene::draw(sf::RenderTarget &target) {

    //TODO do drawing here
    sf::RectangleShape rect(sf::Vector2f(2560,1600));
    rect.setFillColor(sf::Color(0,0,0,3));
    target.draw(rect);

    for(auto &trail : *lightTrails){
        sf::RectangleShape circle(sf::Vector2f(3,3) );
        circle.setPosition(trail->getLocation().x,trail->getLocation().y);
        circle.setFillColor(HueConverter::ToColor(trail->hue));
        target.draw(circle);
    }

    for(auto &point : *gravityPoints){
        sf::CircleShape gCircle(8);
        gCircle.setFillColor(sf::Color::Red);
        gCircle.setPosition(point->location.x,point->location.y);
        target.draw(gCircle);
    }

    //sf::Mouse mouse;
    //sf::Vector2i pos = mouse.getPosition();
    //gravityPoints[0]->location.x = pos.x;
    //gravityPoints[0]->location.y = pos.y;
}

LightTrailScene::LightTrailScene() : Scene(),
                                     lightSources(LightSourceRepository::getInstance()),
                                     lightTrails(LightTrailRepository::getInstance()),
                                     gravityPoints(GravityPointRepository::getInstance()),
                                     colorHoles(ColorHoleRepository::getInstance())
                                             {
    //Initialize lists
    lightSources->add(new LightSource(Vector2(0,0),Range(0,90,true),Range(0,90,true),Range(0,400)));
    lightSources->add(new LightSource(Vector2(2560,0),Range(90,180,true),Range(90,180,true),Range(0,400)));
    lightSources->add(new LightSource(Vector2(0,1600),Range(180,270,true),Range(270,0,true),Range(0,400)));
    lightSources->add(new LightSource(Vector2(2560,1600),Range(270,360,true),Range(180,270,true),Range(0,400)));

    actions.push_back(new UpdateLightTrailsAction());
    actions.push_back(new UpdateLightSourcesAction());
    actions.push_back(new AlternatingGravityPointAction());
    actions.push_back(new AlternatingGravityPointAction());
}



