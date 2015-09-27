﻿#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для степени
class CDegrControlModel : public IBaseExprModel {
public:
	CDegrControlModel( );
	~CDegrControlModel( )
	{
	}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;

	void Resize( );

	void PermutateChildren( );

	int GetMiddle() const;

	ViewType GetType( ) const;
private:
	// Показатель
	std::shared_ptr<CExprControlModel> child;
};