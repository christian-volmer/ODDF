/*

    ODDF - Open Digital Design Framework
    Copyright Advantest Corporation

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

/*

    Classes for the design hierarchy, which supports named instances of
    modules.

*/

#pragma once

namespace dfx {

class Design;

//
// HierarchyLevel class
//

struct HierarchyLevel {

private:

	friend class Hierarchy;

	int sequenceNumber;
	std::string name;
	std::string moduleName;

	HierarchyLevel *parent;
	HierarchyLevel *next;
	HierarchyLevel *firstChild;

	std::unordered_map<std::string, int> blockNameCounters;

public:

	HierarchyLevel();
	~HierarchyLevel() = default;

	HierarchyLevel(HierarchyLevel const &) = delete;
	void operator=(HierarchyLevel const &) = delete;

	int GetSequenceNumber() const;
	std::string const &GetName() const;
	std::string GetFullName() const;

	std::string const &GetModuleName() const;

	HierarchyLevel *GetParent() const;

	std::string GenerateBlockName(std::string const &prefix);
};

//
// Hierarchy class
//

class Hierarchy {

private:

	Design *design;

	HierarchyLevel root;
	HierarchyLevel *current;

	void BeginNewLevel(std::string const &name, std::string const &moduleName);

	void RecursiveDelete(HierarchyLevel *level);
	void RecursivePrintTree(std::basic_ostream<char> &os, HierarchyLevel const *level, int indentation) const;

	friend class ScopedHierarchyLevel;
	friend class ScopedGotoHierarchyLevel;

public:

	~Hierarchy();

	Hierarchy(Hierarchy const &) = delete;
	void operator=(Hierarchy const &) = delete;

	Hierarchy(Design *theDesign);

	HierarchyLevel *GetCurrentLevel() const;

	void Report(std::basic_ostream<char> &os) const;
};

//
// ScopedHierarchyLevel class
//

class ScopedHierarchyLevel {

private:

	Hierarchy &hierarchy;
	HierarchyLevel *previousLevel;

public:

	~ScopedHierarchyLevel();

	ScopedHierarchyLevel(ScopedHierarchyLevel const &) = delete;
	void operator=(ScopedHierarchyLevel const &) = delete;

	ScopedHierarchyLevel(std::string const &name, std::string const &moduleName);
};

#define DFX_INSTANCE(instanceName, moduleName) \
	dfx::ScopedHierarchyLevel _instance(instanceName, moduleName); \
	if (dfx::Design::GetCurrent().hasCustomDefaultEnable) \
	dfx::blocks::_Label(dfx::Design::GetCurrent().customDefaultEnable, "input", "ClockEnable")

//
// ScopedGotoHierarchyLevel class
//

class ScopedGotoHierarchyLevel {

private:

	Hierarchy &hierarchy;
	HierarchyLevel *previousLevel;

public:

	~ScopedGotoHierarchyLevel();

	ScopedGotoHierarchyLevel(ScopedGotoHierarchyLevel const &) = delete;
	void operator=(ScopedGotoHierarchyLevel const &) = delete;

	ScopedGotoHierarchyLevel(HierarchyLevel *gotoLevel);
};

} // namespace dfx
