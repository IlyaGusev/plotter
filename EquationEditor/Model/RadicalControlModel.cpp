﻿#include "Model/RadicalControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"


CRadicalControlModel::CRadicalControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel(rect, parent)
{
	depth = parent.lock()->GetDepth() + 1;
}

void CRadicalControlModel::Resize()
{
	int width = firstChild->GetRect().GetWidth() + secondChild->GetRect().GetWidth() + 10; // 10 пикселей - ширина галочки
	int height = MAX( firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() - secondChild->GetMiddle(), secondChild->GetRect().GetHeight() + 3 ); // 3 - отступ над подкоренным выражением

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CRadicalControlModel::PlaceChildren()
{
	CRect newRect;

	CRect oldRect = firstChild->GetRect();
	newRect.Bottom() = rect.Top() + GetMiddle() - 3;
	newRect.Top() = newRect.Bottom() - oldRect.GetHeight();
	newRect.Left() = rect.Left();
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	firstChild->SetRect(newRect);
	
	oldRect = secondChild->GetRect();
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = newRect.Bottom() - oldRect.GetHeight();
	newRect.Left() = firstChild->GetRect().Right() + 10; // 10 - расстояние для галочки
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	secondChild->SetRect( newRect );

	updatePolygons();
}

int CRadicalControlModel::GetMiddle() const
{
	return rect.GetHeight() - secondChild->GetRect().GetHeight() + secondChild->GetMiddle();
}

void CRadicalControlModel::InitializeChildren()
{
	CRect firstChildRect = CRect( 0, 0, 0, 3 * getDegreeHeight( rect.GetHeight() ) );
	firstChild = std::make_shared<CExprControlModel>( firstChildRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	firstChild->InitializeChildren();

	CRect secondChildRect = CRect( 0, 0, 0, rect.GetHeight() );
	secondChild = std::make_shared<CExprControlModel>( secondChildRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	secondChild->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CRadicalControlModel::GetChildren() const 
{
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CRadicalControlModel::SetRect(const CRect& rect) 
{
	this->rect = rect;

	updatePolygons();
}

ViewType CRadicalControlModel::GetType() const 
{
	return RADICAL;
}

void CRadicalControlModel::MoveBy(int dx, int dy) 
{
	rect.MoveBy(dx, dy);
	updatePolygons();
}

void CRadicalControlModel::MoveCaretLeft(const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */) 
{
	// Если пришли из подкоренного выражения - идём в показатель
	if( from == secondChild.get() ) {
		firstChild->MoveCaretLeft( this, caret, isInSelectionMode );
	}
	//если пришли из родителя - идём в подкоренное выражение
	else if( from == parent.lock().get() ) {
		secondChild->MoveCaretLeft( this, caret, isInSelectionMode );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret, isInSelectionMode );
	}
}

void CRadicalControlModel::MoveCaretRight(const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode/*=false */) 
{
	// Если пришли из родителя - идем в показатель
	if( from == parent.lock().get() ) {
		firstChild->MoveCaretRight( this, caret, isInSelectionMode );
	}
	// если из показателя - в подкоренное выражение
	else if( from == firstChild.get() ) {
		secondChild->MoveCaretRight( this, caret, isInSelectionMode );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
	}
}

bool CRadicalControlModel::IsEmpty() const 
{
	return firstChild->IsEmpty() && secondChild->IsEmpty();
}

bool CRadicalControlModel::IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const {
	return model1 == firstChild.get();
}

// высота показателя степени
int CRadicalControlModel::getDegreeHeight( int rectHeight )
{
	return rectHeight / 4 > CEditControlModel::MINIMAL_HEIGHT ? rectHeight / 4 : CEditControlModel::MINIMAL_HEIGHT;
}

void CRadicalControlModel::updatePolygons()
{
	params.polygon.clear();
	auto firstRect = firstChild->GetRect();
	auto secondRect = secondChild->GetRect();

//	params.polygon.push_back( CLine( firstRect.Left(), firstRect.Bottom(), firstRect.Right(), firstRect.Bottom() ) );  // вариант с чертой под степенью корня
	params.polygon.push_back( CLine( firstRect.Right() - 3, firstRect.Bottom() + 3, firstRect.Right(), firstRect.Bottom() ) ); // вариант с небольшим крючком под степенью корня
	params.polygon.push_back( CLine( firstRect.Right(), firstRect.Bottom(), firstRect.Right() + 5, rect.Bottom() ) );
	params.polygon.push_back( CLine( secondRect.Left() - 5, rect.Bottom(), secondRect.Left(), rect.Top() + 2 ) );
	params.polygon.push_back( CLine( secondRect.Left(), rect.Top() + 2, rect.Right(), rect.Top() + 2 ) );
	params.polygon.push_back( CLine( rect.Right(), rect.Top() + 2, rect.Right(), rect.Top() + 5 ) );
}

void CRadicalControlModel::UpdateSelection()
{
	if( firstChild->IsSelected() && secondChild->IsSelected() ) {
		params.isSelected = true;
	} else {
		params.isSelected = false;
	}
}