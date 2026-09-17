/*------------------------------
| File: EditorGUI.h
| Author: Chandler Mays
------------------------------*/
#pragma once

class EditorGUI
{
private:
    bool m_isInitialized;
    bool m_isLayoutInitialized;

    void BuildDefaultLayout();

public:
    EditorGUI();
    ~EditorGUI();
    EditorGUI(const EditorGUI&) = delete;
    EditorGUI& operator=(const EditorGUI&) = delete;
    EditorGUI(EditorGUI&&) = delete;
    EditorGUI& operator=(EditorGUI&&) = delete;

    bool Initialize();
    void BeginFrame();
    void EndFrame();
    void Shutdown();

    void BeginDockspace();
    void EndDockspace();
};