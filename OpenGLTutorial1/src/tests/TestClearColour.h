#pragma once

#include "Test.h"

namespace test {

	class TestClearColour : public Test {
	private:
		float m_ClearColour[4];
	public:
		TestClearColour();							// Constructor
		~TestClearColour();							// Destructor

		void OnRender() override;					// Render objects
		void OnImGuiRender() override;				// Render GUI
	};

}