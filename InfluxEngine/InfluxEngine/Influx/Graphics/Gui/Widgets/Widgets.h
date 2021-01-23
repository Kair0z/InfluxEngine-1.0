#pragma once

namespace Influx::Widgets
{
	struct Window final
	{
		Window(const std::string& name);
		~Window();

		operator bool() const { return mIsOpen; }

		const bool mIsOpen;
	};

	struct Group final
	{
		Group(const std::string& name);
		operator bool() const { return mIsOpen; }

		const bool mIsOpen;
	};

	struct TreeNode final
	{
		TreeNode(const std::string& name);
		~TreeNode();

		operator bool() const { return mIsOpen; }
		const bool mIsOpen;
	};

	struct Text final { Text(const std::string& text); };
	struct BulletText final { BulletText(const std::string& text); };

	struct Spacing final { Spacing(); };
	struct Separator final { Separator(); };
}


