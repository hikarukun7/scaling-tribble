#include "Game.h"
#include <exception>
#include "TestScene.h"

const float Game::FrameRate = 60.0f;
const float Game::MaxDeltaTime = 0.05f;

Game::Game()
    : m_hwnd(nullptr)
    , m_width(0)
    , m_height(0)
    , m_startTime{}
    , m_endTime{}
    , m_freqTime{}
{
}

Game::~Game()
{
    //�V�[���̊J��
    if (m_scene.get() != nullptr)
    {
        m_scene.reset();
    }
    
}

void Game::initialize(HWND hwnd, int width, int height)
{
    if (width <= 0 || height <= 0) throw std::exception();

    m_hwnd  = hwnd;
    m_width = width;
    m_height = height;

    //�����_���[�̏�����
    m_rederer = std::make_unique<Renderer>(this);
    if (!m_rederer->initialize()) throw std::exception();

    //���Ԍv���̏�����
    QueryPerformanceFrequency(&m_freqTime);
    QueryPerformanceCounter(&m_startTime);

    //�L�[�{�[�h������
    m_keyboard.initialize();

    //�V�[���̏�����
    m_scene = std::make_unique<TestScene>(this);

    //�Q�[����Ԃ̏�����
}

bool Game::loop()
{
    float deltaTime = 0.0f;
    if (tick(deltaTime))
    {
        input();
        update(deltaTime);
        if (m_scene->isRunning() == false) return false;
        draw();
    }
    return m_scene->isRunning();
}

void Game::input()
{
    //�L�[�{�[�h��Q�[���p�b�h�Ȃǂ̓��͂��󂯎�鏈�����L�q
    m_keyboard.input();

    //if (m_keyboard.isDown(VK_LEFT)) OutputDebugStringW(L"�����L�[��������Ԃł�\n");
    //if (m_keyboard.isPressed('A')) OutputDebugStringW(L"A�L�[��������܂���\n");
    //if (m_keyboard.isUp(VK_RIGHT)) OutputDebugStringW(L"�E���L�[��������Ԃł�\n");
    //if (m_keyborad.isReleased('B')) OutputDebugStringW(L"B�L�[��������܂���\n");
}

void Game::update(float deltaTime) 
{
    m_scene->update(deltaTime);
}

void Game::draw()
{
    m_rederer->begin();
    //�����ɃQ�[���̕`�揈�����L�q
    m_scene->draw();

    m_rederer->end();
}

bool Game::tick(float& deltaTime)
{
    QueryPerformanceCounter(&m_endTime);
    if (m_endTime.QuadPart - m_startTime.QuadPart == 0) return false;

    deltaTime = (float)(m_endTime.QuadPart - m_startTime.QuadPart)
        / (float)m_freqTime.QuadPart;
    if (deltaTime < (1.0f / (FrameRate + 1.0f))) return false;

    m_startTime = m_endTime;
    deltaTime = (deltaTime > MaxDeltaTime) ? MaxDeltaTime : deltaTime;
    return true;
}
