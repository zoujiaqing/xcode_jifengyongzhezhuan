#pragma once

class ILogicManager
{
public :
	/*
	* @prototype: goToFrontend()
	* @note:
	*     bring the game to the front of the screen. prepare the resources and init all the game components.
	* @param: void
	* @return: void
	*/
	//virtual void goToFrontend() = 0;

		
	/*
	* @prototype: startGame()
	* @note:
	*     start the game now, bring the player to the hometown.
	* @param: void
	* @return: void
	*/
	virtual void startGame() = 0;

	virtual void exitGame() = 0;
};