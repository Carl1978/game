//
//  Car.cpp
//  game
//
//  Created by Carl Phillips on 21/10/2018.
//
//

#include "Car.h"

USING_NS_CC;

Car::Car() : Entity() {
	//cocos2d::log("Car ctr...");
}

Car::~Car()
{
	//cocos2d::log("Car dtr...");
}

Sprite3D* Car::spawn(const std::string filename, Vec3 P) {
	Entity::spawn(Vec2(P.x, P.y));

	pSprite3D = Sprite3D::create(filename);
	pSprite3D->setTexture("wood5c.png"); // 512x512
	pSprite3D->setContentSize(Size(32, 32)); // 2d plane
	pSprite3D->setScaleX(5.0f);
	pSprite3D->setScaleY(10.0f); // height of cube / car
	pSprite3D->setScaleZ(5.0f);
	pSprite3D->setForce2DQueue(true);
	pSprite3D->setCameraMask((unsigned short)CameraFlag::USER1);

	Pos = startPos = P;
	pSprite3D->setPosition3D(P);

	Vel = Vec3(0.0f, 0.0f, 0.0f);
	Vel.z = 2.0f; // hack for  now!

	type = "Car";
	cocos2d::log("Car::spawn()...");
	return pSprite3D;
}

Car* Car::create(const std::string filename, Vec3 P) {
	Car* pCar = new Car();
	pCar->spawn(filename, P);
	return pCar;
}

void Car::init(void) {
	// TODO:
}

void Car::deinit(void) {
	// TODO:
}

void Car::process(void) {
	cocos2d::log("Vel.z: %f", Vel.z);
	if (Vel.z > 0 && Vel.y < Vel.z) {
		Vel.y += 0.02f;
		cocos2d::log("AAA");
	}
	else if (Vel.z < 0 && Vel.y > Vel.z) {
		Vel.y -= 0.02f;
		cocos2d::log("BBB");
	}
	else {
		cocos2d::log("CCC");
	}

	Pos.y += Vel.y;

	pSprite3D->setPosition3D(Pos);

	cyc++;
	seq++;
}

void Car::draw(void) {
	// TODO:
}