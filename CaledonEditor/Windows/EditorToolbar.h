/*------------------------------
| File: EditorToolbar.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#include <string>

class Editor;

class EditorToolbar
{
private:
	bool m_isPlaying = false;
	bool m_isPaused = false;

	void DrawPlayControls(Editor& editor);
	void DrawSaveControls(Editor& editor);
	void DrawSaveAsDialog(Editor& editor);
	void DrawSceneStatus(Editor& editor);

public:
	EditorToolbar() = default;
	~EditorToolbar() = default;
	EditorToolbar(const EditorToolbar&) = delete;
	EditorToolbar& operator=(const EditorToolbar&) = delete;
	EditorToolbar(EditorToolbar&&) = delete;
	EditorToolbar& operator=(EditorToolbar&&) = delete;

	void Draw(Editor& editor);
	bool IsPlaying() const;
	bool IsPaused() const;
};