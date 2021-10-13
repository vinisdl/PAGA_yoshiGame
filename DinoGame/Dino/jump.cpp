#include "Jump.h"

const int jumpVelocity = 10.0;

Jump::Jump(Sprite* sprite, vector<Sprite>* parallax)
{
	this->sprite = sprite;
	jump = false;
	up = false;
	down = false;		
	this->parallax = parallax;
}
void Jump::StartJump()
{
	if (!jump) {		
		jump = true;
	}
}

void Jump::Check()
{	
	if (jump) {
		if (this->sprite->getPositionY() - this->sprite->getInitalPositionY() < 130.0 && !down)
			this->sprite->IncrensePositionY(jumpVelocity);
		else
		{
			down = true;
		}		
	}	

	if (down && this->sprite->getPositionY() > this->sprite->getInitalPositionY())
		this->sprite->IncrensePositionY(jumpVelocity * -1 / 2);

	this->ResetJump();
	this->checkParallax();
}



void Jump::checkParallax() {
	for (int i = 0; i < this->parallax->size() -1 ; i++)
	{		
		if ((this->parallax->at(i).getPositionY() - this->parallax->at(i).getInitalPositionY() <= 50.0) && (jump || down) &&!this->parallax->at(i).getActionActive()) {
			this->parallax->at(i).IncrensePositionY(jumpVelocity / 2);
		}
		else if (this->parallax->at(i).getPositionY() > this->parallax->at(i).getInitalPositionY()) {
			this->parallax->at(i).setActionActive(true);
			this->parallax->at(i).IncrensePositionY(jumpVelocity * -1 / 4);
		}
		else {
			this->parallax->at(i).setActionActive(false);
		}
	}
}


void Jump::Down()
{
	if (this->sprite->getPositionY() > this->sprite->getInitalPositionY())
	{
		down = true;
		this->sprite->IncrensePositionY(jumpVelocity * -1 / 2);
	}
	
}



void Jump::ResetJump() {
	if (this->sprite->getPositionY() == this->sprite->getInitalPositionY())
	{		
		jump = false;
		down = false;
	}
}