#include "multi_ball.h"

//Default constructor.
MultiBall::MultiBall() {
	head = nullptr;
}

//Constructor with a single BallObject.
MultiBall::MultiBall(BallObject& data) {
	Node* newNode = new Node{data, head};
	head = newNode;
}

//Destructor.
MultiBall::~MultiBall() {
	while (head) {
		Node* toDelete = head;
		head = head->next;
		delete toDelete;
	}
}

//Function to add a BallObject to the front of the linked list.
void MultiBall::addFront(BallObject& data) {
	Node* newNode = new Node{data, nullptr};
	if (head) {
		newNode->next = head;
	}
	head = newNode;
}

// Function to check if the linked list is empty.
bool MultiBall::isEmpty() { 
	return head == nullptr; 
}
