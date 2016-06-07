//
// Created by Wouter Posdijk on 30/05/16.
//

#include <cmath>
#include "RevertMixingAction.h"
#include "MixingAction.h"

RevertMixingAction::RevertMixingAction(const std::shared_ptr<LightPerson> &person1, const std::shared_ptr<LightPerson> &person2,
                                       float progress,
                                       LightTrailRepository *trails, const LightTrailConfiguration &config)
        : person1(person1),
          person2(person2),
          trails(trails),
          config(config) {
    progressLeft = progress;
    progPerSecond = progress/config.mixingRevertTime();
}

bool RevertMixingAction::isDone(std::vector<Action*> &followUp) {
    return fabs(progressLeft) < 0.001;
}

void RevertMixingAction::execute(float dt) {
    float currentProgress = progPerSecond*dt;

    if(fabs(currentProgress) >= fabs(progressLeft)){
        currentProgress = progressLeft;
    }

    progressLeft -= currentProgress;

    MixingAction::shift(trails,person1->getLocation(), person1->hue,-currentProgress);
    MixingAction::shift(trails,person2->getLocation(), person2->hue,currentProgress);
}