#include "Common.h"
#include "GameEngine.h"
#include "MainMenu.h"
#include "GameApplication.h"
#include "State.h"

namespace PangPang
{
class PangpangApp : public GameApplication
{
public:
	PangpangApp(int argc, char* argv[])
		:	GameApplication(argc, argv, L"Pangpang", Size(800,600)),
		renderer(this), stateManager(StateData(this, &renderer, this))
	{
		stateManager.SetState<MainMenu>();
	}
	~PangpangApp()
	{
	}
private:
	virtual void UpdateGame(double deltaTime)
	{
		stateManager.Update(deltaTime);
	}
private:
	Renderer renderer;
	StateManager stateManager;
};
}

int main(int argc, char *argv[])
{
	using namespace PangPang;

	PangpangApp pangpangApp(argc, argv);

	pangpangApp.Run();
}

#if defined(WIN32)
int APIENTRY WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	return main(__argc, __argv);
}
#endif

