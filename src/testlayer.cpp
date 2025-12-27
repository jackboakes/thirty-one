#include "testlayer.h"
#include "globals.h"
#include "hand.h"
#include "card.h"
#include <algorithm>

class SceneRoot : public Element {
protected:
	void OnRender() override {}
	void OnLayout() override {}
};

TestLayer::TestLayer()
{
	// Virtual screen for game area
	m_gameCanvas = LoadRenderTexture(GameResolution::width, GameResolution::height);
	SetTextureFilter(m_gameCanvas.texture, TEXTURE_FILTER_BILINEAR);

	m_SceneRoot = std::make_unique<SceneRoot>();
	auto playerHand = std::make_unique<Hand>();
	float xPos = (GameResolution::f_Width - playerHand->size.x) * 0.5f;
	float yPos = GameResolution::f_Height - playerHand->size.y - 40.0f;
	playerHand->localPosition = { xPos, yPos };

	Hand* handPtr = playerHand.get();

	auto aceOfSpades = std::make_unique<Card>(Suit::Spades, Rank::Ace);
	handPtr->AddCard(std::move(aceOfSpades));

	auto tenOfSpades = std::make_unique<Card>(Suit::Spades, Rank::Ten);
	handPtr->AddCard(std::move(tenOfSpades));

	auto jackOfHearts = std::make_unique<Card>(Suit::Hearts, Rank::Jack);
	handPtr->AddCard(std::move(jackOfHearts));

	auto kingCard = std::make_unique<Card>(Suit::Clubs, Rank::King);
	handPtr->AddCard(std::move(kingCard));

	m_SceneRoot->AddChild(std::move(playerHand));
}

TestLayer::~TestLayer()
{
	UnloadRenderTexture(m_gameCanvas);
	m_SceneRoot.reset();
}

void TestLayer::Update(float deltaTime)
{
	if (m_SceneRoot) 
	{
		m_SceneRoot->Layout();
	}
}

void TestLayer::Draw()
{
	BeginTextureMode(m_gameCanvas);
	ClearBackground(BLANK);

	// turn this on to see where the texture2d is rendering
	//DrawRectangle(0, 0, GameResolution::width, GameResolution::height, Fade(BLACK, 0.5f));

	if (m_SceneRoot) 
	{
		m_SceneRoot->Render();
	}
	EndTextureMode();

	float windowWidth { static_cast<float>(GetScreenWidth()) };
	float windowHeight { static_cast<float>(GetScreenHeight()) };
	float scale { std::min(windowWidth / GameResolution::f_Width, windowHeight / GameResolution::f_Height) };
	float scaledWidth { GameResolution::f_Width * scale };
	float scaledHeight { GameResolution::f_Height * scale };
	float offsetX { (windowWidth - scaledWidth) * 0.5f };
	float offsetY { (windowHeight - scaledHeight) * 0.5f };

	// Overlay the game canvas
	DrawTexturePro(
		m_gameCanvas.texture,
		{ 0, 0, GameResolution::f_Width, -GameResolution::f_Height },
		{ offsetX, offsetY, scaledWidth, scaledHeight },
		{ 0, 0 },
		0.0f,
		WHITE
	);
}