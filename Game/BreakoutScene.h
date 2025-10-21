#pragma once

#include "Scene.h"
#include "Actor.h"
#include "PaddleActor.h"

class BreakoutScene : public Scene
{
public:
	BreakoutScene(class Game* game);
	~BreakoutScene();

	void update(float deltaTime) override;
	void draw() override;

	PaddleActor* getPaddle() { return m_paddle.get(); }
	std::vector<Actor*>& getBlocks() { return m_blocks; }

private:
	std::vector<Actor*> m_balls;
	std::unique_ptr<PaddleActor> m_paddle;
	std::vector<Actor*> m_blocks;
};