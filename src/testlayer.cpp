#include "testlayer.h"
#include "globals.h"
#include "card.h"
#include <algorithm>

TestLayer::TestLayer()
{
	m_cardAtlas = LoadTexture("../assets/cardAtlas.png");
	SetTextureFilter(m_cardAtlas, TEXTURE_FILTER_POINT);

	// Virtual screen for game area
	m_gameCanvas = LoadRenderTexture(GameResolution::width, GameResolution::height);
	SetTextureFilter(m_gameCanvas.texture, TEXTURE_FILTER_BILINEAR);
}

TestLayer::~TestLayer()
{
	UnloadTexture(m_cardAtlas);
	UnloadRenderTexture(m_gameCanvas);
}

void TestLayer::Update(float deltaTime)
{

}

void TestLayer::Draw()
{
	BeginTextureMode(m_gameCanvas);
	ClearBackground(BLANK);

	// turn this on to see where the texture2d is rendering
	DrawRectangle(0, 0, GameResolution::width, GameResolution::height, Fade(BLACK, 0.5f));

	float cardW = m_cardAtlas.width / 17.0f;
	float cardH = m_cardAtlas.height / 4.0f;

	Card testCard = { Suit::Spades, Rank::King };
	Rectangle src = testCard.GetSourceRec(cardW, cardH);
	Rectangle dest = { GameResolution::width / 2, GameResolution::height / 2, cardW, cardH };
	DrawTexturePro(m_cardAtlas, src, dest, { 0,0 }, 0.0f, WHITE);

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