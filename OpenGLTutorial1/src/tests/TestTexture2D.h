#pragma once

#include "Test.h"

namespace test {

	class TestTexture2D : public Test {
	private:
	public:
		TestTexture2D();							// Constructor
		~TestTexture2D();							// Destructor

		void OnUpdate(float deltaTime) override;	// Each frame (or deltaTime)
		void OnRender() override;					// Render objects
		void OnImGuiRender() override;				// Render GUI
	};

}