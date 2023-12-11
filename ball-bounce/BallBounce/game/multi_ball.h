#ifndef MULTIBALL_H
#define MULTIBALL_H

#include "multi_ball.h"
#include "ball_object.h"

//MultiBall contains a linked list of BallObjects.
class MultiBall {
	public:
		//Node for the linked list of BallObjects.
		struct Node {
			BallObject data;
			Node* next;
		};

		//Head node of the linked list.
		Node* head;

		//Constructor and destructors.
		MultiBall();
		MultiBall(BallObject& data);
		~MultiBall();

		//Function to add a BallObject to the front of the linked list.
		void addFront(BallObject& data);

		//Function to check if the linked list is empty.
		bool isEmpty();
};

#endif