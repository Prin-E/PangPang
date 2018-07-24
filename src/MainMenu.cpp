#include "Common.h"
#include "MainMenu.h"
#include "InGameState.h"

namespace PangPang
{
MainMenu::MainMenu(StateManager *ptr) 
	: State(ptr), renderer(GetStateData().renderer), uiManager(renderer),
	backgroundImage(new UI::ImageView()), pvpButton(new UI::Button(GetStateData().window)),
	pvaiButton(new UI::Button(GetStateData().window)),
	versionLabel(new UI::Label()), copyrightLabel(new UI::Label())
{
	// ��� �׸�
	backgroundImage->SetImageAtPath("dir/MainMenuBack1.tga");
	backgroundImage->SetPosition(Point(0, 0));
	backgroundImage->SetSize(Size(800, 600));
	backgroundImage->SetFrameRect(Rect(0, 0, 800, 600));

	// 1vs1 ��ư
	pvpButton->SetPosition(Point(272, 200));
	pvpButton->SetSize(Size(256, 64));
	pvpButton->SetTextureAtPath("dir/MainMenuButton1.tga");
	pvpButton->SetFrameSize(Size(256, 64));
	pvpButton->SetText(L"1 vs 1");
	pvpButton->SetTextSize(30);
	pvpButton->SetTag(0);
	pvpButton->RegisterHitEventCallback(boost::bind(&MainMenu::StartGame, this, _1));

	// 1vs��ǻ�� ��ư
	pvaiButton->SetPosition(Point(272, 100));
	pvaiButton->SetSize(Size(256, 64));
	pvaiButton->SetTextureAtPath("dir/MainMenuButton1.tga");
	pvaiButton->SetFrameSize(Size(256, 64));
	pvaiButton->SetText(L"1 vs ��ǻ��");
	pvaiButton->SetTextSize(30);
	pvaiButton->SetTag(1);
	pvaiButton->RegisterHitEventCallback(boost::bind(&MainMenu::StartGame, this, _1));

	// ����
	versionLabel->SetPosition(Point(10, 578));
	versionLabel->SetSize(Size(300, 12));
	versionLabel->SetTextSize(12);
	versionLabel->SetText(L"Version Alpha 2010-12-24, 2018-07-23");

	// ���۱�
	copyrightLabel->SetPosition(Point(10, 10));
	copyrightLabel->SetSize(Size(780, 12));
	copyrightLabel->SetTextSize(12);
	copyrightLabel->SetTextAlignment(UI::TextAlignmentRight);
	copyrightLabel->SetText(L"Kim Jeonggi, Lee hyeonwoo. University of Seoul. �� 2010, 2018.");

	uiManager.AddUI(backgroundImage, 0);
	uiManager.AddUI(pvpButton, 1);
	uiManager.AddUI(pvaiButton, 2);
	uiManager.AddUI(versionLabel, 2);
	uiManager.AddUI(copyrightLabel, 2);
}

void MainMenu::OnEnter()
{
	std::cerr << __FUNCTION__ << std::endl;
	uiManager.Register();
}

void MainMenu::OnExit()
{
	std::cerr << __FUNCTION__ << std::endl;
	uiManager.Unregister();
}

void MainMenu::Update(float deltaTime)
{
	renderer->Draw();
}

void MainMenu::StartGame(uint32 tag)
{
	std::cout << "��ư�� ���Ƚ��ϴ�. �±� : " << tag << std::endl;

	//tag ---   0 : 1vs1, 1 : 1vsComputer
	//if(tag == 0)
	if(tag == 0)
		SetState<InGameState>(InGameState::PvP);
	else if(tag == 1)
		SetState<InGameState>(InGameState::PvAI);
}
}

