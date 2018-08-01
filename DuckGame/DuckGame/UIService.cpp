#include "UIService.h"

UIService::UIService()
{

}

UIService::UIService(Application application, Spawner spawner)
{
	this->application = application;
	this->spawner = spawner;
}

UIService::~UIService()
{
	this->ui.clear();
}

bool UIService::loadUI(std::vector<const char*> files)
{
	if (files.size() <= 0 || this->ui.size() <= 0)
		return false;

	for (auto file : files) {
		auto model = new Model(file, false);
		if (!model->load(file, false)) {
			return false;
		}
		model->shader(this->pShader, false);
		this->ui.push_back(model);
	}
	return true;
}

void UIService::update(float dtime)
{			  
	this->timePassed = this->application.getTimePassed();
	this->spawnTime = this->spawner.getSpawnTime();
	this->speed = this->spawner.getSpeed();
	this->objectsDodged = this->spawner.getObjectsDodged();
}

void UIService::draw(const BaseCamera & camera)
{
	for (auto model : this->ui) {
		model->draw(camera);
	}
}
