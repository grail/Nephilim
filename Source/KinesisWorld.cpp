#include "Nephilim/KinesisWorld.h"
#include "Nephilim/Renderer.h"
#include "Nephilim/KinesisQueryCallback.h"

#include <iostream>
using namespace std;

NEPHILIM_NS_BEGIN

KinesisWorld::KinesisWorld()
: m_world(new b2World(b2Vec2(0.f, 1.4f)))
, m_velocityIterations(6)
, m_positionIterations(6)
, m_pixelsPerMeter(100)
{
	pickerMouseJoint = NULL;
	defaultGroundBody = NULL;
}

KinesisWorld::KinesisWorld(Vec2f gravity, int velocityIterations, int positionIterations)
: m_world(new b2World(b2Vec2(gravity.x, gravity.y)))
, m_positionIterations(positionIterations)
, m_velocityIterations(velocityIterations)
, m_pixelsPerMeter(100)
{
	pickerMouseJoint = NULL;
	defaultGroundBody = NULL;
}

/// Get the raw b2World object
b2World* KinesisWorld::get()
{
	return m_world;
}

void KinesisWorld::SetVelocityIterationCount(int velocityIterations){
	m_velocityIterations = velocityIterations;
}
		
int KinesisWorld::GetVelocityIterationCount(){
	return m_velocityIterations;
};
		
void KinesisWorld::SetPositionIterationCount(int positionIterations){
	m_positionIterations = positionIterations;
};
		
int KinesisWorld::GetPositionIterationCount(){
	return m_positionIterations;
};

b2Body* KinesisWorld::GetGroundBody(){
	if(!defaultGroundBody)
		return CreateDefaultGroundBody();
	else
		return defaultGroundBody;
};

b2Body* KinesisWorld::CreateDefaultGroundBody()
{
	b2BodyDef def;
	def.position = b2Vec2(ToMeters(1), ToMeters(1));
	def.type = b2_staticBody;

	b2PolygonShape box;
	box.SetAsBox(ToMeters(2/2), ToMeters(2/2));

	b2Body *body = m_world->CreateBody(&def);

	b2FixtureDef fixDef;
	fixDef.shape = &box;
	fixDef.density = 0.0f;
	fixDef.friction = 0.5f;
	fixDef.restitution = 0.5f;

	body->CreateFixture(&fixDef);

	return body;
}


void KinesisWorld::update(float elapsedTime)
{
	m_world->Step(elapsedTime, m_velocityIterations, m_positionIterations);		
};

bool KinesisWorld::StartPicking(float x, float y)
{
		
		if(pickerMouseJoint)
			StopPicking();

		KinesisQueryCallback query;
		b2AABB aabb;
		float real_x_mouse = ToMeters(x);
		float real_y_mouse = ToMeters(y);

		aabb.lowerBound.Set(real_x_mouse - 0.002f, real_y_mouse - 0.002f);
		aabb.upperBound.Set(real_x_mouse + 0.002f, real_y_mouse + 0.002f);
		
		
		m_world->QueryAABB(&query, aabb);

		for(unsigned int i = 0; i < query.getFixtureCount(); i++){
			
			if(query.getFixture(i)->GetBody()->GetType() == b2_staticBody){
				
			}
			else{
				if(query.getFixture(i)->TestPoint(b2Vec2(real_x_mouse, real_y_mouse))){
					b2MouseJointDef jointDef;
					jointDef.bodyA = GetGroundBody();
					jointDef.bodyB = query.getFixture(i)->GetBody();
					jointDef.target = b2Vec2(real_x_mouse, real_y_mouse);
					jointDef.maxForce = 10000;
					//jointDef.frequencyHz = 1/60;
					query.getFixture(i)->GetBody()->SetAwake(true);

					pickerMouseJoint = (b2MouseJoint*)m_world->CreateJoint(&jointDef);
					return true;
				}
				else return false;
				
			}
		}

		return false;
		return false;
}

