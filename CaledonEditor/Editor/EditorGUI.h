/*------------------------------
| File: EditorGUI.h
| Author: Chandler Mays
------------------------------*/
#pragma once

class EditorGUI
{
private:
    bool m_isInitialized;                                               //
    bool m_isLayoutInitialized;

    void BuildDefaultLayout();

public:
    EditorGUI();                                                        // Constructor
    ~EditorGUI();                                                       // Destructor
    EditorGUI(const EditorGUI&) = delete;                               // Prevent copy-construction
    EditorGUI& operator=(const EditorGUI&) = delete;                    // Prevent copy-assignment
    EditorGUI(EditorGUI&&) = delete;                                    // Prevent move-construction
    EditorGUI& operator=(EditorGUI&&) = delete;                         // Prevent move-assignment

    bool Initialize();                                                  // Prepares the EditorGUI for use
    void BeginFrame();                                                  // 
    void EndFrame();
    void ProcessEvent(const void* pEvent);
    void Shutdown();

    void BeginDockspace();
    void EndDockspace();
};