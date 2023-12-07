#include "multi_ball.h"

MultiBall::MultiBall() : head(nullptr), size(0) {}\

MultiBall::MultiBall(BallObject& data) {
	Node* newNode = new Node{data, head};
	head = newNode;
	size = 1;
}

MultiBall::~MultiBall() {
	while (head) {
		Node* toDelete = head;
		head = head->next;
		delete toDelete;
	}
	size = 0;
}

void MultiBall::addFront(BallObject& data) {
	Node* newNode = new Node{ data, nullptr };
	if (head) {
		newNode->next = head;
	}
	head = newNode;
	size++;
}

bool MultiBall::isEmpty() { 
	return head == nullptr; 
}

int MultiBall::getSize() { 
	return size; 
}