#include "DialogTriggerEntity.h"
#include "DialogManager.h"
#include "App.h"
#include "Hud.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneVillage.h"
#include "Hud.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "SceneShop.h"
#include "SceneOasisFaraon.h"
#include "SceneSelection.h"
#include "SceneTemple.h"
#include "SceneFloor1.h"
#include "FadeToBlack.h"
#include "SceneChoza.h"
#include "Npcs.h"
#include "Animation.h"

#include <thread>
#include <chrono>
#include <future>

DialogTrigger::DialogTrigger() : Entity(EntityType::DIALOG_TRIGGER)
{
	name.Create("dialogTrigger");
}

DialogTrigger::~DialogTrigger() {}

bool DialogTrigger::Awake() {

	return true;
}

bool DialogTrigger::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	faceTexturePath = parameters.attribute("facetexturepath").as_string("");
	repeatDialog = parameters.attribute("repeat").as_bool(false);
	DialogMission = parameters.attribute("mission").as_bool(false);
	dialogScene = parameters.attribute("scene").as_int();
	id = parameters.attribute("id").as_int();

	played = false;
	std::string fontTarget = parameters.attribute("font").as_string("primary");

	//Cargar dialogos
	for (pugi::xml_node itemNode = parameters.child("sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		dialogues.Add(app->dialogManager->CreateDialog(itemNode, parameters.attribute("name").as_string(), faceTexturePath, fontTarget.c_str()));
	}

	//Si el dialogo se reite, cargar las lineas que se repite
	if (repeatDialog) {
		for (pugi::xml_node itemNode = parameters.child("repeat_sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
		{
			dialoguesRepeat.Add(app->dialogManager->CreateDialog(itemNode, parameters.attribute("name").as_string(), faceTexturePath, fontTarget.c_str()));
		}
	}

	if (DialogMission) {
		for (pugi::xml_node itemNode = parameters.child("fraseMision").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
		{
			dialoguesMission.Add(app->dialogManager->CreateDialog(itemNode, parameters.attribute("name").as_string(), faceTexturePath, fontTarget.c_str()));
		}
	}
	


	return true;
}

bool DialogTrigger::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	if (app->sceneVillage->active && dialogScene== app->sceneVillage->sceneNum)
	{
		if (!physCreated) CreateCollider();
	}
	else if (app->sceneShop->active && dialogScene == app->sceneShop->sceneNum)
	{
		if (!physCreated) CreateCollider();

	}
	else if (app->sceneOasisFaraon->active && dialogScene == app->sceneOasisFaraon->sceneNum)
	{
		if (!physCreated) CreateCollider();

	}
	else if (app->sceneTemple->active && dialogScene == app->sceneTemple->sceneNum)
	{
		if (!physCreated) CreateColliderBig();
	}
	else if (app->sceneFloor1->active && dialogScene == app->sceneFloor1->sceneNum)
	{
		if (!physCreated) CreateCollider();

	}
	else if (app->sceneChoza->active && dialogScene == app->sceneChoza->sceneNum)
	{
		if (!physCreated) CreateCollider();

	}
	else if (physCreated)
	{
    app->physics->world->DestroyBody(pbody->body);
    physCreated = false;
	}

	if (app->hud->mission32Active && !app->hud->mission3Complete) {
		DialogMission = false;
	}

	return true;
}

bool DialogTrigger::CleanUp()
{

	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(faceTexture);

	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	for (item = dialogues.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		pDialog->CleanUp();
		SDL_DestroyTexture(pDialog->face_tex);
	}

	dialogues.Clear();


	pDialog = nullptr;

	for (item = dialoguesRepeat.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		SDL_DestroyTexture(pDialog->face_tex);
	}

	dialoguesRepeat.Clear();

	pDialog = nullptr;

	for (item = dialoguesMission.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		pDialog->CleanUp();
		SDL_DestroyTexture(pDialog->face_tex);
	}

	dialoguesMission.Clear();

	return true;
}

void DialogTrigger::PlayDialog()
{
	if ((id==3 && app->sceneSelection->currentSelection!=0) || (id == 4 &&  app->sceneSelection->currentSelection != 1)|| (id == 5 && app->sceneSelection->currentSelection != 3)|| (id == 6 &&  app->sceneSelection->currentSelection != 2))
	{
		return;
	}
	if (DialogMission)

	{
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;
		app->audio->PlayFx(dialogs[rand() % 2]);;
		for (item = dialoguesMission.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
		}
		//DialogMission = false;
	}else if ((played && !repeatDialog && !DialogMission) || !played) {
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;
		app->audio->PlayFx(dialogs[rand() % 2]);
		for (item = dialogues.start; item != NULL; item = item->next)
		{

			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
		}
		played = true;


		//Play el dialogo repetido
	}
	else if (played && repeatDialog) {


		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;
		app->audio->PlayFx(dialogs[rand() % 2]);;
		for (item = dialoguesRepeat.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
		}

	}
	Interact(id);
}

void DialogTrigger::OnCollision(PhysBody* physA, PhysBody* physB) {
	GamePad& pad = app->input->pads[0];
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		if (!app->dialogManager->isPlaying && (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN||(pad.x==KEY_DOWN&& !wasXPressed))) {
			PlayDialog();
			wasXPressed = true;
		}
		else if (pad.x!=KEY_DOWN)
		{
			wasXPressed = false;
		}
		break;
	}
}

void DialogTrigger::CreateCollider()
{
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 200, 120, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::DIALOG_TRIGGER;
	physCreated = true;
}

void DialogTrigger::CreateColliderBig()
{
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 100, 300, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::DIALOG_TRIGGER;
	physCreated = true;
}

void DialogTrigger::Interact(int id)
{
	switch (id)
	{
	case 1:
		GiveMission(1);
		break;
	case 2:
		GiveMission(2);
		break;
	case 3:
		GiveMission(3);
		break;
	case 4:
		GiveMission(4);
		break;
	case 5:
		GiveMission(5);
		break;
	case 6:
		GiveMission(6);
		break;
	case 7:
		GiveMission(7);
		break;
	case 8:
		GiveMission(8);
		break;
	case 9:
		GiveMission(9);
		break;
	default:
		break;
	}

}

void DialogTrigger::GiveMission(int idMission)
{
	switch (idMission)
	{
	case 1:
		printf(" La abuela  \n");
		app->hud->mission10Active = true;
		break;
	case 2:
	/*	printf("  la nieta  \n");*/
		app->hud->mission11Active = false;
		app->hud->mission1Complete = true;
		//app->fade->Fade((Module*)app->sceneChoza, (Module*)app->sceneVillage, 60.0f);
		app->hud->exp += 1;
		break;
	case 3:
		//maat
		app->dialogManager->background_tex_logo = app->dialogManager->background_tex_logoMaat;
		if (app->hud->mission21Active && app->sceneSelection->currentSelection == 0)
		{
			app->hud->mission21Active = false;
			app->hud->mission2Complete = true;
			app->hud->coin += 20;
			app->hud->exp += 1;
			app->hud->mission30Active = true;
			PlayDialog();
		}
		else
		{
			if (app->sceneSelection->currentSelection == 0 && app->hud->mission3Complete)
			{
				app->hud->abilityTree = true;
			}
			else if (!app->hud->mission3Complete && !app->hud->mission32Active && app->hud->mission2Complete && app->sceneSelection->currentSelection == 0)
			{
				app->hud->mission30Active = true;
				app->hud->mission20Active = false;
				app->hud->mission21Active = false;
			}
			else if (!app->hud->mission3Complete && app->hud->mission32Active)
			{
				DialogMission = false;
				app->hud->mission3Complete = true;
				app->hud->coin += 50;
				app->hud->exp += 1;
			}
		}
		break;
	case 4:
		//toth
		app->dialogManager->background_tex_logo = app->dialogManager->background_tex_logoThoth;
		if (app->hud->mission21Active && app->sceneSelection->currentSelection == 1)
		{
			app->hud->mission21Active = false;
			app->hud->mission2Complete = true;
			app->hud->coin += 20;
			app->hud->exp += 1;
			app->hud->mission30Active = true; 
			PlayDialog();
		}
		else
		{
			if (app->sceneSelection->currentSelection == 1 && app->hud->mission3Complete)
			{
				app->hud->abilityTree = true;
			}
			else if (!app->hud->mission3Complete && !app->hud->mission32Active && app->hud->mission2Complete && app->sceneSelection->currentSelection == 1)
			{
				app->hud->mission30Active = true;
				app->hud->mission20Active = false;
				app->hud->mission21Active = false;
			}
			else if (!app->hud->mission3Complete && app->hud->mission32Active)
			{
				DialogMission = false;
				app->hud->mission3Complete = true;
				app->hud->coin += 50;
				app->hud->exp += 1;
			}
		}
		break;
	case 5:
		//isis
		app->dialogManager->background_tex_logo = app->dialogManager->background_tex_logoIsis;
		if (app->hud->mission21Active && app->sceneSelection->currentSelection == 3)
		{
			app->hud->mission21Active = false;
			app->hud->mission2Complete = true;
			app->hud->coin += 20;
			app->hud->exp += 1;
			app->hud->mission30Active = true;
			PlayDialog();
		}
		else
		{
			if (app->sceneSelection->currentSelection == 3 && app->hud->mission3Complete)
			{
				app->hud->abilityTree = true;
			}
			else if (!app->hud->mission3Complete && !app->hud->mission32Active && app->hud->mission2Complete && app->sceneSelection->currentSelection == 3)
			{
				app->hud->mission30Active = true;
				app->hud->mission20Active = false; 
				app->hud->mission21Active = false;
			}
			else if (!app->hud->mission3Complete && app->hud->mission32Active)
			{
				DialogMission = false;
				app->hud->mission3Complete = true;
				app->hud->coin += 50;
				app->hud->exp += 1;
			}
		}
		break;
	case 6:
		//Horrus
		app->dialogManager->background_tex_logo = app->dialogManager->background_tex_logoHorus;
		if (app->hud->mission21Active && app->sceneSelection->currentSelection == 2)
		{
			app->hud->mission21Active = false;
			app->hud->mission2Complete = true;
			app->hud->coin += 20;
			app->hud->exp += 1;
			app->hud->mission30Active = true;
			PlayDialog();
		}
		else
		{
			if (app->sceneSelection->currentSelection == 2 && app->hud->mission3Complete)
			{
				app->hud->abilityTree = true;
			}
			else if (!app->hud->mission3Complete && !app->hud->mission32Active && app->hud->mission2Complete && app->sceneSelection->currentSelection == 2)
			{
				app->hud->mission30Active = true;
				app->hud->mission20Active = false;
				app->hud->mission21Active = false;
			}
			else if (!app->hud->mission3Complete && app->hud->mission32Active)
			{
				DialogMission = false;
				app->hud->mission3Complete = true;
				app->hud->coin += 50;
				app->hud->exp += 1;
			}
		}
		break;
	case 7:
		app->sceneShop->mercante->currentAnim = &app->sceneShop->mercante->interactAnim;
		app->dialogManager->background_tex_logo = app->dialogManager->background_tex_logoMercante;
		printf("  mi humilde tienda \n");
		app->hud->shop = true;
		break;
	case 8:
		app->sceneOasisFaraon->tabernero->currentAnim = &app->sceneOasisFaraon->tabernero->interactAnim;
		app->dialogManager->background_tex_logo = app->dialogManager->background_tex_logoTabernero;
		printf(" Soy el tabernero\n");
		if (!app->hud->mission2Complete)
		{
			app->hud->mission20Active = true;
		}
		break;
	case 9:
		app->sceneFloor1->paalaya->currentAnim = &app->sceneFloor1->paalaya->interactAnim;
		app->dialogManager->background_tex_logo = app->dialogManager->background_tex_logoPalaya;
		printf(" Que haces pidiendome whisky con cereales \n");
		break;
	default:
		break;
	}
}