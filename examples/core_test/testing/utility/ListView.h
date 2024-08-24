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

    <no description>

*/

#pragma once

#include <oddf/utility/ListView.h>

#include <iostream>
#include <memory>
#include <type_traits>

namespace oddf::testing::utility {

bool Test_ListView();

template<template<typename> class containerTemplate>
inline bool TestListViewForStdContainer()
{
	struct Base {

		virtual int GetValue() const = 0;
		virtual ~Base() { }
	};

	struct Derived : public Base {

	private:

		int m_Value;

	public:

		Derived(int value) :
			m_Value(value) { }

		virtual int GetValue() const override { return m_Value; }
	};

	// We use a unique_ptr of a derived class as the element type because we want to
	// test the various type casts that are supported by our container-view classes
	using elementT = std::unique_ptr<Derived>;

	// These are our source elements. We put them in a plain array because this allows inplace construction.
	elementT src[5] = {

		std::make_unique<Derived>(6),
		std::make_unique<Derived>(4),
		std::make_unique<Derived>(5),
		std::make_unique<Derived>(3),
		std::make_unique<Derived>(2)
	};

	// Create empty container object of given template type.
	using containerT = containerTemplate<elementT>;
	containerT container;

	// Move elements to the container
	std::move(std::begin(src), std::end(src), std::inserter(container, std::end(container)));

	//
	// No type conversion on the element type
	//

	// Create default ListView
	auto listView = oddf::utility::MakeListView(container);

	// ListView should be copyable into CollectionView
	oddf::utility::CollectionView collectionView = listView;

	// Get an Enumerator from the copied CollectionView
	auto enumerator = collectionView.GetEnumerator();

	// Confirm the element types returned by the ListView and the Enumerator
	static_assert(std::is_same_v<decltype(listView[0]), decltype(container[0])>);
	static_assert(std::is_same_v<decltype(enumerator.GetCurrent()), decltype(*container.begin())>);

	// Create a ListView where the elements of type unique_ptr<Derived> are converted to a plain pointer to const Base.
	auto listViewToConstBaseReference = oddf::utility::MakeListView<Base const *>(container);
	auto enumeratorToConstBaseReference = listViewToConstBaseReference.GetEnumerator();

	// Confirm the element types returned by the ListView and the Enumerator
	static_assert(std::is_same_v<decltype(listViewToConstBaseReference[0]), Base const *>);
	static_assert(std::is_same_v<decltype(enumeratorToConstBaseReference.GetCurrent()), Base const *>);

	// Create a ListView where the elements of type unique_ptr<Derived> are converted to a reference
	auto listViewToReference = oddf::utility::MakeListView<Derived &>(container);
	auto enumeratorToReference = listViewToReference.GetEnumerator();

	// Confirm the element types returned by the ListView and the Enumerator
	static_assert(std::is_same_v<decltype(listViewToReference[0]), Derived &>);
	static_assert(std::is_same_v<decltype(enumeratorToReference.GetCurrent()), Derived &>);

	if (collectionView.GetSize() != 5)
		return false;

	if (listViewToConstBaseReference.GetSize() != 5)
		return false;

	if (listViewToReference.GetSize() != 5)
		return false;

	size_t i = 0;
	enumerator.Reset();

	while (enumerator.MoveNext()) {

		if (enumerator.GetCurrent()->GetValue() != listViewToConstBaseReference[i]->GetValue())
			return false;

		if (enumerator.GetCurrent()->GetValue() != listViewToReference[i].GetValue())
			return false;
		++i;
	}

	if (i != collectionView.GetSize())
		return false;

	return true;
}

} // namespace oddf::testing::utility
