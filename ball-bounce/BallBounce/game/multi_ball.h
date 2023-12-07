#ifndef MULTIBALL_H
#define MULTIBALL_H

#include "multi_ball.h"
#include "ball_object.h"

class MultiBall {
	public:
		struct Node {
			BallObject data;
			Node* next;
		};

		int size;
		Node* head;

		MultiBall();
		MultiBall(BallObject& data);
		~MultiBall();

		void addFront(BallObject& data);
		bool isEmpty();
		int getSize();
};

#endif