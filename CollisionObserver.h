#ifndef __COLLISIONOBSERVER_H__
#define __COLLISIONOBSERVER_H__

struct Collider;
using namespace std;

class CollisionObserver {
public:
	CollisionObserver() { }
	virtual void OnCollision(Collider* collider1, Collider* collider2) = 0;
};

#endif // __COLLISIONOBSERVER_H__