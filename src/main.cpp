#include "application.h"
#include "BackgroundLayer.h"
#include "TestLayer.h"

int main()
{
	Application& application { Application::Instance() };
	application.PushLayer(new BackgroundLayer());
	application.PushLayer(new TestLayer());
	application.Run();
}