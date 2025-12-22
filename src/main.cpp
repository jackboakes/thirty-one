#include "application.h"

int main()
{
	Application& application { Application::Instance() };
	application.Run();
}