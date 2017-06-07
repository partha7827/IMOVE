#include "BothPaddlesControlledCondition.h"
#include <iostream>

BothPaddlesControlledCondition::BothPaddlesControlledCondition(
	std::shared_ptr<Paddle> p1,
	std::shared_ptr<Paddle> p2,
	PongConfiguration config,
	std::shared_ptr<Ball> ball,
	std::shared_ptr<Score> score
) : p1(p1), p2(p2),config(config), ball(ball), score(score) {}

int BothPaddlesControlledCondition::check(float dt, std::vector<Action*> &actions) {
	// If both paddles are being controlled
  if (p1->isControlled == true && p2->isControlled == true) {
		// If there is no game going, reset scores and start new game
		if (!gameStarted) {
			gameStarted = true;
			//std::cout << "Game started" << std::endl;
			score->player1 = 0;
			score->player2 = 0;
		}

	} else {
		if (gameStarted) {
			if (interactionTimer <= 800) {
				interactionTimer++;

			} else {
				interactionTimer = 0;
				gameStarted = false;
				//std::cout << "Game ended" << std::endl;
				actions.push_back(
					static_cast<Action*>(new RestartBallAction(ball,true,config))
				);
				return 1;
			}
		}
		else {
			actions.push_back(
				static_cast<Action*>(new RestartBallAction(ball,true,config))
			);
			return 1;
		}
		// possibly add countdown
		// display text
		// do not launch ball
	}

  return 0;
}