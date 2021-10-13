#include "Jump.h"

const int jumpVelocity = 10.0;

Jump::Jump(Sprite* sprite, vector<Sprite>* connectedObjects)
{
	this->sprite = sprite;
	jump = false;
	up = false;
	down = false;		
	this->connectedObjects = connectedObjects;
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
	this->checkConnectedObjects();
}



void Jump::checkConnectedObjects() {
	for (int i = 0; i < this->connectedObjects->size() -1 ; i++)
	{
		
		if ((this->connectedObjects->at(i).getPositionY() - this->connectedObjects->at(i).getInitalPositionY() <= 50.0) && (jump || down) &&!this->connectedObjects->at(i).getActionActive()) {
			this->connectedObjects->at(i).IncrensePositionY(jumpVelocity / 2);
		}
		else if (this->connectedObjects->at(i).getPositionY() > this->connectedObjects->at(i).getInitalPositionY()) {
			this->connectedObjects->at(i).setActionActive(true);
			this->connectedObjects->at(i).IncrensePositionY(jumpVelocity * -1 / 4);
		}
		else {
			this->connectedObjects->at(i).setActionActive(false);
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