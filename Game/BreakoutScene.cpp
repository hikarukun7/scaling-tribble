#include "BreakoutScene.h"
#include "Game.h"
#include "BallActor.h"
#include "BlockActor.h"

BreakoutScene::BreakoutScene(Game* game)
    : Scene(game)
{
    {
        //ボール生成
        BallActor* ball = new BallActor(this, 10.0f, RGB(255, 255, 30),
            Vector2d(50.0f, 600.0f), Vector2d(100.0f, -200.0f));
        m_balls.push_back(ball);

        ball = new BallActor(this, 10.0f, RGB(255, 30, 255),
            Vector2d(400.0f, 520.0f), Vector2d(-80.0f, -180.0f));
        m_balls.push_back(ball);
    }

    {
        //パドル生成
        m_paddle = std::make_unique<PaddleActor>(this, 120.0f, 30.0f,
            RGB(255, 30, 30), RGB(128, 128, 128), Vector2d(240.0f, 600.0f));
    }

    {
        //ブロック生成
        //サイズ
        float bw = 50.0f, bh = 30.0f; 
        //個数
        int nx = 9, ny = 9;
        Vector2d base(15.0f, 20.0f);  
        for (int j = 0; j < ny; ++j)
        {
            for (int i = 0; i < nx; ++i)
            {
                float x = base.x + bw * (i + 0.5f);
                float y = base.y + bh * (j + 0.5f);
                BlockActor* block = new BlockActor(this, bw, bh,
                    RGB(30, 255, 30), RGB(128, 128, 128), Vector2d(x, y));
                m_blocks.push_back(block);
            }
        }
    }
    m_isRunning = true;
}

BreakoutScene::~BreakoutScene()
{
    releaseActors(m_balls);
    releaseActors(m_blocks);
}

void BreakoutScene::update(float deltaTime)
{
    m_paddle->update(deltaTime);
    updateActors(m_balls, deltaTime);

    removeActors(m_balls);
    removeActors(m_blocks);
}

void BreakoutScene::draw()
{
    m_paddle->draw();
    drawActors(m_blocks);
    drawActors(m_balls);

    int n = (int)m_balls.size();
    m_game->drawString(L"ボールの個数: %d", n,
        ZeroVec2d, RGB(0, 0, 0), 20);
}

