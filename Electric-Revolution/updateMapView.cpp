#include"Game.h"

void	Game::updateMapView()
{
	//カメラの移動処理更新
	planet.camera.update();

	{
		//視点移動の適用
		auto t = planet.camera.createTransformer();

		//マップテクスチャ
		planet.mapTexture.draw();

		//Urbanの描画
		for (auto& u : planet.urbans)
		{
			if (Circle(u.pos, 6).mouseOver())
			{
				Circle(u.pos, 4).draw(Palette::Red).drawFrame(1, Palette::Black);

				//アイコンが押されたらシーンを移行
				if (MouseL.down())
				{
					sceneState = SceneState::FactoryView;

					mainFactory = &u.factory;
					mainFactory->isMain = true;
				}
			}
			else
			{
				Circle(u.pos, 4).draw(Palette::White).drawFrame(1, Palette::Black);
			}

			//都市名の表示
			font(16)(u.name).drawAt(u.pos.movedBy(0, -12), Palette::Black);
		}
	}
}