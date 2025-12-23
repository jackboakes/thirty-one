#include "application.h"
#include "BackgroundLayer.h"
#include "TestLayer.h"

int main()
{
	Application& application { Application::Instance() };
	application.PushLayer<BackgroundLayer>();
	application.PushLayer<TestLayer>();
	application.Run();
}