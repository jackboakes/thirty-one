#include "application.h"
#include "backgroundlayer.h"
#include "gameplaylayer.h"

int main()
{
	Application& application { Application::Instance() };
	application.PushLayer<BackgroundLayer>();
	application.PushLayer<GameplayLayer>();
	application.Run();
}