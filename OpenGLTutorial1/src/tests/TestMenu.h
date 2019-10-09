#pragma once

#include "Test.h"
#include <functional>
#include <vector>
#include <string>

namespace test {

	class TestMenu : public Test {
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	public:
		TestMenu(Test*& currentTestPointer);							// Constructor
		~TestMenu();						// Destructor

		void OnImGuiRender() override;		// Render GUI

		template<typename T>
		void RegisterTest(const std::string& testName) {
			std::cout << "Registering test: " << testName << std::endl;
			m_Tests.push_back(std::make_pair(testName, []() { return new T(); }));
		}
	};

}