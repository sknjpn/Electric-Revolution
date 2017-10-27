#include"Planet.h"

Planet::Planet(Game* _game)
	: game(_game)
	, mapTexture(L"assets/map/mapTexture.png")
	, camera(Rect(1280, 720), 4.0, 1.0)
{
	//マップの基礎情報読み込み
	{
		JSONReader json(L"assets/map/planet.json");
		name = json[L"name"].get<String>();
	}

	//都市データの読み込み
	{
		JSONReader json(L"assets/map/urbans.json");
		urbans.reserve(json.arrayCount());	//ポインタをずらさない為にUrbanのメモリ確保
		for (auto j : json.arrayView()) urbans.emplace_back(this, j);
	}
}