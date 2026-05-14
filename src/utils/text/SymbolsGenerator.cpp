#include "SymbolsGenerator.h"


namespace text {

	SymbolsGenerator::SymbolsGenerator()
	{
		m_currentSymbol = std::make_unique<Symbol>();
		s_instance = this;
	}

	SymbolsGenerator::~SymbolsGenerator()
	{
		m_currentSymbol.reset();
		s_instance = nullptr;
	}

	void SymbolsGenerator::Init()
	{
		m_currentSymbol->Init();
	}

	void SymbolsGenerator::Render()
	{
		m_currentSymbol->Render();
	}

}
