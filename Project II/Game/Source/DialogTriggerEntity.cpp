#include "DialogTriggerEntity.h"
#include "DialogManager.h"
#include "App.h"
#include "Hud.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneVillage.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "SceneShop.h"
#include "SceneOasisFaraon.h"
#include "SceneTemple.h"
#include "SceneFloor1.h"
#include "FadeToBlack.h"
#include "SceneChoza.h"
#include "Npcs.h"

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
	


	return true;
}

bool DialogTrigger::Update(float dt)
{
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

	return true;
}

void DialogTrigger::PlayDialog()
{
	//Play el dialogo normal
	if ((played && !repeatDialog) || !played) {
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;
		app->audio->PlayFx(dialogs[rand()%2]);
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
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		if (!app->dialogManager->isPlaying && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			PlayDialog();
		}
		break;
	}
}

void DialogTrigger::CreateCollider()
{
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 80, 120, bodyType::KINEMATIC);
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
		printf("  la nieta  \n");
		app->hud->mission11Active = false;
		app->hud->mission1Complete = true;
		app->fade->Fade((Module*)app->sceneChoza, (Module*)app->sceneVillage, 60.0f);
		break;
	case 3:
		printf(" Soy maat \n");
		break;
	case 4:
		printf(" toth  \n");
		break;
	case 5:
		printf(" Soy ISIS  \n");
		break;
	case 6:
		printf(" Horrus  \n");
		break;
	case 7:
		printf("  mi humilde tienda \n");
		break;
	case 8:
		printf(" Soy el tabernero\n");
		break;
	case 9:
		printf(" Que haces pidiendome wishky con cereales \n");
		break;
	default:
		break;
	}
}