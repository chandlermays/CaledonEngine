/*------------------------------
| File: ViewportPanel.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <memory>

namespace CE
{
	class Texture;
	class SceneManager;
}

class EditorContext;

class ViewportPanel
{
private:
	std::shared_ptr<CE::Texture> m_pRenderTexture;
	int m_width;
	int m_height;

	void EnsureRenderTexture(int width, int height, CE::SceneManager* pSceneManager);

public:
	ViewportPanel();

	void Draw(EditorContext& context);
};