#include "pch.h"
#include "Widgets.h"

namespace Influx::Widgets
{
	// Window
	Window::Window(const std::string& name)
		: mIsOpen{ ImGui::Begin(name.c_str()) } {}
	Window::~Window()
	{
		ImGui::End();
	}

	// Group
	Group::Group(const std::string& name)
		: mIsOpen{ ImGui::CollapsingHeader(name.c_str(), 0)}{}

	// Treenode:
	TreeNode::TreeNode(const std::string& name)
		: mIsOpen{ ImGui::TreeNode(name.c_str()) }{}
	TreeNode::~TreeNode()
	{
		if (mIsOpen) ImGui::TreePop();
	}

	// Text
	Text::Text(const std::string& text) { ImGui::Text(text.c_str()); }
	BulletText::BulletText(const std::string& text) { ImGui::BulletText(text.c_str()); }

	// Spacing
	Spacing::Spacing() { ImGui::Spacing(); }
	Separator::Separator() { ImGui::Separator(); }
}