void KinesisWorld::StopPicking()
{
	if(IsPickingEnabled()){
		m_world->DestroyJoint(pickerMouseJoint);
		pickerMouseJoint = NULL;
	}
}

bool KinesisWorld::IsPickingEnabled(){
	return pickerMouseJoint ? true : false;
}

void KinesisWorld::UpdatePicking(float x, float y){
	if(pickerMouseJoint){
		pickerMouseJoint->SetTarget(b2Vec2(ToMeters(x), ToMeters(y)));		
	}
}

b2Body* KinesisWorld::CreateQuickCircle(float x, float y, float r){
		b2BodyDef def;
		def.position = b2Vec2(ToMeters(x),ToMeters(y));
		def.type = b2_dynamicBody;

		b2CircleShape circle;
		circle.m_radius = ToMeters(r);
		circle.m_p = b2Vec2(0, 0);		

		b2Body *body = m_world->CreateBody(&def);

		b2FixtureDef fixDef;
		fixDef.shape = &circle;
		fixDef.density = 1.0f;
		fixDef.friction = 0.5f;
		fixDef.restitution = 0.5f;

		body->CreateFixture(&fixDef);

		return body;
}

KinesisBodyActor* KinesisWorld::CreateQuickBox(float x, float y, float width, float height)
{
		b2BodyDef def;
		def.position = b2Vec2(ToMeters(x), ToMeters(y));
		def.type = b2_dynamicBody;
		

		b2PolygonShape box;
		box.SetAsBox(ToMeters(width/2), ToMeters(height/2));
	

		b2Body *body = m_world->CreateBody(&def);

		b2FixtureDef fixDef;
		fixDef.shape = &box;
		fixDef.density = 0.5f;
		fixDef.friction = 0.5f;
		fixDef.restitution = 0.f;


		body->CreateFixture(&fixDef);

		KinesisBodyActor *bodyActor = new KinesisBodyActor(body);
		body -> SetUserData(bodyActor);
		bodyActor->m_body = body;
		return bodyActor;
}

b2Body* KinesisWorld::CreateStaticBox(float x, float y, float width, float height){
		b2BodyDef def;
		def.position = b2Vec2(ToMeters(x), ToMeters(y));
		def.type = b2_staticBody;

		b2PolygonShape box;
		box.SetAsBox(ToMeters(width/2), ToMeters(height/2));

		b2Body *body = m_world->CreateBody(&def);

		b2FixtureDef fixDef;
		fixDef.shape = &box;
		//fixDef.density = 1.0f;
		fixDef.friction = 0.5f;
		fixDef.restitution = 0.1f;

		body->CreateFixture(&fixDef);

		KinesisBodyActor* Actor = new KinesisBodyActor(body);
		body->SetUserData(Actor);

		return body;
}

void KinesisWorld::CreateQuickLine(float x, float y, float xx, float yy)
{
		/*b2BodyDef def;
		def.position = b2Vec2(ToMeters(xx)/ToMeters(x), ToMeters(yy)/ToMeters(y));
		def.type = b2_dynamicBody;

		b2PolygonShape box;
		b2Vec2 v[2];
		v[0] = b2Vec2(ToMeters(x), ToMeters(y));
		v[1] = b2Vec2(ToMeters(xx), ToMeters(yy));
		box.Set(v, 2);

		b2Body *body = CreateBody(&def);

		b2FixtureDef fixDef;
		fixDef.shape = &box;
		fixDef.density = 1.0f;
		fixDef.friction = 0.5f;
		fixDef.restitution = 0.5f;

		body->CreateFixture(&fixDef);*/
}


	/*KinesisDebugDraw* KinesisWorld::GetDebugRenderer(){
		return &myDebugDraw;
	};*/

	float KinesisWorld::getPixelRatio(){
		return m_pixelsPerMeter;
	};


	float KinesisWorld::ToMeters(float amount){
		return amount/getPixelRatio();
	};

	float KinesisWorld::ToPixels(float amount){
		return amount*getPixelRatio();
	};

NEPHILIM_NS_END	