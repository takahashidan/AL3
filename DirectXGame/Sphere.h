#include "WorldTransform.h"
#include"ViewProjection.h"
#include <Model.h>

class Sphere {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Model* model_ = nullptr;
};