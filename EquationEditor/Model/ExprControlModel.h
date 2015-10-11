﻿#pragma once
#include "Model/IBaseExprModel.h"

// Модель холдера под вьюшки
class CExprControlModel : public IBaseExprModel {
public:
	// Прямоугольник соседнего текстового поля, высота которого будет совпадать с высотой каждого текстового поля дроби
	CExprControlModel( const CRect& newRect, const std::weak_ptr<IBaseExprModel> newParent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;

	void InitializeChildren();

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;
	
	// Вставляет нового ребенка после curChild
	// Если curChild == nullptr, вставляет в конец списка
	void AddChildAfter( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> curChild );
	void AddChildBefore( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> curChild );

	ViewType GetType() const;

	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );

	bool IsEmpty() const;

	bool IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const;

	void UpdateSelection();

	bool DeleteSelectedPart();
private:
	std::list<std::shared_ptr<IBaseExprModel>> children;

	// высота оси, относительно которой центрируются все модели внутри данного выражения
	int middle;
};