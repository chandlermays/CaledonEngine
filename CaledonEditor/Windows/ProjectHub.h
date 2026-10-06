/*------------------------------
| File: ProjectHub.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#include <functional>
#include <string>

class ProjectMenu;

class ProjectHub
{
private:
	std::string m_errorMessage;

public:
	void Draw(ProjectMenu& projectMenu, const std::function<void(const std::string&)>& onOpenRequested);

	void SetError(const std::string& message);
	void ClearError();
};