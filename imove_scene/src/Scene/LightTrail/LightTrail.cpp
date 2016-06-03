#include "LightTrail.h"

LightTrail::LightTrail(scene_interface::Vector2 location, scene_interface::Vector2 speed, float hue) :
    location(location),
    speed(speed),
    hue(hue)
{
}

void LightTrail::applyForce(scene_interface::Vector2 force, float dt, float speedCap, bool sidesEnabled,
    unsigned int screenWidth, unsigned int screenHeight) {
    //Increase the speed based on the force and delta time
    speed += force*dt;

    //Cap the speed at 500
    if(speed.size() > speedCap){
        speed = speed/speed.size()*speedCap;
    }

    //Increase the location based on the speed and delta time
    location += speed*dt;

    //Bounce off the edges of the screen
    if(sidesEnabled) {
        if (location.x < 0) {
            location.x = 0;
            speed.x *= -1;
        } else if (location.x > screenWidth) {
            location.x = screenWidth;
            speed.x *= -1;
        }
        if (location.y < 0) {
            location.y = 0;
            speed.y *= -1;
        } else if (location.y > screenHeight) {
            location.y = screenHeight;
            speed.y *= -1;
        }
    }
}

scene_interface::Vector2 LightTrail::getLocation() {
    return location;
}

scene_interface::Vector2 LightTrail::getSpeed() {
    return speed;
}







