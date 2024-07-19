#include "Model.h"
#include "WorldTransform.h"
#include "Viewprojection.h"

class Skydome{
public:
	void Initialize(); 

	void Update();

	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	// モデル
	Model* model_ = nullptr;
};