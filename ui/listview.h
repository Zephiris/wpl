//	Copyright (c) 2011-2014 by Artem A. Gevorkyan (gevorkyan.org)
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#pragma once

#include "widget.h"

#include <string>

namespace wpl
{
	namespace ui
	{
		struct listview : widget
		{
			typedef size_t index_type;
			struct columns_model;
			struct model;
			struct trackable;
			static const index_type npos = static_cast<index_type>(-1);

			virtual void set_columns_model(std::shared_ptr<columns_model> cm) = 0;
			virtual void set_model(std::shared_ptr<model> ds) = 0;

			virtual void adjust_column_widths() = 0;

			virtual void select(index_type item, bool reset_previous) = 0;
			virtual void clear_selection() = 0;

			virtual void ensure_visible(index_type item) = 0;

			signal<void (index_type /*item*/)> item_activate;
			signal<void (index_type /*item*/, bool /*became selected*/)> selection_changed;
		};

		struct listview::columns_model : destructible
		{
			typedef short int index_type;

			struct column;

			static const short int npos = -1;

			virtual index_type get_count() const throw() = 0;
			virtual void get_column(index_type index, column &column) const = 0;
			virtual void update_column(index_type index, short int width) = 0;
			virtual std::pair<index_type, bool> get_sort_order() const throw() = 0;
			virtual void activate_column(index_type column) = 0;

			signal<void (index_type /*new_ordering_column*/, bool /*ascending*/)> sort_order_changed;
		};

		struct listview::columns_model::column
		{
			column();
			explicit column(const std::wstring &caption, short int width = 0);

			std::wstring caption;
			short int width;
		};

		struct listview::model : destructible
		{
			typedef listview::index_type index_type;

			virtual index_type get_count() const throw() = 0;
			virtual void get_text(index_type row, index_type column, std::wstring &text) const = 0;
			virtual void set_order(index_type column, bool ascending) = 0;
			virtual void precache(index_type from, index_type count) const;
			virtual std::shared_ptr<const listview::trackable> track(index_type row) const;

			signal<void (index_type /*new_count*/)> invalidated;
		};

		struct listview::trackable : destructible
		{
			virtual listview::index_type index() const = 0;
		};



		inline void listview::model::precache(index_type /*from*/, index_type /*count*/) const
		{	}

		inline std::shared_ptr<const listview::trackable> listview::model::track(index_type /*row*/) const
		{	return std::shared_ptr<listview::trackable>();	}


		inline listview::columns_model::column::column()
		{	}

		inline listview::columns_model::column::column(const std::wstring &caption_, short int width_)
			: caption(caption_), width(width_)
		{	}
	}
}
