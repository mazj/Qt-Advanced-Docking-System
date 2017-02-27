/*******************************************************************************
** QtAdcancedDockingSystem
** Copyright (C) 2017 Uwe Kindler
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


//============================================================================
/// \file   DockWidgetTitleBar.cpp
/// \author Uwe Kindler
/// \date   27.02.2017
/// \brief  Implementation of CDockWidgetTitleBar class
//============================================================================


//============================================================================
//                                   INCLUDES
//============================================================================
#include "DockWidgetTitleBar.h"

#include <QBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QStyle>

#include "DockWidget.h"

namespace ads
{
/**
 * Private data class of CDockWidgetTitleBar class (pimpl)
 */
struct DockWidgetTitleBarPrivate
{
	CDockWidgetTitleBar* _this;
	CDockWidget* DockWidget;
	QLabel* IconLabel;
	QLabel* TitleLabel;
	QPoint DragStartMousePosition;
	bool IsActiveTab = false;

	/**
	 * Private data constructor
	 */
	DockWidgetTitleBarPrivate(CDockWidgetTitleBar* _public);

	/**
	 * Creates the complete layout including all controls
	 */
	void createLayout();
};
// struct DockWidgetTitleBarPrivate

//============================================================================
DockWidgetTitleBarPrivate::DockWidgetTitleBarPrivate(CDockWidgetTitleBar* _public) :
	_this(_public)
{

}


//============================================================================
void DockWidgetTitleBarPrivate::createLayout()
{
	QBoxLayout* l = new QBoxLayout(QBoxLayout::LeftToRight);
	l->setContentsMargins(0, 0, 0, 0);
	_this->setLayout(l);

	IconLabel = new QLabel();
	IconLabel->setAlignment(Qt::AlignVCenter);
	l->addWidget(IconLabel, Qt::AlignVCenter);

	TitleLabel = new QLabel();
	l->addWidget(TitleLabel, 1);

	IconLabel->setVisible(false);
	TitleLabel->setVisible(true);
	TitleLabel->setText(DockWidget->windowTitle());
}


//============================================================================
CDockWidgetTitleBar::CDockWidgetTitleBar(CDockWidget* DockWidget, QWidget *parent) :
	QFrame(parent),
	d(new DockWidgetTitleBarPrivate(this))
{
	d->DockWidget = DockWidget;
	d->createLayout();
}

//============================================================================
CDockWidgetTitleBar::~CDockWidgetTitleBar()
{
	delete d;
}


//============================================================================
void CDockWidgetTitleBar::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		ev->accept();
        d->DragStartMousePosition = ev->pos();
		return;
	}
	QFrame::mousePressEvent(ev);
}



//============================================================================
void CDockWidgetTitleBar::mouseReleaseEvent(QMouseEvent* ev)
{
    if (!d->DragStartMousePosition.isNull())
    {
		emit clicked();
    }

    d->DragStartMousePosition = QPoint();
}


//============================================================================
void CDockWidgetTitleBar::mouseMoveEvent(QMouseEvent* ev)
{

}


//============================================================================
bool CDockWidgetTitleBar::isActiveTab() const
{
	return d->IsActiveTab;
}


//============================================================================
void CDockWidgetTitleBar::setActiveTab(bool active)
{
	if (d->IsActiveTab == active)
	{
		return;
	}

	d->IsActiveTab = active;
	style()->unpolish(this);
	style()->polish(this);
	d->TitleLabel->style()->unpolish(d->TitleLabel);
	d->TitleLabel->style()->polish(d->TitleLabel);
	update();

	emit activeTabChanged();
}


//============================================================================
CDockWidget* CDockWidgetTitleBar::dockWidget() const
{
	return d->DockWidget;
}
} // namespace ads

//---------------------------------------------------------------------------
// EOF DockWidgetTitleBar.cpp
