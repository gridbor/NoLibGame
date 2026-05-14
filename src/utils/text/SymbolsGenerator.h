#pragma once
#include "GLFunctions.h"
#include "Symbol.h"


namespace text {

	class SymbolsGenerator {
	public:
		SymbolsGenerator();
		~SymbolsGenerator();

		static inline SymbolsGenerator& Get() { return *s_instance; }

		void Init();
		void Render();

	private:
		static inline SymbolsGenerator* s_instance = nullptr;
		std::unique_ptr<Symbol> m_currentSymbol;

	};

}
